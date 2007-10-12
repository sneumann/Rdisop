#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/alphabet.h>
#include <ims/pmffragmenter.h>

using namespace ims;

class PMFFragmenterTest : public CppUnit::TestFixture {
	
	typedef Alphabet alphabet_type;
	CPPUNIT_TEST_SUITE( PMFFragmenterTest );
	CPPUNIT_TEST( testFragmentationWithCleave );
	CPPUNIT_TEST( testFragmentationWithoutCleave );
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testFragmentationWithCleave();
	void testFragmentationWithoutCleave();
private:
	typedef double mass_t;
	typedef long scaled_mass_t;

	alphabet_type alphabet;
	std::string cleavage_chars;
	std::string prohibition_chars;
};

CPPUNIT_TEST_SUITE_REGISTRATION( PMFFragmenterTest );


void PMFFragmenterTest::setUp() {
	alphabet.push_back("A",1.1);
	alphabet.push_back("B",2.2);
	alphabet.push_back("C",3.3);
	alphabet.push_back("D",4.4);
	alphabet.push_back("E",5.5);
	alphabet.push_back("F",6.6);
	alphabet.push_back("G",7.7);
	alphabet.push_back("H",8.8);
	alphabet.push_back("I",9.9);

	cleavage_chars="AB";
	prohibition_chars="HI";
}


void PMFFragmenterTest::tearDown() {
}

void PMFFragmenterTest::testFragmentationWithCleave() {
	ims::PMFFragmenter<mass_t,scaled_mass_t> fragmenter(alphabet, cleavage_chars, prohibition_chars, true);
	ims::PMFFragmenter<mass_t,scaled_mass_t>::peaklist_type pl;

	fragmenter.predictSpectrum(&pl,"CD");
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.7, pl[0].getMass(), 1.0e-5);

	fragmenter.predictSpectrum(&pl,"AB");
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1, pl[0].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[1].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[1].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.2, pl[1].getMass(), 1.0e-5);

	fragmenter.predictSpectrum(&pl,"CGBAEEDDDAB");
	CPPUNIT_ASSERT_EQUAL((size_t)4, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(13.2, pl[0].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[1].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[1].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1, pl[1].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl[2].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)4, pl[2].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(25.3, pl[2].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[3].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)10, pl[3].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.2, pl[3].getMass(), 1.0e-5);

	// test if prohibition chars work
	fragmenter.predictSpectrum(&pl,"CDHIAHG");
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)7, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(44.0, pl[0].getMass(), 1.0e-5);

	// test if miscleaving works
	fragmenter.setMaxMiscleaves(1);
	fragmenter.predictSpectrum(&pl,"CDAGEAFE");
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(8.8, pl[0].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl[1].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[1].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[1].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(23.1, pl[1].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[2].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[2].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[2].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(14.3, pl[2].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl[3].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[3].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[3].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(26.4, pl[3].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[4].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl[4].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[4].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(12.1, pl[4].getMass(), 1.0e-5);

	// ... and finally with 2 miscleaves
	fragmenter.setMaxMiscleaves(2);
	fragmenter.predictSpectrum(&pl,"CDAGEAFE");
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(8.8, pl[0].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl[1].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[1].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[1].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(23.1, pl[1].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)8, pl[2].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[2].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[2].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(35.2, pl[2].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[3].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[3].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[3].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(14.3, pl[3].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl[4].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[4].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[4].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(26.4, pl[4].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[5].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl[5].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[5].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(12.1, pl[5].getMass(), 1.0e-5);
}

void PMFFragmenterTest::testFragmentationWithoutCleave() {
	ims::PMFFragmenter<mass_t,scaled_mass_t> fragmenter(alphabet, cleavage_chars, prohibition_chars, false);
	ims::PMFFragmenter<mass_t,scaled_mass_t>::peaklist_type pl;

	fragmenter.predictSpectrum(&pl,"CD");
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.7, pl[0].getMass(), 1.0e-5);

	fragmenter.predictSpectrum(&pl,"AB");
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl.size());

	fragmenter.predictSpectrum(&pl,"CGBAEEDDDAB");
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, pl[0].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl[1].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)4, pl[1].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(24.2, pl[1].getMass(), 1.0e-5);

	// test if prohibition chars work
	fragmenter.predictSpectrum(&pl,"CDHIAHG");
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)7, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(44.0, pl[0].getMass(), 1.0e-5);

	// test if miscleaving works
	fragmenter.setMaxMiscleaves(1);
	fragmenter.predictSpectrum(&pl,"CDAGEAFE");
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.7, pl[0].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl[1].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[1].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[1].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(22.0, pl[1].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[2].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[2].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[2].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(13.2, pl[2].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl[3].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[3].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[3].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(26.4, pl[3].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[4].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl[4].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[4].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(12.1, pl[4].getMass(), 1.0e-5);

	// ... and finally with 2 miscleaves
	fragmenter.setMaxMiscleaves(2);
	fragmenter.predictSpectrum(&pl,"CDAGEAFE");
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl.size());
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[0].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[0].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.7, pl[0].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl[1].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[1].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[1].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(22.0, pl[1].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)8, pl[2].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[2].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[2].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(35.2, pl[2].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[3].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[3].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[3].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(13.2, pl[3].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)5, pl[4].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)3, pl[4].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)1, pl[4].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(26.4, pl[4].getMass(), 1.0e-5);
	CPPUNIT_ASSERT_EQUAL((size_t)2, pl[5].getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)6, pl[5].getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)0, pl[5].getMiscleavageCount());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(12.1, pl[5].getMass(), 1.0e-5);
}
