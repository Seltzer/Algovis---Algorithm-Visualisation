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
CxxTest::StaticSuiteDescription suiteDescription_AlgoVisTestSuite( "d:/Project/branches/datasource/datasource-wrappers/algovis c++ unit-tests//Tests/AlgoVisTestSuite.h", 9, "AlgoVisTestSuite", suite_AlgoVisTestSuite, Tests_AlgoVisTestSuite );

static class TestDescription_AlgoVisTestSuite_testHack : public CxxTest::RealTestDescription {
public:
 TestDescription_AlgoVisTestSuite_testHack() : CxxTest::RealTestDescription( Tests_AlgoVisTestSuite, suiteDescription_AlgoVisTestSuite, 38, "testHack" ) {}
 void runTest() { suite_AlgoVisTestSuite.testHack(); }
} testDescription_AlgoVisTestSuite_testHack;

#include <cxxtest/Root.cpp>
