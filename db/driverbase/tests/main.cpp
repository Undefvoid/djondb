// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com) // created:
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

#include <iostream>
#include "djondb_client.h"

#include "fileoutputstream.h"
#include "fileinputstream.h"
#include "config.h"
#include "util.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cpptest.h> 
#include <limits.h> 

using namespace std; 
using namespace djondb;

bool __running;
void *startSocketListener(void* arg);


void *startSocketListener(void* arg) {
	/*
		NetworkInputStream* nis = (NetworkInputStream*)arg;

		BSONInputStream* bis = new BSONInputStream(nis);
		while (__running) {
		std::auto_ptr<BSONObj> resObj(bis->readBSON());
		TEST_ASSERT(resObj.get() != NULL);
		TEST_ASSERT(resObj->has("_id"));
		}
		*/
}

class TestDriverBaseSuite: public Test::Suite {
	private:
		char* _host;
		int _port;

	public:

		TestDriverBaseSuite() {
			_host = "localhost";
			_port = 1243;
			/*
			TEST_ADD(TestDriverBaseSuite::testFinds);
			TEST_ADD(TestDriverBaseSuite::testInsert);
			TEST_ADD(TestDriverBaseSuite::testInsertComplex);
			TEST_ADD(TestDriverBaseSuite::testUpdate);
			TEST_ADD(TestDriverBaseSuite::testUpdateValidations);
			TEST_ADD(TestDriverBaseSuite::testRemove);
			TEST_ADD(TestDriverBaseSuite::testFindByFilter);
			TEST_ADD(TestDriverBaseSuite::testDbsNamespaces);

			TEST_ADD(TestDriverBaseSuite::testDropNamespace);
			//TEST_ADD(TestDriverBaseSuite::testTransactions);
			TEST_ADD(TestDriverBaseSuite::testTransactions);

			// Leave this test at the end, because it will shutdow the server
			TEST_ADD(TestDriverBaseSuite::testConnection);
		}


		void testConnection() {
			cout << "\ntestConnection\n" << endl;

			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			try {
				conn->insert("db1", "ns1", "{ name: 'Test'  }");
				TEST_FAIL("If the client is not connected then an exception should be thrown");
			} catch (DjondbException) {
				// Expected behavior
			}

			// test connection and then shutdown the server
			if (conn->open()) {
				conn->shutdown();
				try {
					conn->insert("db1", "ns1", "{ name: 'Test'  }");
				} catch (DjondbException) {
					// Expected behavior
				}
			} else {
				TEST_FAIL("Cannot establish a connection with the server");
			}
		}

		void testDbsNamespaces() {
			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			if (!conn->open()) {
				cout << "\nCannot connect to localhost" << endl;
				exit(0);
			}

			std::string bson = "{ name: 'Test'}";
			conn->insert("db1", "ns1", bson);
			conn->insert("db2", "ns1", bson);
			conn->insert("db3", "ns1", bson);

			std::vector<std::string>* dbs = conn->dbs();

			TEST_ASSERT(dbs->size() >= 3);

			delete dbs;
		}

		void testDropNamespace() {
			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			if (!conn->open()) {
				cout << "\nCannot connect to localhost" << endl;
				exit(0);
			}

			conn->insert("db", "testdrop.namespace", "{ name: 'Test' }");

			bool result = conn->dropNamespace("db", "testdrop.namespace");

			TEST_ASSERT(result);

			BSONArrayObj* testresult = conn->find("db", "testdrop.namespace", "*", std::string(""));

			TEST_ASSERT(testresult->length() == 0);
			delete testresult;
		}

		void testInsertComplex() {
			cout << "\nTesting complex" << endl;
			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			if (!conn->open()) {
				cout << "\nCould not connect to " << _host << endl;
				exit(0);
			}

			conn->dropNamespace("test", "ns");

			BSONObj obj;
			std::string* id = uuid();
			obj.add("_id", const_cast<char*>(id->c_str()));
			obj.add("name", "John");
			BSONObj inner;
			inner.add("innername", "Test");
			obj.add("inner", inner);

			conn->insert("test", "ns", obj);

			BSONArrayObj* res = conn->find("test", "ns", "*", "$'_id' == '" + *id + "'");
			TEST_ASSERT(res->length() == 1);
			BSONObj* bres = *res->begin();
			TEST_ASSERT(bres->has("inner"));
			BSONObj* innerres = bres->getBSON("inner");
			TEST_ASSERT(innerres != NULL);
			TEST_ASSERT(innerres->has("innername"));
			TEST_ASSERT(((std::string)"Test").compare(innerres->getString("innername")) == 0);

			// testing arrays
			cout << "testInsertComplex: Testing arrays" << endl;
			std::string* id2 = uuid();
			conn->insert("test", "ns", "{ '_id': '" + *id2 + "', 'array': [ { 'x': 'test', 'y': 3},  { 'x': 'test2', 'y': 4}]  }");

			BSONArrayObj* res2 = conn->find("test", "ns", "*", "$'_id' == '" + *id2 + "'");
			TEST_ASSERT(res2->length() == 1);
			BSONObj* o2 = *res2->begin();
			TEST_ASSERT(o2 != NULL);

			TEST_ASSERT(o2->has("array"));

			delete res;
			delete res2;

			// testing a customer
			conn->dropNamespace("db", "testcustomer");
			BSONObj* customer = BSONParser::parse("{ 'name': 'Martin', 'lastName': 'Scor', 'finantial': { 'salary': 150000, 'rent': 10000} }");
			conn->insert("db", "testcustomer", *customer);
			delete customer;

			res2 = conn->find("db", "testcustomer", "*", "$'name' == 'Martin'");
			TEST_ASSERT(res2->length() == 1);
			if (res2->length() == 1) {
				BSONObj* objCustomer = *res2->begin();
				int d = *objCustomer->getXpath("finantial.salary");
				TEST_ASSERT(d == 150000);
			}
			delete res2;
		}

		void testInsert() {
			int inserts = 1;

			Logger* log = getLogger(NULL);

			cout << "\nStarting " << endl;

			log->startTimeRecord();
			__running = true;

			DjondbConnection* conn = DjondbConnectionManager::getConnection(std::string(_host));

			if (!conn->open()) {
				cout << "\nCould not connect to " << _host << endl;
				exit(0);
			}
			std::vector<std::string> ids;
			for (int x = 0; x < inserts; x++) {

				BSONObj obj;
				std::auto_ptr<std::string> guid(uuid());
				obj.add("_id", const_cast<char*>(guid->c_str()));
				int test = rand() % 10;
				if (test > 0) {
					ids.push_back(*guid.get());
				}
				//        obj->add("name", "John");
				char* temp = (char*)malloc(2000);
				memset(temp, 0, 2000);
				memset(temp, 'a', 1999);
				int len = strlen(temp);
				obj.add("content", temp);
				free(temp);

				conn->insert("db", "driverbase.test", obj);

				if ((inserts > 9) && (x % (inserts / 10)) == 0) {
					cout << "\n" << x << " Records sent" << endl;
				}
			}
			FileOutputStream* fosIds = new FileOutputStream("results.txt", "wb");
			fosIds->writeInt(ids.size());
			for (std::vector<std::string>::iterator i2 = ids.begin(); i2!= ids.end(); i2++) {
				std::string s = *i2;
				fosIds->writeString(s);
			}
			fosIds->close();
			cout << "\nall sent" << endl;

			log->stopTimeRecord();

			DTime rec = log->recordedTime();

			int secs = rec.totalSecs();
			cout << "\ninserts " << inserts << ", time: " << rec.toChar() << endl;

			if (secs > 0) {
				cout << "\nThroughput: " << (inserts / secs) << " ops." << endl;
			}
			cout << "\n------------------------------------------------------------" << endl;
			cout << "\nReady to close the connection" << endl;
			//getchar();
			__running = false;

			//    cout << "Closing the connection" << endl;
			//    conn->close();
			//
			//    delete conn;
		}

		void testFinds() {
			int maxfinds = 1;
			Logger* log = getLogger(NULL);

			cout << "\nStarting testFinds" << endl;

			__running = true;
			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			if (!conn->open()) {
				cout << "\nCannot connect to localhost" << endl;
				exit(0);
			}

			BSONObj test;
			std::string* guid = uuid();
			test.add("_id", const_cast<char*>(guid->c_str()));
			test.add("int", 1);
			test.add("long", (__int64) 10L);
			test.add("longmax",(__int64) LONG_MAX);
			test.add("char", "testing");

			conn->insert("db", "driver.test", test);

			log->debug("Data inserted");

			BSONObj* objResult = conn->findByKey("db", "driver.test", "*", *guid);

			TEST_ASSERT(objResult != NULL);
			TEST_ASSERT(objResult->has("int"));
			TEST_ASSERT(objResult->getInt("int") == 1);
			TEST_ASSERT(objResult->has("long"));
			TEST_ASSERT(objResult->getLong("long") == 10);
			TEST_ASSERT(objResult->has("longmax"));
			TEST_ASSERT(objResult->getLong("longmax") == LONG_MAX);
			TEST_ASSERT(objResult->has("char"));
			TEST_ASSERT(objResult->getString("char").compare("testing") == 0);

			__running = false;

		}

		void testFindByFilter() {
			// Insert record to search for

			BSONObj* obj = BSONParser::parse("{'name': 'Test', 'inner': { 'x': 1 }}");

			//delete id;

			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			if (!conn->open()) {
				cout << "\nCannot connect to localhost" << endl;
				exit(0);
			}


			// Drops the current namespace to start from scratch
			conn->dropNamespace("db", "test.filter2");

			conn->insert("db", "test.filter2", *obj);

			// doing search
			//

			cout << "\nTestbyfilter" << endl;
			std::string filter = "";
			BSONArrayObj* result = conn->find("db", "test.filter2", "*", filter);			
			TEST_ASSERT(result->length() > 0);
			filter = "$'name' == 'Test'";
			delete result;
			result = conn->find("db", "test.filter2", filter);			
			TEST_ASSERT(result->length() > 0);

			BSONObj* objR = *result->begin();
			TEST_ASSERT(objR != NULL);
			TEST_ASSERT(objR->has("name"));
			TEST_ASSERT(objR->getString("name").compare("Test") == 0);

			char* temp = objR->toChar();
			cout << "\nobj: " << temp << endl;

			result = conn->find("db", "test.filter2", "*", "$'name' == 'Test'");
			TEST_ASSERT(result->length() == 1);

			result = conn->find("db", "test.filter2", "$'inner.x' == 1");
			TEST_ASSERT(result->length() == 1);

			result = conn->find("db", "test.filter2", "*", "$'inner.x' > 0");
			TEST_ASSERT(result->length() == 1);

			result = conn->find("db", "test.filter2", "$'inner.x' > 1");
			TEST_ASSERT(result->length() == 0);

			delete objR;
			delete result;

		}

		void testUpdate() {
			int maxupdates = 1;
			Logger* log = getLogger(NULL);

			cout << "\nStarting " << endl;

			log->startTimeRecord();
			__running = true;

			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			if (!conn->open()) {
				cout << "\nCannot connect to localhost" << endl;
				exit(0);
			}
			FileInputStream* fisIds = new FileInputStream("results.txt", "rb");
			int x = 0;
			int count = fisIds->readInt();
			if ((maxupdates > -1) && (count > maxupdates)) {
				count = maxupdates;
			}
			cout << "\nRecords to update: " << count << endl;

			std::vector<std::string> idsUpdated;
			for (x =0; x < count; x++) {
				std::auto_ptr<std::string> guid(fisIds->readString());

				BSONObj obj;
				obj.add("_id", const_cast<char*>(guid->c_str()));

				idsUpdated.push_back(*guid.get());
				char* temp = (char*)malloc(100);
				memset(temp, 0, 100);
				memset(temp, 'b', 99);
				int len = strlen(temp);
				obj.add("content", temp);
				free(temp);

				conn->update("db", "driverbase.test", obj);

				if ((count > 9) && (x % (count / 10)) == 0) {
					cout << "\n" << x << " Records received" << endl;
				}
			}

			log->stopTimeRecord();

			cout << "\nExecuting a verification" << endl;

			for (std::vector<std::string>::iterator i = idsUpdated.begin(); i != idsUpdated.end(); i++) {
				std::string guid = *i;

				std::auto_ptr<BSONObj> resObj(conn->findByKey("db", "driverbase.test", "*", guid));

				TEST_ASSERT(resObj.get() != NULL);
				TEST_ASSERT(resObj->has("_id"));
				TEST_ASSERT(resObj->has("content"));

				char* temp = (char*)malloc(100);
				memset(temp, 0, 100);
				memset(temp, 'b', 99);
				TEST_ASSERT(resObj->getString("content").compare(temp) == 0);
				free(temp);
			}
			DTime rec = log->recordedTime();

			int secs = rec.totalSecs();
			cout<< "finds " << count << ", time: " << rec.toChar() << endl;

			if (secs > 0) {
				cout << "\nThroughput: " << (count / secs) << " ops." << endl;
			}

			cout << "\n------------------------------------------------------------" << endl;
			cout << "\nReady to close the connection" << endl;
			//getchar();
			__running = false;

			//    cout << "Closing the connection" << endl;
			//    conn->close();
			//
			//    delete conn;
		}

		void testUpdateValidations() {
			cout << "\ntestUpdateValidations\n" << endl;

			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			if (!conn->open()) {
				cout << "\nCannot connect to localhost" << endl;
				exit(0);
			}

			try {
				BSONObj obj;
				obj.add("test", "test");
				conn->update("mydb", "updatens", obj);
				TEST_FAIL("The update should validate that an _id and _revision are required fields");
			} catch (DjondbException e) {
				//Expected behavior
			}
		}

		void testRemove() {
			cout << "\ntestRemove\n" << endl;

			DjondbConnection* conn = DjondbConnectionManager::getConnection("localhost");

			if (!conn->open()) {
				cout << "\nCannot connect to localhost" << endl;
				exit(0);
			}

			conn->dropNamespace("testdb", "deletens");

			BSONObj obj;
			std::string* id = uuid();
			std::string* revision = uuid();
			obj.add("_id", const_cast<char*>(id->c_str())); 
			obj.add("_revision", const_cast<char*>(revision->c_str())); 

			conn->insert("testdb", "deletens", obj);

			conn->remove("testdb", "deletens", *id, *revision);

			BSONObj* res = conn->findByKey("testdb", "deletend", *id);
			TEST_ASSERT(res == NULL);

			DjondbConnectionManager::releaseConnection(conn);

			delete id;
			delete revision;
		}


		void testTransactions() {
			cout << "\ntestTransactions()\n" << endl;

			DjondbConnection* connection = DjondbConnectionManager::getConnection("localhost");

			if (connection->open()) {
				// Out of the transaction
				//
				connection->dropNamespace("testdb", "tx");
				BSONObj ontx;
				std::auto_ptr<std::string> idx(uuid());
				ontx.add("_id", const_cast<char*>(idx->c_str()));
				ontx.add("name", "TestOutTx");
				connection->insert("testdb", "tx", ontx);


				// Now the transaction is started
				const char* trans = connection->beginTransaction();

				BSONObj o;
				std::auto_ptr<std::string> id(uuid());
				o.add("_id", const_cast<char*>(id->c_str()));
				o.add("name", "John");
				o.add("lastName", "Crossley");

				connection->insert("testdb", "tx", o);

				BSONObj* res = connection->findByKey("testdb", "tx", id->c_str());

				TEST_ASSERT(res != NULL);
				TEST_ASSERT(res->has("name"));
				if (res->has("name")) TEST_ASSERT(res->getString("name").compare("John") == 0);

				// Test records out of the transaction
				BSONArrayObj* array1 = connection->find("testdb", "tx", "*", "");
				TEST_ASSERT(array1->length() == 2);

				connection->commitTransaction();

				BSONObj* res2 = connection->findByKey("testdb", "tx", id->c_str());

				TEST_ASSERT(res2 != NULL);
				TEST_ASSERT(res2->has("name"));
				if (res2->has("name")) TEST_ASSERT(res2->getString("name").compare("John") == 0);

			}
		}

};


enum OutputType
{
	Compiler,
	Html,
	TextTerse,
	TextVerbose
};

	static void
usage()
{
	cout << "usage: mytest [MODE]\n"
		<< "where MODE may be one of:\n"
		<< "  --compiler\n"
		<< "  --html\n"
		<< "  --text-terse (default)\n"
		<< "  --text-verbose\n";
	exit(0);
}

	static auto_ptr<Test::Output>
cmdline(int argc, char* argv[])
{
	if (argc > 2)
		usage(); // will not return

	Test::Output* output = 0;

	if (argc == 1)
		output = new Test::TextOutput(Test::TextOutput::Verbose);
	else
	{
		const char* arg = argv[1];
		if (strcmp(arg, "--compiler") == 0)
			output = new Test::CompilerOutput;
		else if (strcmp(arg, "--html") == 0)
			output =  new Test::HtmlOutput;
		else if (strcmp(arg, "--text-terse") == 0)
			output = new Test::TextOutput(Test::TextOutput::Terse);
		else if (strcmp(arg, "--text-verbose") == 0)
			output = new Test::TextOutput(Test::TextOutput::Verbose);
		else
		{
			cout << "invalid commandline argument: " << arg << endl;
			usage(); // will not return
		}
	}

	return auto_ptr<Test::Output>(output);
}

// Main test program
//
int main(int argc, char* argv[])
{
	try
	{
		// Demonstrates the ability to use multiple test suites
		//
		Test::Suite ts;
		ts.add(auto_ptr<Test::Suite>(new TestDriverBaseSuite));
		//        ts.add(auto_ptr<Test::Suite>(new CompareTestSuite));
		//        ts.add(auto_ptr<Test::Suite>(new ThrowTestSuite));

		// Run the tests
		//
		auto_ptr<Test::Output> output(cmdline(argc, argv));
		ts.run(*output, true);

		Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
		if (html)
			html->generate(cout, true, "MyTest");
	}
	catch (...)
	{
		cout << "unexpected exception encountered\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
