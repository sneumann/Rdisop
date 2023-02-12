#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/peaklist.h>
#include <ims/massintensitypeak.h>
#include <ims/modifier/voidmodifier.h>

class VoidModifierTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(VoidModifierTest);
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


CPPUNIT_TEST_SUITE_REGISTRATION(VoidModifierTest);


void VoidModifierTest::setUp() {
	peaklist.push_back(peak_t(5.0, 3.2));
	peaklist.push_back(peak_t(4.0, 8.2));
	peaklist.push_back(peak_t(2.0, 103.2));
	peaklist.push_back(peak_t(8.0, 2003.2));
	peaklist.push_back(peak_t(4.0, 5000.0));
}


void VoidModifierTest::tearDown() {
	peaklist.clear();
}

void VoidModifierTest::assertHelp() {
	CPPUNIT_ASSERT_EQUAL((size_t)5, peaklist.size());
	CPPUNIT_ASSERT_EQUAL(5.0, peaklist[0].getMass());
	CPPUNIT_ASSERT_EQUAL(4.0, peaklist[1].getMass());
	CPPUNIT_ASSERT_EQUAL(2.0, peaklist[2].getMass());
	CPPUNIT_ASSERT_EQUAL(8.0, peaklist[3].getMass());
	CPPUNIT_ASSERT_EQUAL(4.0, peaklist[4].getMass());
}


void VoidModifierTest::testModify() {
	ims::VoidModifier<peaklist_t> modifier;
	modifier.modify(peaklist);
	assertHelp();
}


void VoidModifierTest::testClone() {
	ims::VoidModifier<peaklist_t> modifier;
	std::unique_ptr<ims::Modifier<peaklist_t> > cloned(modifier.clone());
	cloned->modify(peaklist);
	assertHelp();
}
