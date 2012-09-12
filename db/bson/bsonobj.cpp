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

#include "bsonobj.h"
#include "util.h"
#include "bsonutil.h"

#include "bsonparser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <set>

using namespace std;

#define MAX_BSONOBJ_BUFFER 8000
BSONObj::BSONObj()
{
}

BSONObj::~BSONObj()
{
	for (std::map<t_keytype, BSONContent*>::const_iterator i = _elements.begin(); i != _elements.end(); i++) {
		BSONContent* cont = i->second;
		delete(cont);
	}
	_elements.clear();
}

/*
	void BSONObj::add(t_keytype key, void* val) {
	fillContent(key, PTR, val);
	}
	*/

void BSONObj::add(t_keytype key, int val) {
	int* internalValue = new int();
	*internalValue = val;
	fillContent(key, INT_TYPE, internalValue);
}

void BSONObj::add(t_keytype key, double val) {
	double* internalValue = new double();
	*internalValue = val;
	fillContent(key, DOUBLE_TYPE, internalValue);
}

void BSONObj::add(t_keytype key, long val) {
	long* internalValue = new long();
	*internalValue = val;
	fillContent(key, LONG_TYPE, internalValue);
}

void BSONObj::add(t_keytype key, std::string val) {
	std::string* internalValue = new std::string(val);
	fillContent(key, STRING_TYPE, internalValue);
}

void BSONObj::add(t_keytype key, const BSONObj& val) {
	BSONObj* internalValue = new BSONObj(val);
	fillContent(key, BSON_TYPE, internalValue);
}

void BSONObj::add(t_keytype key, const BSONArrayObj& val) {
	BSONArrayObj* internalValue = new BSONArrayObj(val);
	fillContent(key, BSONARRAY_TYPE, internalValue);
}


char* BSONObj::toChar() const {
	Logger* log = getLogger(NULL);
	
	char* result = (char*)malloc(MAX_BSONOBJ_BUFFER);
	memset(result, 0, MAX_BSONOBJ_BUFFER);

	int pos = 0;
	result[0] = '{';
	pos += 1;

	bool first = true;
	
	for (std::map<t_keytype, BSONContent* >::const_iterator i = _elements.begin(); i != _elements.end(); i++) {
		if (!first) {
			result[pos] = ',';
			pos++;
		}
		first = false;
		BSONContent* content = i->second;
		t_keytype key = i->first;
		sprintf(result + pos, " \"%s\" : ", key.c_str());
		pos += key.length() + 6;
		//ss << "\"" << key << "\" :";
		char* chr;
		const char* cstr;
		switch (content->type())  {
			case BSON_TYPE:
					chr = ((BSONObj*)content->_element)->toChar();
					sprintf(result + pos, "%s", chr);
					free(chr);
					break;
			case BSONARRAY_TYPE:
					chr = ((BSONArrayObj*)content->_element)->toChar();
					sprintf(result + pos, "%s", chr);
					free(chr);
					break;
			case INT_TYPE: 
					sprintf(result + pos, "%d", *((int*)content->_element));
					break;
			case LONG_TYPE:
					sprintf(result + pos, "%ld", *((long*)content->_element));
					break;
			case DOUBLE_TYPE:
					sprintf(result + pos, "%f", *((double*)content->_element));
					break;
			case STRING_TYPE:
					cstr = ((std::string*)content->_element)->c_str();
					sprintf(result + pos, "\"%s\"", cstr);
					break;
		}
		pos = strlen(result);
		assert(pos < MAX_BSONOBJ_BUFFER);
	}
	result[pos] = '}';
	result[pos+1] = 0;
	pos++;

	int len = strlen(result);

	char* cresult = strcpy(result);

	delete log;
	free(result);
	return cresult;
}

int* BSONObj::getInt(t_keytype key) const {
	BSONContent* content = getContent(key);
	if ((content != NULL) && (content->type() == INT_TYPE)) {
		int* res = (int*)content->_element;
		return res;
	} else {
		return NULL;
	}
}

double* BSONObj::getDouble(t_keytype key) const {
	BSONContent* content = getContent(key);
	if ((content != NULL) && (content->type() == DOUBLE_TYPE)) {
		double* res = (double*)content->_element;
		return res;
	} else {
		return NULL;
	}
}

long* BSONObj::getLong(t_keytype key) const {
	BSONContent* content = getContent(key);
	if ((content != NULL) && (content->type() == LONG_TYPE)) {
		long* res = (long*)content->_element;
		return res;
	} else {
		return NULL;
	}
}

std::string BSONObj::getString(t_keytype key) const {
	BSONContent* content = getContent(key);
	if (!content) {
		return std::string();
	}
	std::string result;
	if (content->type() == STRING_TYPE) {
		result = *(std::string*)content->_element;
	} else {
		assert(false);
	}
	return result;
}

BSONObj* BSONObj::getBSON(t_keytype key) const {
	BSONContent* content = getContent(key);
	if ((content != NULL) && (content->type() == BSON_TYPE)) {
		BSONObj* res = (BSONObj*)content->_element;
		return res;
	} else {
		return NULL;
	}
}

BSONArrayObj* BSONObj::getBSONArray(t_keytype key) const {
	BSONContent* content = getContent(key);
	if ((content != NULL) && (content->type() == BSONARRAY_TYPE)) {
		BSONArrayObj* res = (BSONArrayObj*)content->_element;
		return res;
	} else {
		return NULL;
	}
}

BSONContent* BSONObj::getContent(t_keytype key) const {
	BSONContent* content = NULL;
	for (std::map<t_keytype, BSONContent* >::const_iterator it = _elements.begin(); it != _elements.end(); it++) {
		t_keytype itKey = it->first;
		if (itKey.compare(key) == 0) {
			content = it->second;
			break;
		}
	}
	return content;
}

BSONContent* BSONObj::getContent(t_keytype key, BSONTYPE ttype) const {
	BSONContent* content = getContent(key);
	if (content != NULL) {
		if (content->type() != ttype) {
			throw "type does not match";
		}
	}
	return content;
}

BSONObj::const_iterator BSONObj::begin() const {
	return _elements.begin();
}

BSONObj::const_iterator BSONObj::end() const {
	return _elements.end();
}

int BSONObj::length() const {
	return _elements.size();
}

bool BSONObj::has(t_keytype key) const {
	for (std::map<t_keytype, BSONContent* >::const_iterator it = _elements.begin(); it != _elements.end(); it++) {
		t_keytype itKey = it->first;
		if (itKey.compare(key) == 0) {
			return true;
		}
	}
	return false;
}

BSONObj::BSONObj(const BSONObj& orig) {
	for (std::map<t_keytype, BSONContent* >::const_iterator i = orig._elements.begin(); i != orig._elements.end(); i++) {
		t_keytype key = i->first;
		BSONContent* origContent = i->second;
		BSONContent* content = new BSONContent(*origContent);

		this->_elements.insert(pair<t_keytype, BSONContent* >(i->first, content));
	}
}

BSONTYPE BSONObj::type(t_keytype key) const {
	BSONContent* content = NULL;
	//    SEARCHBSON(key, STRING_TYPE);
	for (std::map<t_keytype, BSONContent* >::const_iterator it = _elements.begin(); it != _elements.end(); it++) {
		t_keytype itKey = it->first;
		if (itKey.compare(key) == 0) {
			content = it->second;
			break;
		}
	}
	if (content != NULL) {
		return content->type();
	} else {
		return UNKNOWN_TYPE;
	}
}

void* BSONObj::get(t_keytype key) const {
	BSONContent* content = NULL;
	//    SEARCHBSON(key, STRING_TYPE);
	for (std::map<t_keytype, BSONContent* >::const_iterator it = _elements.begin(); it != _elements.end(); it++) {
		t_keytype itKey = it->first;
		if (itKey.compare(key) == 0) {
			content = it->second;
			break;
		}
	}
	if (content != NULL) {
		return content->_element;
	} else {
		return NULL;
	}
}

BSONContent BSONObj::getXpath(const std::string& xpath) const {
	int posDot = xpath.find('.');
	if (posDot == string::npos) {
		BSONContent* result = getContent(xpath);
		if (result != NULL) {
			return *result;
		} else {
			return BSONContent(); // this will return a NULL_TYPE
		}
	} else {
		std::string path = xpath.substr(0, posDot);
		BSONContent* content = getContent(path);
		if (content == NULL) {
			return BSONContent();
		}
		if (content->type() == BSON_TYPE) {
			BSONObj inner = (BSONObj)*content;
			return inner.getXpath(xpath.substr(posDot + 1));
		} else {
			return BSONContent();
		}
	}

	return BSONContent();
}

bool BSONObj::operator ==(const BSONObj& obj) const {
	if (this->has("_id") && obj.has("_id")) {
		BSONContent* idThis = this->getContent("_id");
		BSONContent* idOther = obj.getContent("_id");

		return (*idThis == *idOther);
	}
	// Element count
	if (this->length() != obj.length()) {
		return false;
	}
	for (BSONObj::const_iterator it = this->begin(); it != this->end(); it++) {
		t_keytype key = it->first;
		BSONContent* content = it->second;

		BSONContent* other = obj.getContent(key);
		if (other == NULL) {
			return false;
		}
		if (*content != *other) {
			return false;
		}
	}
	return true;
}

bool BSONObj::operator !=(const BSONObj& obj) const {
	if (this->has("_id") && obj.has("_id")) {
		BSONContent* idThis = this->getContent("_id");
		BSONContent* idOther = obj.getContent("_id");

		return (*idThis != *idOther);
	}

	// Element count
	if (this->length() != obj.length()) {
		return true;
	}
	for (BSONObj::const_iterator it = this->begin(); it != this->end(); it++) {
		t_keytype key = it->first;
		BSONContent* content = it->second;

		BSONContent* other = obj.getContent(key);
		if (other == NULL) {
			return true;
		}
		if (*content != *other) {
			return true;
		}
	}
	return false;
}

BSONObj* BSONObj::select(const char* sel) const {
	std::set<std::string> columns = bson_splitSelect(sel);
	bool include_all = (strcmp(sel, "*") == 0);

	BSONObj* result = new BSONObj();

	for (std::map<t_keytype, BSONContent* >::const_iterator i = this->_elements.begin(); i != this->_elements.end(); i++) {
		t_keytype key = i->first;
		if (include_all || (columns.find(key) != columns.end())) {
			BSONContent* origContent = i->second;

			switch (origContent->type()) {
				case BSON_TYPE:  
					{
						BSONObj inner = (BSONObj)*origContent;
						
					   if (!include_all) {
							char* subselect = bson_subselect(sel, key.c_str());
							BSONObj* innerSubSelect = inner.select(subselect);
							result->add(key, *innerSubSelect);
							delete innerSubSelect;
						} else {
							result->add(key, inner);
						}
						break;
					}
				case BSONARRAY_TYPE: 
					{
						BSONArrayObj innerArray = (BSONArrayObj)*origContent;
					   if (!include_all) {
							char* subselect = bson_subselect(sel, key.c_str());
							BSONArrayObj* innerSubArray = innerArray.select(subselect);
							result->add(key, *innerSubArray);
							delete innerSubArray;
						} else {
							result->add(key, innerArray);
						}
						break;
					}
				case INT_TYPE: 
					{
						int val = *origContent;
						result->add(key, val);
						break;
					}
				case LONG_TYPE:
					{
						long val = *origContent;
						result->add(key, val);
						break;
					}
				case DOUBLE_TYPE:
					{
						double val = *origContent;
						result->add(key, val);
						break;
					}
				case STRING_TYPE:
					{
						std::string val = *origContent;
						result->add(key, val);
						break;
					}
			}
		}
	}
	return result;
}

void BSONObj::fillContent(t_keytype kkey, BSONTYPE ttype, void* vval) {
	std::map<t_keytype, BSONContent* >::iterator i = _elements.find(kkey);
	if (i != _elements.end()) {
		// Removes the previous element
		BSONContent* current = i->second;
		delete current;
		_elements.erase(i);
	}
	BSONContent* content = new BSONContent(); 
	content->setType(ttype); 
	content->_element = vval; 
	_elements.insert(pair<t_keytype, BSONContent* >(kkey, content));
}
