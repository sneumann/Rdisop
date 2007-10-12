#include <fstream>
#include <algorithm>
#include <string>
//#include <map>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/distributedalphabet.h>
#include <ims/base/parser/alphabettextparser.h>

using namespace ims;

class DistributedAlphabetTest : public CppUnit::TestFixture {
	
	typedef DistributedAlphabet alphabet_type;
	
	CPPUNIT_TEST_SUITE(DistributedAlphabetTest);
	CPPUNIT_TEST(testLoad);
	CPPUNIT_TEST(testLoadWithParser);
	CPPUNIT_TEST(testPushBack);
	CPPUNIT_TEST(testSetGetProbability);
	CPPUNIT_TEST(testIsDistributionCorrect);
	CPPUNIT_TEST(testNormalizeDistribution);		
	CPPUNIT_TEST_SUITE_END();	
	public:
		void setUp();
		void tearDown();
		void testLoad();
		void testLoadWithParser();
		void testPushBack();
		void testSetGetProbability();		
		void testIsDistributionCorrect();
		void testNormalizeDistribution();
};


CPPUNIT_TEST_SUITE_REGISTRATION(DistributedAlphabetTest);


void DistributedAlphabetTest::setUp() {
	// set up for testLoad()
	std::ofstream ofs("distributedalphabet.temp");
	ofs << "# comment \n"
		"\t \t \n"
		"B 1.23 2\n"
		"A 3.0e5 2\n"
		" # another comment   \t\n"		
		" CDEfg  \t 2.74 1\n"
		"\n";
	ofs.close();
}


void DistributedAlphabetTest::tearDown() {
	std::remove("distributedalphabet.temp");
}


void DistributedAlphabetTest::testLoad() {
	alphabet_type *loadedAlphabet = new alphabet_type;
	loadedAlphabet->load("distributedalphabet.temp");
	// checks if it is sorted
	CPPUNIT_ASSERT_EQUAL(std::string("B"), loadedAlphabet->getName(0));
	CPPUNIT_ASSERT_EQUAL(1.23, loadedAlphabet->getMass(0));
	CPPUNIT_ASSERT_EQUAL(std::string("CDEfg"), loadedAlphabet->getName(1));
	CPPUNIT_ASSERT_EQUAL(2.74, loadedAlphabet->getMass(1));
	CPPUNIT_ASSERT_EQUAL(std::string("A"), loadedAlphabet->getName(2));
	CPPUNIT_ASSERT_EQUAL(300000.0, loadedAlphabet->getMass(2));
	CPPUNIT_ASSERT(loadedAlphabet->isDistributionCorrect());
	CPPUNIT_ASSERT_EQUAL(0.4, loadedAlphabet->getProbability("A"));
	CPPUNIT_ASSERT_EQUAL(0.4, loadedAlphabet->getProbability("B"));
	CPPUNIT_ASSERT_EQUAL(0.2, loadedAlphabet->getProbability("CDEfg"));
	delete loadedAlphabet;
}


void DistributedAlphabetTest::testLoadWithParser() {
	alphabet_type *loadedAlphabet = new alphabet_type;
	AlphabetTextParser parser;
	loadedAlphabet->load("distributedalphabet.temp", &parser);
	// checks whether it is sorted
	CPPUNIT_ASSERT_EQUAL(std::string("B"), loadedAlphabet->getName(0));
	CPPUNIT_ASSERT_EQUAL(1.23, loadedAlphabet->getMass(0));
	CPPUNIT_ASSERT_EQUAL(std::string("CDEfg"), loadedAlphabet->getName(1));
	CPPUNIT_ASSERT_EQUAL(2.74, loadedAlphabet->getMass(1));
	CPPUNIT_ASSERT_EQUAL(std::string("A"), loadedAlphabet->getName(2));
	CPPUNIT_ASSERT_EQUAL(300000.0, loadedAlphabet->getMass(2));
	CPPUNIT_ASSERT(!loadedAlphabet->isDistributionCorrect());
	delete loadedAlphabet;
}


void DistributedAlphabetTest::testPushBack() {
	alphabet_type a;
	a.push_back("Lys", 231, 0.34);
	a.push_back("Arg", 142, 0.23);
	a.push_back("P", 156, 0.0);	
	CPPUNIT_ASSERT_EQUAL(a.getProbability("P"), 0.0);
	CPPUNIT_ASSERT_EQUAL(a.getProbability("Arg"), 0.23);
	CPPUNIT_ASSERT_EQUAL(a.getProbability("Lys"), 0.34);
}


void DistributedAlphabetTest::testSetGetProbability() {
	alphabet_type *a = new alphabet_type;		
	a->push_back("Lys", 234, 0.3);
	a->push_back("Arg", 209, 0.8);
	a->setProbability("Lys", 0.2);
	CPPUNIT_ASSERT_EQUAL(0.2, a->getProbability("Lys"));
	a->setProbability("Arg", 0.6);
	CPPUNIT_ASSERT_EQUAL(0.6, a->getProbability("Arg"));
	delete a;
}


void DistributedAlphabetTest::testIsDistributionCorrect() {
	alphabet_type *a = new alphabet_type;	
	a->push_back("Lys", 231.8, 2);
	a->push_back("Arg", 341.01, 1);
	a->push_back("Pro", 125.9, 1);
	CPPUNIT_ASSERT(!a->isDistributionCorrect());
	a->setProbability("Lys", 0.43);
	a->setProbability("Arg", 0.43);
	a->setProbability("Pro", 0.14);	
	CPPUNIT_ASSERT(a->isDistributionCorrect());
	delete a;
}

void DistributedAlphabetTest::testNormalizeDistribution() {
	alphabet_type *a = new alphabet_type;	
	a->push_back("Lys", 231.8, 2);
	a->push_back("Arg", 341.01, 1);
	a->push_back("Pro", 125.9, 1);
	CPPUNIT_ASSERT(!a->isDistributionCorrect());
	a->normalizeDistribution();
	CPPUNIT_ASSERT(a->isDistributionCorrect());
	a->push_back("K", 189.03, 0.25);
	CPPUNIT_ASSERT(!a->isDistributionCorrect());
	a->normalizeDistribution();
	CPPUNIT_ASSERT(a->isDistributionCorrect());	
	delete a;
}
