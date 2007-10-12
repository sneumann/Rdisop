#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <unistd.h>
#include <iostream>

#include <ims/utils/stopwatch.h>

/* This actually tests Stopwatch AND ProcessStopwatch */

class StopwatchTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(StopwatchTest);
	CPPUNIT_TEST(testStopwatch);
	CPPUNIT_TEST(testProcessStopwatch);
	CPPUNIT_TEST_SUITE_END();

public:
	void testStopwatch();
	void testProcessStopwatch();
	void eatCPU();
};

CPPUNIT_TEST_SUITE_REGISTRATION( StopwatchTest );

/* Just uses the CPU for some time */
void StopwatchTest::eatCPU() {
	for (int j = 0; j < 10; ++j) {
		std::vector<int> v;
		for (int i = 0; i < 100000; ++i) {
			v.push_back(i);
		}
	}
}


void StopwatchTest::testStopwatch() {
	ims::Stopwatch stopwatch;
	stopwatch.start();
	sleep(1);
	double elapsed = stopwatch.elapsed();
	CPPUNIT_ASSERT( elapsed >= 0.9 );
	// this could fail if the system is under heavy load
	CPPUNIT_ASSERT( elapsed < 10.0 );
}

void StopwatchTest::testProcessStopwatch() {
	ims::ProcessStopwatch pstopwatch;
	pstopwatch.start();
	eatCPU();
	double pelapsed = pstopwatch.elapsed();
	CPPUNIT_ASSERT( pelapsed >= 0.0 );
	// this check should succeed on all current CPUs
	CPPUNIT_ASSERT( pelapsed < 10.0 );
}
