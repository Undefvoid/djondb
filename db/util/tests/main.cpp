#include <iostream>
#include "util.h"
#include <string.h>
#include <stdlib.h>

#include <cpptest.h>


using namespace std;

class TestUtilSuite : public Test::Suite
{
public:
    TestUtilSuite() {
        TEST_ADD(TestUtilSuite::testDates)
        TEST_ADD(TestUtilSuite::testTimes)
        TEST_ADD(TestUtilSuite::testStrings)
        TEST_ADD(TestUtilSuite::testUUID);
    }

private:
    void testDates()
    {
        DateTime dt = DateTime::today(true);

        DateTime dt2 = dt.addDays(1);

        TEST_ASSERT((dt.getDay() + 1) == dt2.getDay());

        // testing day of the week
        dt = DateTime(2011, 9, 12);
        TEST_ASSERT(dt.dayOfTheWeek() == 1);

        // testing day of the week "complex", 29 of febr
        dt = DateTime(2008, 2, 29);
        TEST_ASSERT(dt.dayOfTheWeek() == 5);

        // diff
        dt = DateTime(2011, 2, 10);
        dt2 = DateTime(2011, 3, 1);
        int diff = dt2.daysTo(dt);
        TEST_ASSERT(diff == 19);
    }

    void testTimes()
    {
        // Testing diff
        DTime t(15, 40, 0);
        DTime t2(16, 20, 0);
        DTime res = t2 - t;

        TEST_ASSERT((res.hour() == 0) && (res.minutes() == 40) && (res.seconds() == 0));

        res = t2 - 200;
        TEST_ASSERT((res.hour() == 16) && (res.minutes() == 16) && (res.seconds() == 40));
        // teting add
    }

    void testStrings()
    {
        // Copy chars
        char* test = "Hello world!";
        char* res = strcpy(test, strlen(test));

        TEST_ASSERT(strcmp(test, res) == 0);
        TEST_ASSERT(test != res);
        free(res);

        //copy string
        std::string s = "Hello world!";
        res = strcpy(s);
        TEST_ASSERT(s.compare(res) == 0);

        // ends with
        bool com = endsWith("test.tex", ".tex");
        TEST_ASSERT(com);
        com = endsWith("test.ss", "test");
        TEST_ASSERT(!com);

        // tokenizer
        std::vector<std::string*>* token = tokenizer("test,other,and 1 more", ",");
        TEST_ASSERT(token->size() == 3);
        TEST_ASSERT(token->at(0)->compare("test") == 0);
        TEST_ASSERT(token->at(1)->compare("other") == 0);
        TEST_ASSERT(token->at(2)->compare("and 1 more") == 0);
        delete(token);

        // format
        std::string sformat = format("test %d %s %5.4f", 10, "Hello World!", 3.14159);
        TEST_ASSERT(sformat.compare("test 10 Hello World! 3.1416") == 0);

        // toString
        std::string s2 = toString(10.1);
        TEST_ASSERT(s2.compare("10.1")== 0);

        std::string s3 = toString(3.14159, 2);
        TEST_ASSERT(s3.compare("3.14")== 0);

        std::string s4 = toString(3);
        TEST_ASSERT(s4.compare("3")== 0);

        // split
        std::vector<std::string> sp = split("test,other,and 1 more", ",");
        TEST_ASSERT(sp.size() == 3);
        TEST_ASSERT(sp.at(0).compare("test") == 0);
        TEST_ASSERT(sp.at(1).compare("other") == 0);
        TEST_ASSERT(sp.at(2).compare("and 1 more") == 0);

        //Count char
        long c = countChar("testing.this.component.!", '.');
        TEST_ASSERT(c == 3);
    }

    void testUUID()
    {
        std::string* u = uuid();
        TEST_ASSERT(u != NULL);
        delete u;
    }

};
//// Tests unconditional fail TEST_ASSERTs
////
//class FailTestSuite : public Test::Suite
//{
//public:
//    FailTestSuite()
//    {
//        TEST_ADD(FailTestSuite::success)
//        TEST_ADD(FailTestSuite::always_fail)
//
//    }
//
//private:
//    void success() {}
//
//    void always_fail()
//    {
//        // This will always fail
//        //
//        TEST_FAIL("unconditional fail");
//    }
//};
//
//// Tests compare TEST_ASSERTs
////
//class CompareTestSuite : public Test::Suite
//{
//public:
//    CompareTestSuite()
//    {
//        TEST_ADD(CompareTestSuite::success)
//        TEST_ADD(CompareTestSuite::compare)
//        TEST_ADD(CompareTestSuite::delta_compare)
//    }
//
//private:
//    void success() {}
//
//    void compare()
//    {
//        // Will succeed since the expression evaluates to true
//        //
//        TEST_ASSERT(1 + 1 == 2)
//
//        // Will fail since the expression evaluates to false
//        //
//        TEST_ASSERT(0 == 1);
//    }
//
//    void delta_compare()
//    {
//        // Will succeed since the expression evaluates to true
//        //
//        TEST_ASSERT_DELTA(0.5, 0.7, 0.3);
//
//        // Will fail since the expression evaluates to false
//        //
//        TEST_ASSERT_DELTA(0.5, 0.7, 0.1);
//    }
//};
//
//// Tests throw TEST_ASSERTs
////
//class ThrowTestSuite : public Test::Suite
//{
//public:
//    ThrowTestSuite()
//    {
//        TEST_ADD(ThrowTestSuite::success)
//        TEST_ADD(ThrowTestSuite::test_throw)
//    }
//
//private:
//    void success() {}
//
//    void test_throw()
//    {
//        // Will fail since the none of the functions throws anything
//        //
//        TEST_THROWS_MSG(func(), int, "func() does not throw, expected int exception")
//        TEST_THROWS_MSG(func_no_throw(), int, "func_no_throw() does not throw, expected int exception")
//        TEST_THROWS_ANYTHING_MSG(func(), "func() does not throw, expected any exception")
//        TEST_THROWS_ANYTHING_MSG(func_no_throw(), "func_no_throw() does not throw, expected any exception")
//
//        // Will succeed since none of the functions throws anything
//        //
//        TEST_THROWS_NOTHING(func())
//        TEST_THROWS_NOTHING(func_no_throw())
//
//        // Will succeed since func_throw_int() throws an int
//        //
//        TEST_THROWS(func_throw_int(), int)
//        TEST_THROWS_ANYTHING(func_throw_int())
//
//        // Will fail since func_throw_int() throws an int (not a float)
//        //
//        TEST_THROWS_MSG(func_throw_int(), float, "func_throw_int() throws an int, expected a float exception")
//        TEST_THROWS_NOTHING_MSG(func_throw_int(), "func_throw_int() throws an int, expected no exception at all")
//    }
//
//    void func() {}
//    void func_no_throw() throw() {}
//    void func_throw_int() throw(int)
//    {
//        throw 13;
//    }
//};

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
        ts.add(auto_ptr<Test::Suite>(new TestUtilSuite));
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