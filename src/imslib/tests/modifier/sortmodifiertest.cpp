#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/peaklist.h>
#include <ims/massintensitypeak.h>
#include <ims/modifier/sortmodifier.h>

class SortModifierTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(SortModifierTest);
	CPPUNIT_TEST( testModify );
	CPPUNIT_TEST( testClone );
	CPPUNIT_TEST_SUITE_END();

	public:
		typedef ims::MassIntensityPeak<double,double> peak_t;
		typedef ims::PeakList<peak_t> peaklist_t;

		void setUp();
		void tearDown();
		void testModify();
		void testClone();
		void assertHelp();

	private:
		peaklist_t peaklist;
};


CPPUNIT_TEST_SUITE_REGISTRATION(SortModifierTest);


void SortModifierTest::setUp() {
	peaklist.push_back(peak_t(5.0, 3.2));
	peaklist.push_back(peak_t(4.0, 8.2));
	peaklist.push_back(peak_t(2.0, 103.2));
	peaklist.push_back(peak_t(8.0, 2003.2));
	peaklist.push_back(peak_t(4.0, 5000.0));
}


void SortModifierTest::tearDown() {
	peaklist.clear();
}

void SortModifierTest::assertHelp() {
	CPPUNIT_ASSERT_EQUAL((size_t)5, peaklist.size());
	CPPUNIT_ASSERT_EQUAL(2.0, peaklist[0].getMass());
	CPPUNIT_ASSERT_EQUAL(103.2, peaklist[0].getIntensity());

	// the following assumes a stable sort
	CPPUNIT_ASSERT_EQUAL(4.0, peaklist[1].getMass());
	CPPUNIT_ASSERT_EQUAL(8.2, peaklist[1].getIntensity());

	CPPUNIT_ASSERT_EQUAL(4.0, peaklist[2].getMass());
	CPPUNIT_ASSERT_EQUAL(5000.0, peaklist[2].getIntensity());

	CPPUNIT_ASSERT_EQUAL(5.0, peaklist[3].getMass());
	CPPUNIT_ASSERT_EQUAL(3.2, peaklist[3].getIntensity());

	CPPUNIT_ASSERT_EQUAL(8.0, peaklist[4].getMass());
	CPPUNIT_ASSERT_EQUAL(2003.2, peaklist[4].getIntensity());

}


void SortModifierTest::testModify() {
	ims::SortModifier<peaklist_t> modifier;
	modifier.modify(peaklist);
	assertHelp();
}


void SortModifierTest::testClone() {
	ims::SortModifier<peaklist_t> modifier;
	std::unique_ptr<ims::Modifier<peaklist_t> > cloned(modifier.clone());
	cloned->modify(peaklist);
	assertHelp();
}
