// =====================================================================================
//  Filename:  settings.cpp
// 
//  Description:  Implementation of Settings reader.
// 
//  Version:  1.0
//  Created:  03/11/2012 12:39:40 PM
//  Revision:  none
//  Compiler:  gcc
// 
//  Author:  YOUR NAME (), 
// 
// License:
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
// =====================================================================================


#include "settings.h"
#include <map>
#include <stdlib.h>
#include "fileutil.h"
#include "stringfunctions.h" 

std::map<std::string, std::string> __settingsValues;
bool __settingsLoaded;

void readSettings() {
#ifndef WINDOWS
	char* ccont = readFile("/etc/djondb.conf");
#else
	char* ccont = readFile("C:\\workspace\\personal\\djondb\\db\\windows\\djondb.conf");
#endif
   std::string content = std::string(ccont);

	std::vector<std::string> lines = splitLines(content);
	for (std::vector<std::string>::const_iterator i = lines.begin(); i != lines.end(); i++) {
		std::vector<std::string> vals = split(*i, "=");
		std::string key = vals[0];
		std::string value = vals[1];

	   __settingsValues.insert(std::pair<std::string, std::string>(key, value));
	}
	__settingsLoaded = true;
	free(ccont);
}

std::string getSetting(std::string key) {
	if (__settingsLoaded == false) {
		readSettings();
	}

	std::map<std::string, std::string>::iterator it = __settingsValues.find(key);

	if (it != __settingsValues.end()) {
		return it->second;
	} else {
		return "";
	}
}
