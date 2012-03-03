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

#include "connectionmanager.h"

#include "connection.h"

#ifdef WINDOWS
	#include <winsock.h>
#endif
using namespace djondb;

std::map<std::string, ConnectionReference> ConnectionManager::_connections;
bool ConnectionManager::__initialized = false;

ConnectionManager::ConnectionManager()
{
}

ConnectionManager::~ConnectionManager()
{
#ifdef WINDOWS
	WSACleanup();
#endif
}

Connection* ConnectionManager::getConnection(std::string host) {
#ifdef WINDOWS
	if (!__initialized) {
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
		wVersionRequested = MAKEWORD(2, 2);

		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0) {
			/* Tell the user that we could not find a usable */
			/* Winsock DLL.                                  */
			printf("WSAStartup failed with error: %d\n", err);
		}
		__initialized = true;
	}
#endif
    std::map<std::string, ConnectionReference>::iterator i = _connections.find(host);
    if (i != _connections.end()) {
        ConnectionReference reference = i->second;
        reference._references++;
        if (reference._connection->isOpen()) {
            return reference._connection;
        } else {
            _connections.erase(i);
        };
    }

    Connection* conn = new Connection(host);
    ConnectionReference ref;
    ref._references = 1;
    ref._connection = conn;
    _connections.insert(pair<std::string, ConnectionReference>(host, ref));
    return conn;
}

void ConnectionManager::releaseConnection(Connection* conn) {
    std::map<std::string, ConnectionReference>::iterator i = _connections.find(conn->host());
    if (i != _connections.end()) {
        ConnectionReference reference = i->second;
        reference._references--;
        if (reference._references == 0) {
            reference._connection->internalClose();
            _connections.erase(i);
        };
    }
}
