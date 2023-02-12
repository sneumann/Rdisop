#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/peaklist.h>
#include <ims/massintensitypeak.h>
#include <ims/modifier/massrangemodifier.h>

class MassRangeModifierTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(MassRangeModifierTest);
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


CPPUNIT_TEST_SUITE_REGISTRATION(MassRangeModifierTest);


void MassRangeModifierTest::setUp() {
	peaklist.push_back(peak_t(17.0, 3.2));
	peaklist.push_back(peak_t(5.5, 3.2));
	peaklist.push_back(peak_t(5.0, 3.2));
	peaklist.push_back(peak_t(4.0, 8.2));
	peaklist.push_back(peak_t(2.0, 103.2));
	peaklist.push_back(peak_t(8.0, 2003.2));
	peaklist.push_back(peak_t(4.0, 5000.0));
	peaklist.push_back(peak_t(28.0, 3.2));
}


void MassRangeModifierTest::tearDown() {
	peaklist.clear();
}


void MassRangeModifierTest::assertHelp() {
	CPPUNIT_ASSERT_EQUAL((size_t)4, peaklist.size());
	CPPUNIT_ASSERT_EQUAL(17.0, peaklist[0].getMass());
	CPPUNIT_ASSERT_EQUAL(5.5, peaklist[1].getMass());
	CPPUNIT_ASSERT_EQUAL(5.0, peaklist[2].getMass());
	CPPUNIT_ASSERT_EQUAL(8.0, peaklist[3].getMass());
}


void MassRangeModifierTest::testModify() {
	ims::MassRangeModifier<peaklist_t> modifier(5.0, 20.0);
	modifier.modify(peaklist);
	assertHelp();
}


void MassRangeModifierTest::testClone() {
	ims::MassRangeModifier<peaklist_t> modifier(5.0, 20.0);
	std::unique_ptr<ims::Modifier<peaklist_t> > cloned(modifier.clone());
	cloned->modify(peaklist);
	assertHelp();
}
