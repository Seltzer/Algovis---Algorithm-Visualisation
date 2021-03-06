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
#include "d:/Project/branches/datasource/datasource-wrappers/algovis c++ unit-tests//Tests/AlgoVisTestSuite.h"

static AlgoVisTestSuite suite_AlgoVisTestSuite;

static CxxTest::List Tests_AlgoVisTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_AlgoVisTestSuite( "d:/Project/branches/datasource/datasource-wrappers/algovis c++ unit-tests//Tests/AlgoVisTestSuite.h", 18, "AlgoVisTestSuite", suite_AlgoVisTestSuite, Tests_AlgoVisTestSuite );

static class TestDescription_AlgoVisTestSuite_testIntWrapper : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testIntWrapper() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 46, "testIntWrapper" ) {}
 void runTest() { suite_AlgoVisTestSuite.testIntWrapper(); }
} testDescription_AlgoVisTestSuite_testIntWrapper;

static class TestDescription_AlgoVisTestSuite_testFloatWrapper : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testFloatWrapper() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 54, "testFloatWrapper" ) {}
 void runTest() { suite_AlgoVisTestSuite.testFloatWrapper(); }
} testDescription_AlgoVisTestSuite_testFloatWrapper;

static class TestDescription_AlgoVisTestSuite_testDoubleWrapper : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testDoubleWrapper() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 59, "testDoubleWrapper" ) {}
 void runTest() { suite_AlgoVisTestSuite.testDoubleWrapper(); }
} testDescription_AlgoVisTestSuite_testDoubleWrapper;

static class TestDescription_AlgoVisTestSuite_testCharWrapper : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testCharWrapper() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 64, "testCharWrapper" ) {}
 void runTest() { suite_AlgoVisTestSuite.testCharWrapper(); }
} testDescription_AlgoVisTestSuite_testCharWrapper;

static class TestDescription_AlgoVisTestSuite_testVectorWrapper : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testVectorWrapper() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 69, "testVectorWrapper" ) {}
 void runTest() { suite_AlgoVisTestSuite.testVectorWrapper(); }
} testDescription_AlgoVisTestSuite_testVectorWrapper;

static class TestDescription_AlgoVisTestSuite_testAlgoMatrix : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testAlgoMatrix() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 76, "testAlgoMatrix" ) {}
 void runTest() { suite_AlgoVisTestSuite.testAlgoMatrix(); }
} testDescription_AlgoVisTestSuite_testAlgoMatrix;

#include <cxxtest/Root.cpp>
