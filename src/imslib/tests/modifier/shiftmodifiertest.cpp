#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/peaklist.h>
#include <ims/massintensitypeak.h>
#include <ims/modifier/shiftmodifier.h>

class ShiftModifierTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ShiftModifierTest);
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


CPPUNIT_TEST_SUITE_REGISTRATION(ShiftModifierTest);


void ShiftModifierTest::setUp() {
	peaklist.push_back(peak_t(5.0, 3.2));
	peaklist.push_back(peak_t(4.0, 8.2));
	peaklist.push_back(peak_t(2.0, 103.2));
	peaklist.push_back(peak_t(8.0, 2003.2));
	peaklist.push_back(peak_t(4.0, 5000.0));
}


void ShiftModifierTest::tearDown() {
	peaklist.clear();
}

void ShiftModifierTest::assertHelp() {
	CPPUNIT_ASSERT_EQUAL((size_t)5, peaklist.size());
	// change this to CPPUNIT_ASSERT_DOUBLES_EQUAL as soon as it fails
	CPPUNIT_ASSERT_EQUAL(5.0+15.0, peaklist[0].getMass());
	CPPUNIT_ASSERT_EQUAL(4.0+15.0, peaklist[1].getMass());
	CPPUNIT_ASSERT_EQUAL(2.0+15.0, peaklist[2].getMass());
	CPPUNIT_ASSERT_EQUAL(8.0+15.0, peaklist[3].getMass());
	CPPUNIT_ASSERT_EQUAL(4.0+15.0, peaklist[4].getMass());
}


void ShiftModifierTest::testModify() {
	ims::ShiftModifier<peaklist_t> modifier(15.0);
	modifier.modify(peaklist);
	assertHelp();
}


void ShiftModifierTest::testClone() {
	ims::ShiftModifier<peaklist_t> modifier(15.0);
	std::unique_ptr<ims::Modifier<peaklist_t> > cloned(modifier.clone());
	cloned->modify(peaklist);
	assertHelp();
}
