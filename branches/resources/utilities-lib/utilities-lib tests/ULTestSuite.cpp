/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "d:/Project/branches/resources/utilities-lib/utilities-lib tests//ULTestSuite.h"

static UtilitiesLibTestSuite suite_UtilitiesLibTestSuite;

static CxxTest::List Tests_UtilitiesLibTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UtilitiesLibTestSuite( "d:/Project/branches/resources/utilities-lib/utilities-lib tests//ULTestSuite.h", 16, "UtilitiesLibTestSuite", suite_UtilitiesLibTestSuite, Tests_UtilitiesLibTestSuite );

static class TestDescription_UtilitiesLibTestSuite_testTemplateMP : public CxxTest::RealTestDescription {
public:
 TestDescription_UtilitiesLibTestSuite_testTemplateMP() : CxxTest::RealTestDescription( Tests_UtilitiesLibTestSuite, suiteDescription_UtilitiesLibTestSuite, 30, "testTemplateMP" ) {}
 void runTest() { suite_UtilitiesLibTestSuite.testTemplateMP(); }
} testDescription_UtilitiesLibTestSuite_testTemplateMP;

static class TestDescription_UtilitiesLibTestSuite_testStrings : public CxxTest::RealTestDescription {
public:
 TestDescription_UtilitiesLibTestSuite_testStrings() : CxxTest::RealTestDescription( Tests_UtilitiesLibTestSuite, suiteDescription_UtilitiesLibTestSuite, 36, "testStrings" ) {}
 void runTest() { suite_UtilitiesLibTestSuite.testStrings(); }
} testDescription_UtilitiesLibTestSuite_testStrings;

static class TestDescription_UtilitiesLibTestSuite_testEnum : public CxxTest::RealTestDescription {
public:
 TestDescription_UtilitiesLibTestSuite_testEnum() : CxxTest::RealTestDescription( Tests_UtilitiesLibTestSuite, suiteDescription_UtilitiesLibTestSuite, 42, "testEnum" ) {}
 void runTest() { suite_UtilitiesLibTestSuite.testEnum(); }
} testDescription_UtilitiesLibTestSuite_testEnum;

static class TestDescription_UtilitiesLibTestSuite_testExceptions : public CxxTest::RealTestDescription {
public:
 TestDescription_UtilitiesLibTestSuite_testExceptions() : CxxTest::RealTestDescription( Tests_UtilitiesLibTestSuite, suiteDescription_UtilitiesLibTestSuite, 54, "testExceptions" ) {}
 void runTest() { suite_UtilitiesLibTestSuite.testExceptions(); }
} testDescription_UtilitiesLibTestSuite_testExceptions;

#include <cxxtest/Root.cpp>
