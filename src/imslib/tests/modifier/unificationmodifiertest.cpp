#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/peaklist.h>
#include <ims/massintensitypeak.h>
#include <ims/modifier/unificationmodifier.h>

class UnificationModifierTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(UnificationModifierTest);
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


CPPUNIT_TEST_SUITE_REGISTRATION(UnificationModifierTest);


void UnificationModifierTest::setUp() {
	peaklist.push_back(peak_t(2.0, 103.2));
	peaklist.push_back(peak_t(4.0, 8.2));
	peaklist.push_back(peak_t(4.0, 5000.0));
	peaklist.push_back(peak_t(5.0, 3.2));
	peaklist.push_back(peak_t(8.0, 2003.2));
}


void UnificationModifierTest::tearDown() {
	peaklist.clear();
}

void UnificationModifierTest::assertHelp() {
	CPPUNIT_ASSERT_EQUAL((size_t)4, peaklist.size());
	CPPUNIT_ASSERT_EQUAL(2.0, peaklist[0].getMass());
	CPPUNIT_ASSERT_EQUAL(4.0, peaklist[1].getMass());
	CPPUNIT_ASSERT_EQUAL(5.0, peaklist[2].getMass());
	CPPUNIT_ASSERT_EQUAL(8.0, peaklist[3].getMass());
}


void UnificationModifierTest::testModify() {
	ims::UnificationModifier<peaklist_t> modifier;
	modifier.modify(peaklist);
	assertHelp();
}


void UnificationModifierTest::testClone() {
	ims::UnificationModifier<peaklist_t> modifier;
	std::unique_ptr<ims::Modifier<peaklist_t> > cloned(modifier.clone());
	cloned->modify(peaklist);
	assertHelp();
}
