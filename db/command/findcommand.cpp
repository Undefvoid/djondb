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

#include "findcommand.h"

#include "bsonoutputstream.h"
#include "bsoninputstream.h"
#include "dbcontroller.h"
#include "outputstream.h"
#include "util.h"

FindCommand::FindCommand()
: Command(FIND)
{
	//ctor
	_findresult = NULL;
}

FindCommand::~FindCommand()
{
	if (_filter != NULL) delete _filter;
	if (_namespace != NULL) delete _namespace;
	if (_db != NULL) delete _db;
}

FindCommand::FindCommand(const FindCommand& other)
:Command(FIND)
{
	this->_filter = new std::string(*other._filter);
	this->_namespace = new std::string(*other._namespace);
	this->_db = new std::string(*other._db);

	this->_findresult = new std::vector<BSONObj*>();

	for (std::vector<BSONObj*>::const_iterator i = other._findresult->begin(); i != other._findresult->end(); i++) {
		BSONObj* obj = new BSONObj(**i);
		this->_findresult->push_back(obj);
	}
}

void FindCommand::execute() {
	Logger* log = getLogger(NULL);
	if (log->isDebug()) log->debug("executing find command on %s", nameSpace()->c_str());

	_findresult = dbController()->find(const_cast<char*>(DB()->c_str()), const_cast<char*>(nameSpace()->c_str()), filter()->c_str());
	
	delete log;
}

void* FindCommand::result() {
	return _findresult;
}

void FindCommand::writeCommand(OutputStream* out) const {
	out->writeString(*_db);
	out->writeString(*_namespace);
	out->writeString(*_filter);
}

void FindCommand::readResult(InputStream* is) {
	Logger* log = getLogger(NULL);
	if (log->isDebug()) log->debug("writing result of find command on %s", nameSpace()->c_str());

	BSONInputStream* bsonin = new BSONInputStream(is);
	std::vector<BSONObj*>* result = bsonin->readBSONArray();
	_findresult = result;

	delete bsonin;
}

void FindCommand::writeResult(OutputStream* out) const {
	Logger* log = getLogger(NULL);
	if (log->isDebug()) log->debug("writing result of find command on %s", nameSpace()->c_str());
	BSONOutputStream* bsonout = new BSONOutputStream(out);
	bsonout->writeBSONArray(*_findresult);
	delete bsonout;
	delete log;
}

void FindCommand::setNameSpace(const std::string& ns) {
	_namespace = new std::string(ns);
}

std::string* FindCommand::nameSpace() const {
	return _namespace;
}

void FindCommand::setFilter(const std::string& filter) {
	_filter = new std::string(filter);
}

std::string* FindCommand::filter() const {
	return _filter;
}

void FindCommand::setDB(const std::string& db) {
	_db = new std::string(db);
}

const std::string* FindCommand::DB() const {
	return _db;
}

