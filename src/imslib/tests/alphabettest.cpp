/**
 * alphabettest.cpp
 * 
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <fstream>
#include <cstdio>
#include <ims/alphabet.h>
#include <iostream>

using namespace ims;

class AlphabetTest : public CppUnit::TestFixture {
	typedef Alphabet alphabet_type;
	typedef alphabet_type::element_type element_type;
	typedef alphabet_type::size_type size_type;
	typedef alphabet_type::container elements_type;
	typedef alphabet_type::masses_type alphabet_masses_type;
	typedef element_type::isotopes_type isotopes_type;
	typedef isotopes_type::mass_type mass_type;
	typedef isotopes_type::abundance_type abundance_type;
	typedef isotopes_type::nominal_mass_type nominal_mass_type;	
	typedef isotopes_type::peaks_container peaks_container;
	
	
	CPPUNIT_TEST_SUITE(AlphabetTest);
	CPPUNIT_TEST(testConstructor);
	CPPUNIT_TEST(testPushBack);	
	CPPUNIT_TEST(testHasName);
	CPPUNIT_TEST(testLoad);
	CPPUNIT_TEST(testSortByValues);
	CPPUNIT_TEST(testSortByNames);
	CPPUNIT_TEST(testGetMasses);
	CPPUNIT_TEST_EXCEPTION( testGetMassThrow, Exception );
	CPPUNIT_TEST_EXCEPTION( testLoadThrow, IOException );
	CPPUNIT_TEST_SUITE_END();

	void initializeAlphabet(alphabet_type& chnops);
	
	public:
		void setUp() ;
		void tearDown() ;
		void testConstructor();
		void testPushBack();
		void testHasName();
		void testLoad();
		void testGetMasses();	
		void testSortByValues();
		void testSortByNames();
		void testGetMassThrow();
		void testLoadThrow();
};


CPPUNIT_TEST_SUITE_REGISTRATION(AlphabetTest);

void AlphabetTest::setUp() {
	std::ofstream ofs("alphabet.temp");
	ofs << "# comment \n"
		" # another one    \t\n"
		"\n"
		"\t \t \n"
		"  B 1.23\n"
		"A 3.0e5\n"
		" CDEfg  \t 2.74 \n";
	ofs.close();

	std::ofstream ofs2("alphabet2.temp");
	ofs2 << "# comment \n"
		"\n"
		"    b 6039.2 \n"
		"  d \t 901.1 \n"
		" c 56.9 \n"
		"a 254.9";
	ofs2.close();	
}


void AlphabetTest::tearDown() {
	std::remove("alphabet.temp");
	std::remove("alphabet2.temp");	
}

void AlphabetTest::testConstructor() {
	
	// tests default constructor
	alphabet_type a;
	size_type size = 0;
	CPPUNIT_ASSERT_EQUAL(a.size(), size);
	a.sortByNames();
	a.sortByValues();
	CPPUNIT_ASSERT_EQUAL(a.size(), size);
	
	// tests constructor with elements container
	
	// prepares elements
	element_type e1("e1");
	element_type e2("e2");	
	alphabet_type::container elements;
	elements.push_back(e1);
	elements.push_back(e2);
	
	// creates and tests alphabet
	alphabet_type *b = new alphabet_type(elements);
	size = 2;
	CPPUNIT_ASSERT_EQUAL(b->size(), size);
	delete b;
}


////////////////////////////////////////////////////////////////////////////////////////////
///// TODO: move the below cases to Weights tests where we deal with precision /////
////////////////////////////////////////////////////////////////////////////////////////////
//void AlphabetTest::testConstructor() {
//	Alphabet<int> a;
//	CPPUNIT_ASSERT_EQUAL((size_t)0, a.size());
//	CPPUNIT_ASSERT_EQUAL(1, a.getPrecision());
//	a.sortByNames();
//	a.sortByValues();
//	CPPUNIT_ASSERT_EQUAL((size_t)0, a.size());
//		
//	Alphabet<double> *b = new Alphabet<double>(0.1) ;
//	CPPUNIT_ASSERT_EQUAL((size_t)0, b->size());
//	CPPUNIT_ASSERT_EQUAL(0.1, b->getPrecision());
//	delete b;
//}
//
//
//void AlphabetTest::testSetPrecisionDoubleInt() {
//	Alphabet<double, int> a;
//	a.load("alphabet2.temp");
//
//	CPPUNIT_ASSERT_EQUAL(254.9, a.getRealMass("a"));
//
//	a.setPrecision(0.01);
//	CPPUNIT_ASSERT_EQUAL(254.9, a.getRealMass("a"));	
//	CPPUNIT_ASSERT_EQUAL(25490, a.getMass("a"));
//	
//	a.setPrecision(-0.9);
//	CPPUNIT_ASSERT_EQUAL(6039.2, a.getRealMass("b"));
//	CPPUNIT_ASSERT_EQUAL(6039, a.getMass("b"));
//
//	// TODO: how it should behave?
//	a.setPrecision(10);
//	CPPUNIT_ASSERT_EQUAL(901.1, a.getRealMass("d"));	
//	CPPUNIT_ASSERT_EQUAL(90, a.getMass("d"));	
//	
//}
//
//void AlphabetTest::testSetPrecisionInt() {
//	Alphabet<int> a;
//	a.load("alphabet.temp");
//	
//	CPPUNIT_ASSERT_EQUAL(1, a.getRealMass("B"));
//	CPPUNIT_ASSERT_EQUAL(1, a.getMass("B"));	
//	
//	a.setPrecision(0);
//	CPPUNIT_ASSERT_EQUAL(1, a.getRealMass("B"));
//	CPPUNIT_ASSERT_EQUAL(1, a.getMass("B"));	
//	
//	a.setPrecision(-12);
//	CPPUNIT_ASSERT_EQUAL(2, a.getRealMass("CDEfg"));
//	CPPUNIT_ASSERT_EQUAL(2, a.getMass("CDEfg"));
//	
//	a.setPrecision(15);
//	CPPUNIT_ASSERT_EQUAL(300000, a.getRealMass("A"));
//	CPPUNIT_ASSERT_EQUAL(20000, a.getMass("A"));		
//	
//}
//
//void AlphabetTest::testGetPrecision() {
//	Alphabet<double> a(0.9);
//	CPPUNIT_ASSERT_EQUAL(0.9, a.getPrecision());	
//	a.setPrecision(-2.8);
//	CPPUNIT_ASSERT_EQUAL(1.0, a.getPrecision());
//	a.setPrecision(0);
//	CPPUNIT_ASSERT_EQUAL(1.0, a.getPrecision());	
//	a.setPrecision(0.0001);	
//	CPPUNIT_ASSERT_EQUAL(0.0001, a.getPrecision());	
//}
//
//void AlphabetTest::testPushBackInt() {
//	Alphabet<int> a;
//	a.push_back("first", 23);
//	CPPUNIT_ASSERT_EQUAL((size_t)1, a.size());
//	CPPUNIT_ASSERT_EQUAL(23, a.getRealMass("first"));
//	CPPUNIT_ASSERT_EQUAL(23, a.getMass("first"));
//	a.push_back("second", 1);
//	a.push_back("third", 3);
//	a.push_back("fourth", 12);
//	CPPUNIT_ASSERT_EQUAL((size_t)4, a.size());
//}
//
//void AlphabetTest::testPushBackDoubleInt() {
//	Alphabet<double, int> a(0.02);
//	a.push_back("first", 2.3);
//	CPPUNIT_ASSERT_EQUAL((size_t)1, a.size());
//	CPPUNIT_ASSERT_EQUAL(2.3, a.getRealMass("first"));
//	CPPUNIT_ASSERT_EQUAL(115, a.getMass("first"));
//	a.push_back("second", 0.1);
//	a.push_back("third", 0.3);
//	a.push_back("fourth", 1.2);
//	CPPUNIT_ASSERT_EQUAL((size_t)4, a.size());
//	CPPUNIT_ASSERT_EQUAL(1.2, a.getRealMass("fourth"));
//	CPPUNIT_ASSERT_EQUAL(60, a.getMass("fourth"));	
//}

void AlphabetTest::testPushBack() {
	alphabet_type a;

	// tests pushing back an element	
	
	// creates an element
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	
	element_type element("h", distributionH);
	a.push_back(element);
	
	size_type a_size = 1;
	CPPUNIT_ASSERT_EQUAL(a.size(), a_size);
	CPPUNIT_ASSERT_EQUAL(a.getMass("h"), 1.007825);

	// creates another elements without distribution
	element_type p("p");
	element_type s("s");	
	
	a.push_back(p);
	a.push_back(s);
	a_size = 3;
	CPPUNIT_ASSERT_EQUAL(a.size(), a_size);
	CPPUNIT_ASSERT(a.hasName("p"));
	CPPUNIT_ASSERT(a.hasName("s"));
	

	// tests pushing back name and value

	mass_type value = 24.0;
	a.push_back("name", value);
	CPPUNIT_ASSERT_EQUAL(a.size(), ++a_size);
	CPPUNIT_ASSERT_EQUAL(a.getMass("name"), value);
		
}


void AlphabetTest::testHasName() {
	alphabet_type a;
	a.load("alphabet.temp");
	CPPUNIT_ASSERT(a.hasName("CDEfg"));
	CPPUNIT_ASSERT(!a.hasName("CDEf"));
}


void AlphabetTest::testLoad() {
	alphabet_type *test_load = new alphabet_type();
	test_load->load("alphabet2.temp");
	size_type size = 4;
	CPPUNIT_ASSERT_EQUAL(test_load->size(), size);	
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("a"), 254.9);
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("b"), 6039.2);
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("c"), 56.9);
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("d"), 901.1);

	try {
		test_load->load("something.incorrect");
	} catch (IOException) {
		// do nothing
	}

	CPPUNIT_ASSERT_EQUAL(test_load->size(), size);	
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("a"), 254.9);
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("b"), 6039.2);
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("c"), 56.9);
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("d"), 901.1);
	
	test_load->load("alphabet.temp");
	size = 3;
	CPPUNIT_ASSERT_EQUAL(test_load->size(), size);	
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("A"), 300000.0);
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("B"), 1.23);
	CPPUNIT_ASSERT_EQUAL(test_load->getMass("CDEfg"), 2.74);

	delete test_load;
}


void AlphabetTest::testGetMasses() {
	alphabet_type chnops;
	
	initializeAlphabet(chnops);
	
	// check monoisotopic masses
	alphabet_masses_type masses = chnops.getMasses();
	for (alphabet_masses_type::size_type i = 0; i < masses.size(); ++i) {
		CPPUNIT_ASSERT_DOUBLES_EQUAL(masses.at(i), chnops.getMass(i), 1.0e-6);
	}	
		
	// check average masses
	masses = chnops.getAverageMasses();
	for (alphabet_masses_type::size_type i = 0; i < masses.size(); ++i) {
		CPPUNIT_ASSERT_DOUBLES_EQUAL(masses.at(i), chnops.getElement(i).getAverageMass(), 1.0e-6);
	}
	
}


void AlphabetTest::testSortByValues() {
	alphabet_type a;
	a.load("alphabet2.temp");
	// masses must be sorted by default after loading
	size_type a_size = 4;	
	CPPUNIT_ASSERT_EQUAL(a.size(), a_size);
	CPPUNIT_ASSERT_EQUAL(a.getMass(0), 56.9);
	CPPUNIT_ASSERT_EQUAL(a.getName(0), std::string("c"));
	CPPUNIT_ASSERT_EQUAL(a.getMass(1), 254.9);
	CPPUNIT_ASSERT_EQUAL(a.getName(1), std::string("a"));
	CPPUNIT_ASSERT_EQUAL(a.getMass(2), 901.1);
	CPPUNIT_ASSERT_EQUAL(a.getName(2), std::string("d"));
	CPPUNIT_ASSERT_EQUAL(a.getMass(3), 6039.2);
	CPPUNIT_ASSERT_EQUAL(a.getName(3), std::string("b"));
}


void AlphabetTest::testSortByNames() {
	alphabet_type a;
	a.load("alphabet2.temp");
	a.sortByNames();
	size_type a_size = 4;	
	CPPUNIT_ASSERT_EQUAL(a.size(), a_size);
	CPPUNIT_ASSERT_EQUAL(a.getMass(0), 254.9);
	CPPUNIT_ASSERT_EQUAL(a.getName(0), std::string("a"));
	CPPUNIT_ASSERT_EQUAL(a.getMass(1), 6039.2);
	CPPUNIT_ASSERT_EQUAL(a.getName(1), std::string("b"));
	CPPUNIT_ASSERT_EQUAL(a.getMass(2), 56.9);
	CPPUNIT_ASSERT_EQUAL(a.getName(2), std::string("c"));
	CPPUNIT_ASSERT_EQUAL(a.getMass(3), 901.1);
	CPPUNIT_ASSERT_EQUAL(a.getName(3), std::string("d"));
}


void AlphabetTest::testGetMassThrow() {
	alphabet_type a;
	a.load("alphabet2.temp");	
	a.getMass("e");
}


void AlphabetTest::testLoadThrow() {
	alphabet_type a;
	a.load("nonexistent.temp");
}

void AlphabetTest::initializeAlphabet(alphabet_type& chnops) {

	isotopes_type::SIZE = 10;
	isotopes_type::ABUNDANCES_SUM_ERROR = 0.0001;

// Hydrogen
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);

// Oxygen		
	nominal_mass_type massO = 16;
	peaks_container peaksO;
	peaksO.push_back(peaks_container::value_type(-0.005085, 0.99762));
	peaksO.push_back(peaks_container::value_type(-0.000868, 0.00038));
	peaksO.push_back(peaks_container::value_type(-0.000839, 0.002));
	
	isotopes_type distributionO(peaksO, massO);

// Carbonate	
	nominal_mass_type massC = 12;
	peaks_container peaksC;
	peaksC.push_back(peaks_container::value_type(0.0, 0.9889));
	peaksC.push_back(peaks_container::value_type(0.003355, 0.0111));

	isotopes_type distributionC(peaksC, massC);

// Nitrogen
	nominal_mass_type massN = 14;
	peaks_container peaksN;
	peaksN.push_back(peaks_container::value_type(0.003074, 0.99634));
	peaksN.push_back(peaks_container::value_type(0.000109, 0.00366));

	isotopes_type distributionN(peaksN, massN);

// Sulfur
	nominal_mass_type massS = 32;
	peaks_container peaksS;
	peaksS.push_back(peaks_container::value_type(-0.027929, 0.9502));
	peaksS.push_back(peaks_container::value_type(-0.028541, 0.0075));
	peaksS.push_back(peaks_container::value_type(-0.032133, 0.0421));
	peaksS.push_back(peaks_container::value_type());
	peaksS.push_back(peaks_container::value_type(-0.032919, 0.0002));

	isotopes_type distributionS(peaksS, massS);

// Phosphor
	nominal_mass_type massP = 31;
	peaks_container peaksP;
	peaksP.push_back(peaks_container::value_type(-0.026238, 1.0));

	isotopes_type distributionP(peaksP, massP);

	element_type H("H", distributionH);
	element_type C("C", distributionC);
	element_type N("N", distributionN);
	element_type O("O", distributionO);
	element_type P("P", distributionP);
	element_type S("S", distributionS);
	
	chnops.push_back(H);
	chnops.push_back(C);
	chnops.push_back(N);
	chnops.push_back(O);
	chnops.push_back(P);
	chnops.push_back(S);
}

