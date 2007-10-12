#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/peaklist.h>
#include <ims/massintensitypeak.h>

using namespace ims;

class PeakListTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE( PeakListTest );
		CPPUNIT_TEST( testPushBack );
		CPPUNIT_TEST( testIterator );
		CPPUNIT_TEST( testPropertyIterator );
		CPPUNIT_TEST( testConstPropertyIterator );
		
		CPPUNIT_TEST( testResize );
		CPPUNIT_TEST_SUITE_END();

		typedef double value_t;
		typedef MassIntensityPeak<value_t, value_t> peak_t;
		typedef PeakList<peak_t> peaklist_t;
	public:
		void testPushBack();
		void testIterator();
		void testPropertyIterator();
		void testConstPropertyIterator();
		void testResize();
};

CPPUNIT_TEST_SUITE_REGISTRATION(PeakListTest);

void PeakListTest::testPushBack() {
	peak_t peak(1.23, 4.56);
	peaklist_t list;
	CPPUNIT_ASSERT_EQUAL((size_t)0, list.size());
	CPPUNIT_ASSERT(list.empty());
	
	list.push_back(peak);
	CPPUNIT_ASSERT_EQUAL((size_t)1, list.size());
	CPPUNIT_ASSERT(list[0] == peak);
	CPPUNIT_ASSERT(not list.empty());
}

void PeakListTest::testIterator() {
	peak_t peak0(1.23, 1.23);
	peak_t peak1(4.56, 4.56);
	peak_t peak2(7.89, 7.89);

	peaklist_t list;
	list.push_back(peak0);
	list.push_back(peak1);
	list.push_back(peak2);
	CPPUNIT_ASSERT(list.front() == peak0);
	CPPUNIT_ASSERT(list.back() == peak2);
	CPPUNIT_ASSERT(list.begin() == list.end()-3);
}

void PeakListTest::testPropertyIterator() {
	value_t value = 1;
	peak_t peak0(value, value+1);
	peak_t peak1(peak0);
	peak_t peak2 = peak0;
	
	peaklist_t peaklist;
	peaklist.push_back(peak0);
	peaklist.push_back(peak1);
	peaklist.push_back(peak2);
	
	// check whether intensity property can be correctly extracted from the corresponding peaklist
	peaklist_t::property_iterator<peak_t::IntensityGetter>::type 
		intensityIt = peaklist.begin<peak_t::IntensityGetter>(),
		intensityEnd = peaklist.end<peak_t::IntensityGetter>();
	for ( ; intensityIt != intensityEnd; ++intensityIt) {
		CPPUNIT_ASSERT(*intensityIt == value+1);
	}

	// check that both peak iterator and different property iterators 
	// correspond to each other
	peaklist_t::iterator peakIt = peaklist.begin();
	peaklist_t::property_iterator<peak_t::MassGetter>::type 
		massIt = peaklist.begin<peak_t::MassGetter>();
	peaklist_t::property_iterator<peak_t::IntensityGetter>::type 
		intensityIt2 = peaklist.begin<peak_t::IntensityGetter>();
	
	for ( ; peakIt != peaklist.end(); ++peakIt, ++massIt, ++intensityIt2) {
		CPPUNIT_ASSERT(peakIt->getMass() == *massIt);
		CPPUNIT_ASSERT(peakIt->getIntensity() == *intensityIt2);
	}	
}

void PeakListTest::testConstPropertyIterator() {
	value_t value = 1;
	peak_t peak0(value, value+1);
	peak_t peak1(peak0);
	peak_t peak2 = peak0;
	
	peaklist_t peaklist;
	peaklist.push_back(peak0);
	peaklist.push_back(peak1);
	peaklist.push_back(peak2);
	
	// check whether intensity property can be correctly extracted from the corresponding peaklist
	peaklist_t::const_property_iterator<peak_t::IntensityGetter>::type
		intensityIt = peaklist.begin<peak_t::IntensityGetter>(),
		intensityEnd = peaklist.end<peak_t::IntensityGetter>();
	for ( ; intensityIt != intensityEnd; ++intensityIt) {
		CPPUNIT_ASSERT(*intensityIt == value+1);
	}

	// check that both peak iterator and different property iterators
	// correspond to each other
	peaklist_t::const_iterator peakIt = peaklist.begin();
	peaklist_t::const_property_iterator<peak_t::MassGetter>::type
		massIt = peaklist.begin<peak_t::MassGetter>();
	peaklist_t::const_property_iterator<peak_t::IntensityGetter>::type
		intensityIt2 = peaklist.begin<peak_t::IntensityGetter>();
	
	for ( ; peakIt != peaklist.end(); ++peakIt, ++massIt, ++intensityIt2) {
		CPPUNIT_ASSERT(peakIt->getMass() == *massIt);
		CPPUNIT_ASSERT(peakIt->getIntensity() == *intensityIt2);
	}
}


void PeakListTest::testResize() {
	peak_t peak(1.23, 4.56);
	peaklist_t list;
	list.push_back(peak);
	list.resize(23);
	CPPUNIT_ASSERT_EQUAL((size_t)23, list.size());
	list.resize(2);
	CPPUNIT_ASSERT_EQUAL((size_t)2, list.size());
}
