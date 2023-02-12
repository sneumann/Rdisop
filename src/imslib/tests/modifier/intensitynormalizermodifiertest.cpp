#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/peaklist.h>
#include <ims/massintensitypeak.h>
#include <ims/modifier/intensitynormalizermodifier.h>

class IntensityNormalizerModifierTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(IntensityNormalizerModifierTest);
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


CPPUNIT_TEST_SUITE_REGISTRATION(IntensityNormalizerModifierTest);


void IntensityNormalizerModifierTest::setUp() {
	peaklist.push_back(peak_t(5.0, 3.2));
	peaklist.push_back(peak_t(4.0, 8.2));
	peaklist.push_back(peak_t(2.0, 103.2));
	peaklist.push_back(peak_t(8.0, 2003.2));
	peaklist.push_back(peak_t(4.0, 5000.0));
}


void IntensityNormalizerModifierTest::tearDown() {
	peaklist.clear();
}

void IntensityNormalizerModifierTest::assertHelp() {
	CPPUNIT_ASSERT_EQUAL((size_t)5, peaklist.size());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2/5000.0, peaklist[0].getIntensity(), 1e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(8.2/5000.0, peaklist[1].getIntensity(), 1e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(103.2/5000.0, peaklist[2].getIntensity(), 1e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2003.2/5000.0, peaklist[3].getIntensity(), 1e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, peaklist[4].getIntensity(), 1e-6);
}


void IntensityNormalizerModifierTest::testModify() {
	ims::IntensityNormalizerModifier<peaklist_t> modifier(1.0);
	modifier.modify(peaklist);
	assertHelp();
}


void IntensityNormalizerModifierTest::testClone() {
	ims::IntensityNormalizerModifier<peaklist_t> modifier(1.0);
	std::unique_ptr<ims::Modifier<peaklist_t> > cloned(modifier.clone());
	cloned->modify(peaklist);
	assertHelp();
}
