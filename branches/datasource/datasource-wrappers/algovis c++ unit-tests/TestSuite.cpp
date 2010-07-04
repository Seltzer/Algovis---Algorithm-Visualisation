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
#include "d:/Programming/Project3/branches/datasource/datasource-wrappers/algovis c++ unit-tests//Tests/AlgoVisTestSuite.h"

static AlgoVisTestSuite suite_AlgoVisTestSuite;

static CxxTest::List Tests_AlgoVisTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_AlgoVisTestSuite( "d:/Programming/Project3/branches/datasource/datasource-wrappers/algovis c++ unit-tests//Tests/AlgoVisTestSuite.h", 10, "AlgoVisTestSuite", suite_AlgoVisTestSuite, Tests_AlgoVisTestSuite );

static class TestDescription_AlgoVisTestSuite_testIntWrapper : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testIntWrapper() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 33, "testIntWrapper" ) {}
 void runTest() { suite_AlgoVisTestSuite.testIntWrapper(); }
} testDescription_AlgoVisTestSuite_testIntWrapper;

static class TestDescription_AlgoVisTestSuite_testVectorBasics : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testVectorBasics() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 117, "testVectorBasics" ) {}
 void runTest() { suite_AlgoVisTestSuite.testVectorBasics(); }
} testDescription_AlgoVisTestSuite_testVectorBasics;

static class TestDescription_AlgoVisTestSuite_testAdvancedVector : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testAdvancedVector() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 161, "testAdvancedVector" ) {}
 void runTest() { suite_AlgoVisTestSuite.testAdvancedVector(); }
} testDescription_AlgoVisTestSuite_testAdvancedVector;

static class TestDescription_AlgoVisTestSuite_testFloatWrapper : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testFloatWrapper() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 199, "testFloatWrapper" ) {}
 void runTest() { suite_AlgoVisTestSuite.testFloatWrapper(); }
} testDescription_AlgoVisTestSuite_testFloatWrapper;

#include <cxxtest/Root.cpp>
