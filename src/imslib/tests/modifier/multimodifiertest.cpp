#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/peaklist.h>
#include <ims/massintensitypeak.h>
#include <ims/modifier/sortmodifier.h>
#include <ims/modifier/unificationmodifier.h>
#include <ims/modifier/multimodifier.h>

class MultiModifierTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(MultiModifierTest);
	CPPUNIT_TEST( testModify );
	CPPUNIT_TEST( testClone );
	CPPUNIT_TEST( testAssignment );
	CPPUNIT_TEST_SUITE_END();

	public:
		typedef ims::MassIntensityPeak<double,double> peak_t;
		typedef ims::PeakList<peak_t> peaklist_t;

		void setUp();
		void tearDown();
		void testModify();
		void testClone();
		void testAssignment();
		void assertHelp();
		void preparePeaklist();

	private:
		peaklist_t peaklist;
};


CPPUNIT_TEST_SUITE_REGISTRATION(MultiModifierTest);


void MultiModifierTest::setUp() {
	preparePeaklist();
}


void MultiModifierTest::tearDown() {
	peaklist.clear();
}


void MultiModifierTest::assertHelp() {
	CPPUNIT_ASSERT_EQUAL((size_t)4, peaklist.size());
	CPPUNIT_ASSERT_EQUAL(2.0, peaklist[0].getMass());
	CPPUNIT_ASSERT_EQUAL(4.0, peaklist[1].getMass());
	CPPUNIT_ASSERT_EQUAL(5.0, peaklist[2].getMass());
	CPPUNIT_ASSERT_EQUAL(8.0, peaklist[3].getMass());
}


void MultiModifierTest::preparePeaklist() {
	peaklist.clear();
	peaklist.push_back(peak_t(5.0, 3.2));
	peaklist.push_back(peak_t(4.0, 8.2));
	peaklist.push_back(peak_t(2.0, 103.2));
	peaklist.push_back(peak_t(8.0, 2003.2));
	peaklist.push_back(peak_t(4.0, 5000.0));
}


void MultiModifierTest::testModify() {
	ims::MultiModifier<peaklist_t> modifier;
	modifier.addModifier(std::unique_ptr<ims::Modifier<peaklist_t> >(
		new ims::SortModifier<peaklist_t>));
	modifier.addModifier(std::unique_ptr<ims::Modifier<peaklist_t> >(
		new ims::UnificationModifier<peaklist_t>));
	modifier.modify(peaklist);
	assertHelp();
}


void MultiModifierTest::testClone() {
        ims::MultiModifier<peaklist_t> modifier;
        modifier.addModifier(std::unique_ptr<ims::Modifier<peaklist_t> >(
                new ims::SortModifier<peaklist_t>));
        modifier.addModifier(std::unique_ptr<ims::Modifier<peaklist_t> >(
                new ims::UnificationModifier<peaklist_t>));

        std::unique_ptr<ims::Modifier<peaklist_t> > cloned(modifier.clone());
        cloned->modify(peaklist);
        assertHelp();
}


void MultiModifierTest::testAssignment() {
	ims::MultiModifier<peaklist_t> modifier, modifier2;
	modifier.addModifier(std::unique_ptr<ims::Modifier<peaklist_t> >(
		new ims::SortModifier<peaklist_t>));
	modifier.addModifier(std::unique_ptr<ims::Modifier<peaklist_t> >(
		new ims::UnificationModifier<peaklist_t>));
	
	modifier2=modifier;
	modifier2.modify(peaklist);
	assertHelp();

	preparePeaklist();
	modifier.modify(peaklist);
	assertHelp();

	preparePeaklist();
	modifier=modifier;
	modifier.modify(peaklist);
	assertHelp();
}
