#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <ims/base/parser/moleculesequenceparser.h>

using namespace ims;

// TODO: write exception test case
class MoleculeSequenceParserTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(MoleculeSequenceParserTest);
		CPPUNIT_TEST(testParse);
		CPPUNIT_TEST_EXCEPTION( testParseThrow, UnknownCharacterException );		
		CPPUNIT_TEST_SUITE_END();

		typedef MoleculeSequenceParser::sequence_type sequence_type;
		typedef MoleculeSequenceParser::container container;
	public:
		void setUp();
		void tearDown();
		void testParse();
		void testParseThrow();
		
};

CPPUNIT_TEST_SUITE_REGISTRATION(MoleculeSequenceParserTest);

void MoleculeSequenceParserTest::setUp() {
}

void MoleculeSequenceParserTest::tearDown() {
}

void MoleculeSequenceParserTest::testParse() {
	sequence_type elementH("H"), elementArg("Arg"), 
				  elementCl("Cl"), elementO("O");
				  
	// checks simple case
	sequence_type sequence1("HH2200OArg2Cl1H");
	MoleculeSequenceParser parser;
	parser.parse(sequence1);
	container parsedElements = parser.getElements();
	CPPUNIT_ASSERT(parsedElements.size() == 4);
	CPPUNIT_ASSERT(parsedElements[elementH] == 2202);
	CPPUNIT_ASSERT(parsedElements[elementO] == 1);
	CPPUNIT_ASSERT(parsedElements[elementArg] == 2);
	CPPUNIT_ASSERT(parsedElements[elementCl] == 1);

	// checks case with parsing brackets
	sequence_type sequence2("H(H2O)2Cl");
	parser.parse(sequence2);
	parsedElements = parser.getElements();	
	CPPUNIT_ASSERT(parsedElements.size() == 3);
	CPPUNIT_ASSERT(parsedElements[elementH] == 5);
	CPPUNIT_ASSERT(parsedElements[elementO] == 2);
	CPPUNIT_ASSERT(parsedElements[elementCl] == 1);

	// checks case with >1 brackets 
	sequence_type sequence3("H(H2O)(ClArg2O)4Cl");
	parser.parse(sequence3);
	parsedElements = parser.getElements();	
	CPPUNIT_ASSERT(parsedElements.size() == 4);
	CPPUNIT_ASSERT(parsedElements[elementH] == 3);
	CPPUNIT_ASSERT(parsedElements[elementO] == 5);
	CPPUNIT_ASSERT(parsedElements[elementArg] == 8);	
	CPPUNIT_ASSERT(parsedElements[elementCl] == 5);
	
}

void MoleculeSequenceParserTest::testParseThrow() {
	sequence_type elementH("H"), elementArg("Arg"), 
				  elementCl("Cl"), elementO("O");	
	// checks the case with enclosing brackets
	sequence_type sequence("H(Cl(H))2");
	MoleculeSequenceParser parser;
	parser.parse(sequence);	
}
