// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
// 
// This file is part of the djondb project, for license information please refer to the LICENSE file,
// the application and libraries are provided as-is and free of use under the terms explained in the file LICENSE
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

#include "dbcontroller.h"
#include "bson.h"
#include "util.h"
#include "fileinputoutputstream.h"
#include "fileinputstream.h"
#include "bsonbufferedobj.h"
#include "mmapinputstream.h"
#include "fileoutputstream.h"
#include "dbfileinputstream.h"

#include "bsonoutputstream.h"
#include "bsoninputstream.h"
#include "filterparser.h"
#include "baseexpression.h"
#include "expressionresult.h"

#include "cachemanager.h"
#include "indexfactory.h"
#include "index.h"
#include <memory>
#include <iostream>
#include <sstream>
#include <string.h>
#include <assert.h>

using namespace std;

DBController::DBController()
{
	_logger = getLogger(NULL);
}

DBController::~DBController()
{
	delete _logger;
}

void DBController::shutdown() {
	if (_logger->isInfo()) _logger->info("DBController shutting down");
	StreamManager::getStreamManager()->closeDatabases();
	clearCache();
}

void DBController::clearCache() {
	if (_logger->isInfo()) _logger->info("DBController::clearCache");
	for (ObjectCache::iterator i = CacheManager::objectCache()->begin(); i != CacheManager::objectCache()->end(); i++) {
		BSONObj* item = i->second;
		delete item;
	}
	CacheManager::objectCache()->clear();

	for (StructureCache::iterator i = CacheManager::structuresCache()->begin(); i != CacheManager::structuresCache()->end(); i++) {
		Structure* item = i->second;
		delete item;
	}
	CacheManager::structuresCache()->clear();

}

void DBController::initialize() {
	std::string dataDir = getSetting("DATA_DIR");
	if ((dataDir.length() > 0) && !endsWith(dataDir.c_str(), FILESEPARATOR)) {
		dataDir = dataDir + FILESEPARATOR;
	} else {
		dataDir = std::string("\0");
	}

	initialize(dataDir);
	_initialized = true;
}

void DBController::initialize(std::string dataDir) {
	if (_logger->isInfo()) _logger->info("DBController initializing. Data dir");

	_dataDir = dataDir;

	if (_logger->isDebug()) _logger->debug(0, "data dir = %s", _dataDir.c_str());

	if (!existDir(_dataDir.c_str())) {
		makeDir(_dataDir.c_str());
	}

	if (!checkFileCreation(_dataDir.c_str())) {
		_logger->error("An error ocurred using the data folder: %s. Please check that the user has permissions for writing over that directory. Error: %s", _dataDir.c_str(), lastErrorDescription()); 
		exit(1);
	}

	StreamManager::getStreamManager()->setDataDir(_dataDir);
	StreamManager::getStreamManager()->setInitializing(true);

	std::auto_ptr<FileInputStream> fis(new FileInputStream((_dataDir + "djondb.dat").c_str(), "rb"));
	while (!fis->eof()) {
		std::auto_ptr<std::string> db(fis->readString());
		std::auto_ptr<std::string> ns(fis->readString());
		int streams = fis->readInt();
		for (int x = 0; x < streams; x++) {
			FILE_TYPE type = static_cast<FILE_TYPE>(fis->readInt());
			StreamType* stream = StreamManager::getStreamManager()->open(*(db.get()), *(ns.get()), type);

			if (type == INDEX_FTYPE) {
				long currentPos = stream->currentPos();
				stream->seek(0);

				int records = 0;
				IndexAlgorithm* impl = NULL;
				while (!stream->eof()) {
					BSONObj* obj = readBSON(stream);

					if (!impl) {
						std::set<std::string> skeys;
						for (BSONObj::const_iterator i = obj->begin(); i != obj->end(); i++) {
							std::string key = i->first;
							skeys.insert(key);
						}
						impl = IndexFactory::indexFactory.index(db->c_str(), ns->c_str(), skeys);
					}
					long indexPos = stream->readLong();
					long posData = stream->readLong();
					if (obj->has("_id")) {
						impl->add(*obj, obj->getDJString("_id"), posData, indexPos);
						records++;
					}
					delete obj;
				}
				stream->seek(currentPos);

				if (_logger->isInfo()) _logger->info("db: %s, ns: %s, Index initialized. Records: %d", db->c_str(), ns->c_str(), records);
			}
		}
	}
	fis->close();
	StreamManager::getStreamManager()->setInitializing(false);
}

long DBController::checkStructure(BSONObj* obj) {
	Structure* structure = new Structure();
	for (std::map<std::string, BSONContent* >::const_iterator i = obj->begin(); i != obj->end(); i++) {
		structure->add(i->first);
	}

	StructureCache* cache = CacheManager::structuresCache();
	long strId = structure->crc();
	if (!cache->containsKey(strId)) {
		cache->add(strId, structure);
	} else {
		delete(structure);
	}
	return strId;
}

void DBController::writeBSON(StreamType* stream, BSONObj* obj) {
	if (_logger->isDebug()) _logger->debug(3, "DBController is writing bson to disc: %s", obj->toChar());

	auto_ptr<BSONOutputStream> out(new BSONOutputStream(stream));
	out->writeBSON(*obj);
	stream->flush();
}

BSONObj* DBController::readBSON(StreamType* stream) {
	auto_ptr<BSONInputStream> is(new BSONInputStream(stream));
	BSONObj* res = is->readBSON();
	if (_logger->isDebug()) _logger->debug(3, "DBController read bson from disc: %s", res->toChar());
	return res;
}

BSONObj* DBController::insert(char* db, char* ns, BSONObj* obj) {
	if (_logger->isDebug()) _logger->debug(2, "DBController::insert ns: %s, bson: %s", ns, obj->toChar());
	std::string sdb(db);
	std::string sns(ns);
	StreamType* streamData = StreamManager::getStreamManager()->open(sdb, sns, DATA_FTYPE);

	BSONObj* result = NULL;
	if (!obj->has("_id")) {
		if (_logger->isDebug()) _logger->debug(2, "BSON does not contain an id, DBController is creating one");
		string* tid = uuid();
		std::string key("_id");
		obj->add(key, const_cast<char*>(tid->c_str()));
		result = new BSONObj();
		result->add("_id", const_cast<char*>(tid->c_str()));
		delete tid;
	}
	if (!obj->has("_revision")) {
		if (_logger->isDebug()) _logger->debug(2, "BSON does not contain revision, DBController is creating one");
		string* trev = uuid();
		std::string key("_revision");
		obj->add(key, const_cast<char*>(trev->c_str()));
		result = new BSONObj();
		result->add("_revision", const_cast<char*>(trev->c_str()));
		delete trev;
	}
	// _status flag
	obj->add("_status", 1); // Active

	//    long crcStructure = checkStructure(obj);

	//    char* text = obj->toChar();
	//    streamData->writeChars(text, strlen(text));
	//    free(text);

	insertIndex(db, ns, obj, streamData->currentPos());

	writeBSON(streamData, obj);

	//CacheManager::objectCache()->add(id, new BSONObj(*obj));

	return result;
}

void DBController::update(char* db, char* ns, BSONObj* obj) {
	if (_logger->isDebug()) _logger->debug(2, "DBController::update ns: %s, bson: %s", ns, obj->toChar());
	StreamType* streamData = StreamManager::getStreamManager()->open(std::string(db), std::string(ns), DATA_FTYPE);

	Index* index = findIndex(db, ns, obj);

	long currentPos = streamData->currentPos();

	// Moves to the record to update
	streamData->seek(index->posData);

	BSONObj* previous = readBSON(streamData);
	previous->add("_status", 3); // Updated

	streamData->seek(index->posData);
	writeBSON(streamData, previous);

	// Back to the end of the stream
	streamData->seek(currentPos);

	updateIndex(db, ns, index, streamData->currentPos());

	obj->add("_status", 1); // Active

	writeBSON(streamData, obj);

	//std::string id = obj->getDJString("_id");

	//CacheManager::objectCache()->add(id, new BSONObj(*obj));
}

void DBController::remove(char* db, char* ns, const std::string& documentId, const std::string& revision) {
	if (_logger->isDebug()) _logger->debug(2, "DBController::update db: %s, ns: %s, documentId: %s, revision: %s", db, ns, documentId.c_str(), revision.c_str());
	StreamType* streamData = StreamManager::getStreamManager()->open(std::string(db), std::string(ns), DATA_FTYPE);

	IndexAlgorithm* impl = IndexFactory::indexFactory.index(db, ns, "_id");

	BSONObj indexBSON;
	indexBSON.add("_id", const_cast<char*>(documentId.c_str()));
	Index* index = impl->find(&indexBSON);
	if (index != NULL) {

		// TODO check the revision id
		StreamType* out = StreamManager::getStreamManager()->open(db, ns, DATA_FTYPE);
		out->flush();

		long currentPos = out->currentPos();

		out->seek(index->posData);

		BSONObj* obj = readBSON(out);
		obj->add("_status", 2); // DELETED

		// Get back to the record start
		out->seek(index->posData);
		writeBSON(out, obj);

		// restores the last position
		out->seek(currentPos);

		//std::string id = obj->getDJString("_id");

		//CacheManager::objectCache()->remove(id);
		delete obj;
	}
}

Index* DBController::findIndex(char* db, char* ns, BSONObj* bson) {
	IndexAlgorithm* impl = IndexFactory::indexFactory.index(db, ns, "_id");

	BSONObj indexBSON;
	indexBSON.add("_id", bson->getDJString("_id"));
	Index* index = impl->find(&indexBSON);

	return index;
}

void DBController::updateIndex(char* db, char* ns, Index* index, long filePos) {
	index->posData = filePos;

	StreamType* out = StreamManager::getStreamManager()->open(db, ns, INDEX_FTYPE);
	long currentPos = out->currentPos();
	out->seek(index->indexPos);
	BSONObj* key = index->key;
	writeBSON(out, key);
	out->writeLong(index->indexPos);
	out->writeLong(index->posData);
	out->seek(currentPos);
}

void DBController::insertIndex(char* db, char* ns, BSONObj* bson, long filePos) {
	BSONObj indexBSON;
	djondb::string id = bson->getDJString("_id");
	indexBSON.add("_id", (char*)id);

	IndexAlgorithm* impl = IndexFactory::indexFactory.index(db, ns, "_id");

	StreamType* out = StreamManager::getStreamManager()->open(std::string(db), std::string(ns), INDEX_FTYPE);

	impl->add(indexBSON, id, filePos, out->currentPos());

	writeBSON(out, &indexBSON);
	out->writeLong(out->currentPos());
	out->writeLong(filePos);
}

BSONArrayObj* DBController::find(char* db, char* ns, const char* select, const char* filter) throw(ParseException) {
	if (_logger->isDebug()) _logger->debug(2, "DBController::find db: %s, ns: %s, select: %s, filter: %s", db, ns, select, filter);

	BSONArrayObj* result;

	FilterParser* parser = FilterParser::parse(filter);

	std::set<std::string> tokens = parser->tokens();

	if (IndexFactory::indexFactory.containsIndex(db, ns, tokens)) {
		IndexAlgorithm* impl = IndexFactory::indexFactory.index(db, ns, tokens);

		std::list<Index*> elements = impl->find(parser);

		std::string filedir = _dataDir + db;
		filedir = filedir + FILESEPARATOR;

		std::stringstream ss;
		ss << filedir << ns << ".dat";

		std::string filename = ss.str();
		result = new BSONArrayObj();
		FileInputStream* fis = new FileInputStream(filename.c_str(), "rb");
		DBFileInputStream* dbStream = new DBFileInputStream(fis);

		BSONInputStream* bis = new BSONInputStream(dbStream);
		for (std::list<Index*>::iterator it = elements.begin(); it != elements.end(); it++) {
			Index* index = *it;

			long posData = index->posData;
			dbStream->seek(posData);

			BSONObj* obj = bis->readBSON(select);

			result->add(*obj);

			delete obj;
		}
		delete bis;
		delete dbStream;

	} else {
		result = findFullScan(db, ns, select, parser);
	}

	delete parser;
	return result;
}

BSONObj* DBController::findFirst(char* db, char* ns, const char* select, const char* filter) throw(ParseException) {
	if (_logger->isDebug()) _logger->debug(2, "DBController::findFirst db: %s, ns: %s, select: %s, filter: %s", db, ns, select, filter);
	std::string filedir = _dataDir + db;
	filedir = filedir + FILESEPARATOR;

	std::stringstream ss;
	ss << filedir << ns << ".dat";

	std::string filename = ss.str();

	// Execute open on streammanager, just to check that the file was alrady opened
	StreamManager::getStreamManager()->open(db, ns, DATA_FTYPE);

	MMapInputStream* mmis = new MMapInputStream(filename.c_str(), "rb");
	DBFileInputStream* dbStream = new DBFileInputStream(mmis);
	BSONArrayObj result;

	BSONInputStream* bis = new BSONInputStream(mmis);

	FilterParser* parser = FilterParser::parse(filter);

	BSONBufferedObj* obj = NULL;
	BSONObj* bsonResult = NULL;
	mmis->seek(29);
	while (!mmis->eof()) {
		if (obj == NULL) {
			obj = new BSONBufferedObj(mmis->pointer(), mmis->length() - mmis->currentPos());
		} else {
			obj->reset(mmis->pointer(), mmis->length() - mmis->currentPos());
		}
		mmis->seek(mmis->currentPos() + obj->bufferLength());
		// Only "active" Records
		if (obj->getInt("_status") == 1) {
			ExpressionResult* result = parser->eval(*obj);
			if (result->type() == ExpressionResult::RT_BOOLEAN) {
				bool bres = *result;
				if (bres) {
					bsonResult = obj->select(select);
					break;
				}
			}
			delete result;
		}
		if (bsonResult) {
			break;
		}
	}

	if (obj != NULL) delete obj;
	dbStream->close();
	delete dbStream;

	delete parser;
	return bsonResult;
}

BSONArrayObj* DBController::findFullScan(char* db, char* ns, const char* select, FilterParser* parser) throw(ParseException) {
	if (_logger->isDebug()) _logger->debug(2, "DBController::findFullScan with parser db: %s, ns: %s", db, ns);
	std::string filedir = _dataDir + db;
	filedir = filedir + FILESEPARATOR;

	std::stringstream ss;
	ss << filedir << ns << ".dat";

	std::string filename = ss.str();

	// Execute open on streammanager, just to check that the file was alrady opened
	StreamManager::getStreamManager()->open(db, ns, INDEX_FTYPE);
	// Execute open on streammanager, just to check that the file was alrady opened
	StreamManager::getStreamManager()->open(db, ns, DATA_FTYPE);
	//FileInputStream* fis = new FileInputStream(filename.c_str(), "rb");
	MMapInputStream* mmis = new MMapInputStream(filename.c_str(), "rb");
	DBFileInputStream* dbStream = new DBFileInputStream(mmis);
	BSONArrayObj* result = new BSONArrayObj();

	BSONInputStream* bis = new BSONInputStream(dbStream);

	std::set<std::string> tokens = parser->xpathTokens();
	std::string filterSelect;

	if (tokens.size() > 0) {
		// this will reserve enough space to concat the filter tokens
		filterSelect.reserve(tokens.size() * 100);
		filterSelect.append("$'_status'");
		for (std::set<std::string>::iterator i = tokens.begin(); i != tokens.end(); i++) {
			std::string token = *i;
			filterSelect.append(", ");
			filterSelect.append("$'");
			filterSelect.append(token);
			filterSelect.append("'");
		}
	} else {
		filterSelect = "*";
	}

	mmis->seek(29);
	BSONBufferedObj* obj = NULL;

	std::string smax = getSetting("max_results");
	__int64 maxResults = 30;
	if (smax.length() > 0) {
#ifdef WINDOWS
		maxResults = _atoi64(smax.c_str());
#else
		maxResults = atoll(smax.c_str());
#endif
	}
	__int64 count = 0;
	while (!mmis->eof() && (count < maxResults)) {
		if (obj == NULL) {
			obj = new BSONBufferedObj(mmis->pointer(), mmis->length() - mmis->currentPos());
		} else {
			obj->reset(mmis->pointer(), mmis->length() - mmis->currentPos());
		}
		mmis->seek(mmis->currentPos() + obj->bufferLength());
		// Only "active" Records
		if (obj->getInt("_status") == 1) {
			bool match = false;
			ExpressionResult* expresult = parser->eval(*obj);
			if (expresult->type() == ExpressionResult::RT_BOOLEAN) {
				match = *expresult;
			}
			delete expresult;
			if (match) {
				BSONObj* objSubselect = obj->select(select);
				result->add(*objSubselect);
				delete objSubselect;
				count++;
			}
		}
	}

	if (obj != NULL) delete obj;
	delete bis;
	dbStream->close();
	delete dbStream;

	return result;
}

std::vector<std::string>* DBController::dbs() const {
	return StreamManager::getStreamManager()->dbs();
}

std::vector<std::string>* DBController::namespaces(const char* db) const {
	return StreamManager::getStreamManager()->namespaces(db);
}

bool DBController::dropNamespace(char* db, char* ns) {
	return StreamManager::getStreamManager()->dropNamespace(db, ns);
}
