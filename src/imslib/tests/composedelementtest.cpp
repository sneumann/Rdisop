/**
 * composedelementtest.cpp
 * 
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <ims/composedelement.h>	
#include <ims/alphabet.h>

using namespace ims;

class ComposedElementTest : public CppUnit::TestFixture {
	typedef ComposedElement composed_element_type;
	typedef composed_element_type::name_type name_type;
	typedef composed_element_type::isotopes_type isotopes_type;
	typedef composed_element_type::container elements_container;
	typedef isotopes_type::mass_type mass_type;
	typedef isotopes_type::abundance_type abundance_type;
	typedef isotopes_type::nominal_mass_type nominal_mass_type;	
	typedef isotopes_type::peaks_container peaks_container;
	
	CPPUNIT_TEST_SUITE(ComposedElementTest);
	CPPUNIT_TEST(testConstructorSequenceAlphabet);
	CPPUNIT_TEST(testConstructorTexNotationSequenceAlphabet);
	CPPUNIT_TEST(testConstructorDecompositionAlphabet);
	CPPUNIT_TEST(testGetElementAbundance);
	CPPUNIT_TEST(testCopyConstructor);
	CPPUNIT_TEST(testOperatorAssign);
	CPPUNIT_TEST(testOperatorEqual);	
	CPPUNIT_TEST(testOperatorNotEqual);
	CPPUNIT_TEST(testOperatorMinus);
	CPPUNIT_TEST(testOperatorPlus);
	CPPUNIT_TEST(testUpdateSequence);
	CPPUNIT_TEST(testUpdateIsotopeDistribution);
	CPPUNIT_TEST_SUITE_END();

	Element *hydrogen;	
	Element *oxygen;
	Element *carbon;
	
	public:
		void setUp() ;
		void tearDown() ;
		void testConstructorNameElements();
		void testConstructorSequenceAlphabet();
		void testConstructorTexNotationSequenceAlphabet();
		void testConstructorDecompositionAlphabet();
		void testGetElementAbundance();
		void testCopyConstructor();
		void testOperatorAssign();
		void testOperatorEqual();
		void testOperatorNotEqual();
		void testOperatorMinus();
		void testOperatorPlus();
		void testUpdateSequence();
		void testUpdateIsotopeDistribution();
};


CPPUNIT_TEST_SUITE_REGISTRATION(ComposedElementTest);


void ComposedElementTest::setUp() {

// Hydrogen
	name_type nameH = "H";

	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
		
	hydrogen = new Element(nameH, distributionH);
	
// Oxygen		
	name_type nameO = "O";

	nominal_mass_type massO = 16;
	peaks_container peaksO;
	peaksO.push_back(peaks_container::value_type(-0.005085, 0.99762));
	peaksO.push_back(peaks_container::value_type(-0.000868, 0.00038));
	peaksO.push_back(peaks_container::value_type(-0.000839, 0.002));
	
	isotopes_type distributionO(peaksO, massO);
	
	oxygen = new Element(nameO, distributionO);	
	
// Carbon
	name_type nameC = "C";

	nominal_mass_type massC = 12;
	peaks_container peaksC;
	peaksC.push_back(peaks_container::value_type(0.0, 0.9889));
	peaksC.push_back(peaks_container::value_type(0.003355, 0.0111));

	isotopes_type distributionC(peaksC, massC);
		
	carbon = new Element(nameC, distributionC);
	
}


void ComposedElementTest::tearDown() {	
	
	delete carbon;
	delete oxygen;
	delete hydrogen;
	
}


void ComposedElementTest::testConstructorNameElements() {
	Element elementH(*hydrogen);
	Element elementO(*oxygen);
	Element elementC(*carbon);
	
	elements_container elements;
	elements[elementH] = 4;
	elements[elementO] = 2;
	elements[elementC] = 2;
	
	// checks constructor with default sequence order
	composed_element_type molecule(elements);
		
	////////////////////////////////////////////////////////////
	// sequence (but not directly, since we don't know the order 
	// in which elements appear in sequence)
	name_type sequence = molecule.getSequence();
	// sequence must be of a certain length
	CPPUNIT_ASSERT_EQUAL(sequence.size(), static_cast<name_type::size_type>(6));
	// sequence must contain certain substrings
	CPPUNIT_ASSERT(sequence.find("H4") != name_type::npos);
	CPPUNIT_ASSERT(sequence.find("C2") != name_type::npos);
	CPPUNIT_ASSERT(sequence.find("O2") != name_type::npos);	
	////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////
	// isotope distribution
	isotopes_type isodistr = molecule.getIsotopeDistribution();
// C2H4O2 isotope distribution
//0: {60.02113;0.972690002}
//1: {61.0245862;0.0231608083}
//2: {62.0254827;0.00405304857}
//3: {63.0288285;9.1561898e-05}
//4: {64.0299819;4.48812038e-06}
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(0), static_cast<mass_type>(60.02113), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(1), static_cast<mass_type>(61.0245862), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(2), static_cast<mass_type>(62.0254827), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(3), static_cast<mass_type>(63.0288285), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(0), static_cast<abundance_type>(0.972690002), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(1), static_cast<abundance_type>(0.0231608083), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(2), static_cast<abundance_type>(0.00405304857), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(3), static_cast<abundance_type>(9.1561898e-05), 1.0e-6);
	////////////////////////////////////////////////////////////
	
	// checks constructor with a given sequence order 			
	std::vector<std::string> sequence_order;
	sequence_order.push_back("O");
	sequence_order.push_back("H");
	sequence_order.push_back("C");
	
	composed_element_type other_molecule(elements, &sequence_order);
	////////////////////////////////////////////////////////////
	// sequence
	CPPUNIT_ASSERT_EQUAL(other_molecule.getSequence(), static_cast<name_type>("O2H4C2"));
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	// isotope distribution
	isodistr = other_molecule.getIsotopeDistribution();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(0), static_cast<mass_type>(60.02113), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(1), static_cast<mass_type>(61.0245862), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(2), static_cast<mass_type>(62.0254827), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(3), static_cast<mass_type>(63.0288285), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(0), static_cast<abundance_type>(0.972690002), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(1), static_cast<abundance_type>(0.0231608083), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(2), static_cast<abundance_type>(0.00405304857), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(3), static_cast<abundance_type>(9.1561898e-05), 1.0e-6);
	////////////////////////////////////////////////////////////
	
}


void ComposedElementTest::testConstructorSequenceAlphabet() {
	Alphabet cho;
	cho.push_back(*hydrogen);
	cho.push_back(*carbon);
	cho.push_back(*oxygen);
	
	composed_element_type molecule("C2H4O2", cho);
	CPPUNIT_ASSERT_EQUAL(molecule.getSequence(), static_cast<name_type>("C2H4O2"));	
	CPPUNIT_ASSERT_EQUAL(molecule.getElements().size(), 
				static_cast<elements_container::size_type>(3));
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("C"), 
							static_cast<elements_container::mapped_type>(2));
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("H"), 
							static_cast<elements_container::mapped_type>(4));		
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("O"), 
							static_cast<elements_container::mapped_type>(2));	
	// checks if isotope distribution is not yet initialized
	CPPUNIT_ASSERT(molecule.getIsotopeDistribution().empty());							
}

void ComposedElementTest::testConstructorTexNotationSequenceAlphabet() {
	Alphabet cho;
	cho.push_back(*hydrogen);
	cho.push_back(*carbon);
	cho.push_back(*oxygen);
	
	composed_element_type molecule("C_{2}H_{4}O_{2}", cho, composed_element_type::TEX_NOTATION_MOLECULE_SEQUENCE_TYPE);
	CPPUNIT_ASSERT_EQUAL(molecule.getSequence(), static_cast<name_type>("C_{2}H_{4}O_{2}"));	
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("C"), 
							static_cast<elements_container::mapped_type>(2));
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("H"), 
							static_cast<elements_container::mapped_type>(4));		
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("O"), 
							static_cast<elements_container::mapped_type>(2));	
	// checks if isotope distribution is not yet initialized
	CPPUNIT_ASSERT(molecule.getIsotopeDistribution().empty());							
}

void ComposedElementTest::testConstructorDecompositionAlphabet() {
	Alphabet cho;
	cho.push_back(*hydrogen);
	cho.push_back(*carbon);
	cho.push_back(*oxygen);

	std::vector<unsigned int> decomposition;
	decomposition.push_back(static_cast<unsigned int>(3));
	decomposition.push_back(static_cast<unsigned int>(2));
	decomposition.push_back(static_cast<unsigned int>(1));

	composed_element_type molecule(decomposition, cho);

	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("C"), 
							static_cast<elements_container::mapped_type>(2));
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("H"), 
							static_cast<elements_container::mapped_type>(3));							
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("O"), 
							static_cast<elements_container::mapped_type>(1));	

	// checks if isotope distribution is not yet initialized
	CPPUNIT_ASSERT(molecule.getIsotopeDistribution().empty());
	// checks if sequence is not yet initialized
	CPPUNIT_ASSERT(molecule.getSequence().empty());
}


void ComposedElementTest::testUpdateSequence() {
	Element elementH(*hydrogen);
	Element elementO(*oxygen);
	Element elementC(*carbon);
	
	elements_container elements;
	elements[elementH] = 2;
	elements[elementC] = 3;
	elements[elementO] = 4;

	std::vector<std::string> sequence_order;
	sequence_order.push_back("H");
	sequence_order.push_back("C");
	sequence_order.push_back("O");

	// initializes molecule with random order of elements in the sequence
	composed_element_type molecule(elements);
	
	// updates sequence by a given order of element names
	molecule.updateSequence(&sequence_order);
	CPPUNIT_ASSERT_EQUAL(molecule.getSequence(), static_cast<name_type>("H2C3O4"));	

	// re-intializes sequence order different from above
	sequence_order.clear();
	sequence_order.push_back("C");
	sequence_order.push_back("H");
	sequence_order.push_back("O");

	// updates sequence by a new order of element names
	molecule.updateSequence(&sequence_order);
	CPPUNIT_ASSERT_EQUAL(molecule.getSequence(), static_cast<name_type>("C3H2O4"));	
}


void ComposedElementTest::testUpdateIsotopeDistribution() {
	Alphabet cho;
	cho.push_back(*hydrogen);
	cho.push_back(*carbon);
	cho.push_back(*oxygen);

	std::vector<unsigned int> decomposition;
	decomposition.push_back(static_cast<unsigned int>(4));
	decomposition.push_back(static_cast<unsigned int>(2));
	decomposition.push_back(static_cast<unsigned int>(2));
	
	// initializes molecule without setting isotope distribution and sequence
	composed_element_type molecule(decomposition, cho);

	molecule.updateIsotopeDistribution();

	isotopes_type isodistr = molecule.getIsotopeDistribution();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(0), static_cast<mass_type>(60.02113), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(1), static_cast<mass_type>(61.0245862), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(2), static_cast<mass_type>(62.0254827), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(3), static_cast<mass_type>(63.0288285), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(0), static_cast<abundance_type>(0.972690002), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(1), static_cast<abundance_type>(0.0231608083), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(2), static_cast<abundance_type>(0.00405304857), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(3), static_cast<abundance_type>(9.1561898e-05), 1.0e-6);

}


void ComposedElementTest::testGetElementAbundance() {
	Element e1("A");
	Element e2("B");
	Element e3("C");
	
	elements_container elements;
	elements[e1] = 3;
	elements[e2] = 2;
	elements[e3] = 1;
	
	composed_element_type molecule(elements);
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("A"), 
							static_cast<elements_container::mapped_type>(3));
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("B"), 
							static_cast<elements_container::mapped_type>(2));							
	CPPUNIT_ASSERT_EQUAL(molecule.getElementAbundance("C"), 
							static_cast<elements_container::mapped_type>(1));		
}


void ComposedElementTest::testCopyConstructor() {	
	Element elementH(*hydrogen);
	Element elementO(*oxygen);
	Element elementC(*carbon);
	
	elements_container elements;
	elements[elementH] = 4;
	elements[elementO] = 2;
	elements[elementC] = 2;
	
	// checks constructor with default sequence order
	composed_element_type molecule(elements);
	
	composed_element_type molecule_copy(molecule);
	
	////////////////////////////////////////////////////////////
	// checks sequence (but not directly, since we don't know the order 
	// in which elements appear in sequence)
	name_type sequence = molecule_copy.getSequence();
	// sequence must be of a certain length
	CPPUNIT_ASSERT_EQUAL(sequence.size(), static_cast<name_type::size_type>(6));
	// sequence must contain certain substrings
	CPPUNIT_ASSERT(sequence.find("H4") != name_type::npos);
	CPPUNIT_ASSERT(sequence.find("C2") != name_type::npos);
	CPPUNIT_ASSERT(sequence.find("O2") != name_type::npos);	
	////////////////////////////////////////////////////////////

	CPPUNIT_ASSERT_EQUAL(molecule_copy.getElementAbundance("C"), 
							static_cast<elements_container::mapped_type>(2));
	CPPUNIT_ASSERT_EQUAL(molecule_copy.getElementAbundance("H"), 
							static_cast<elements_container::mapped_type>(4));							
	CPPUNIT_ASSERT_EQUAL(molecule_copy.getElementAbundance("O"), 
							static_cast<elements_container::mapped_type>(2));	
	isotopes_type isodistr = molecule_copy.getIsotopeDistribution();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(0), static_cast<mass_type>(60.02113), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(1), static_cast<mass_type>(61.0245862), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(2), static_cast<mass_type>(62.0254827), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(3), static_cast<mass_type>(63.0288285), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(0), static_cast<abundance_type>(0.972690002), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(1), static_cast<abundance_type>(0.0231608083), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(2), static_cast<abundance_type>(0.00405304857), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(3), static_cast<abundance_type>(9.1561898e-05), 1.0e-6);
	
}


void ComposedElementTest::testOperatorAssign() {
	
	Element elementH(*hydrogen);
	Element elementO(*oxygen);
	Element elementC(*carbon);
	
	elements_container elements;
	elements[elementH] = 4;
	elements[elementO] = 2;
	elements[elementC] = 2;
	
	// checks constructor with default sequence order
	composed_element_type molecule(elements);
	
	composed_element_type molecule_assign = molecule;
	
	////////////////////////////////////////////////////////////
	// checks sequence (but not directly, since we don't know the order 
	// in which elements appear in sequence)
	name_type sequence = molecule_assign.getSequence();
	// sequence must be of a certain length
	CPPUNIT_ASSERT_EQUAL(sequence.size(), static_cast<name_type::size_type>(6));
	// sequence must contain certain substrings
	CPPUNIT_ASSERT(sequence.find("H4") != name_type::npos);
	CPPUNIT_ASSERT(sequence.find("C2") != name_type::npos);
	CPPUNIT_ASSERT(sequence.find("O2") != name_type::npos);	
	////////////////////////////////////////////////////////////

	CPPUNIT_ASSERT_EQUAL(molecule_assign.getElementAbundance("C"), 
							static_cast<elements_container::mapped_type>(2));
	CPPUNIT_ASSERT_EQUAL(molecule_assign.getElementAbundance("H"), 
							static_cast<elements_container::mapped_type>(4));							
	CPPUNIT_ASSERT_EQUAL(molecule_assign.getElementAbundance("O"), 
							static_cast<elements_container::mapped_type>(2));	
	isotopes_type isodistr = molecule_assign.getIsotopeDistribution();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(0), static_cast<mass_type>(60.02113), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(1), static_cast<mass_type>(61.0245862), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(2), static_cast<mass_type>(62.0254827), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(3), static_cast<mass_type>(63.0288285), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(0), static_cast<abundance_type>(0.972690002), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(1), static_cast<abundance_type>(0.0231608083), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(2), static_cast<abundance_type>(0.00405304857), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(3), static_cast<abundance_type>(9.1561898e-05), 1.0e-6);
	
}


void ComposedElementTest::testOperatorEqual() {

	// checks for type of constructors with sequence	
	Alphabet cho;
	cho.push_back(*hydrogen);
	cho.push_back(*carbon);
	cho.push_back(*oxygen);
	
	composed_element_type molecule1("H4C2O2", cho);
	
	composed_element_type molecule1_equal("H4C2O2", cho);

	CPPUNIT_ASSERT(molecule1 == molecule1_equal);	
	
	// checks for type of constructors with elements
	Element elementH(*hydrogen);
	Element elementO(*oxygen);
	Element elementC(*carbon);
	
	elements_container elements;
	elements[elementH] = 4;
	elements[elementO] = 2;
	elements[elementC] = 2;
	
	// checks for type of constructors with elements and random sequence order
	composed_element_type molecule2(elements);
	
	composed_element_type molecule2_equal(elements);

	CPPUNIT_ASSERT(molecule2 == molecule2_equal);		

}

void ComposedElementTest::testOperatorNotEqual() {

	// initializes alphabet	
	Alphabet cho;
	cho.push_back(*hydrogen);
	cho.push_back(*carbon);
	cho.push_back(*oxygen);

	// initializes elements
	Element elementH(*hydrogen);
	Element elementO(*oxygen);
	Element elementC(*carbon);
	
	elements_container elements;
	elements[elementH] = 4;
	elements[elementO] = 2;
	elements[elementC] = 2;
			
	// checks case with different elements
	composed_element_type molecule1(elements);
	elements[elementH] = 5;
	composed_element_type molecule1_not_equal(elements);
	
	CPPUNIT_ASSERT(molecule1 != molecule1_not_equal);
		
	// checks case with different sequences
	composed_element_type molecule3("C2H4O2", cho),
						molecule3_not_equal("C2H3O2", cho);
	CPPUNIT_ASSERT(molecule3 != molecule3_not_equal);
		
}

void ComposedElementTest::testOperatorMinus() {

	// initializes alphabet	
	Alphabet cho;
	cho.push_back(*hydrogen);
	cho.push_back(*carbon);
	cho.push_back(*oxygen);
		
	composed_element_type molecule1("H10O6", cho), molecule2("O4H4", cho);

	molecule1 -= molecule2;

	CPPUNIT_ASSERT_EQUAL(molecule1.getElements().size(), 
				static_cast<elements_container::size_type>(2));
	CPPUNIT_ASSERT_EQUAL(molecule1.getElementAbundance("O"), 
				static_cast<elements_container::mapped_type>(2));
	CPPUNIT_ASSERT_EQUAL(molecule1.getElementAbundance("H"), 
				static_cast<elements_container::mapped_type>(6));
	
	composed_element_type molecule3("O8H2", cho), molecule4("O6H7C5", cho);

	molecule3 -= molecule4;

	CPPUNIT_ASSERT_EQUAL(molecule3.getElements().size(), 
				static_cast<elements_container::size_type>(1));
	CPPUNIT_ASSERT_EQUAL(molecule3.getElementAbundance("O"), 
				static_cast<elements_container::mapped_type>(2));
}

void ComposedElementTest::testOperatorPlus() {

	// initializes alphabet	
	Alphabet cho;
	cho.push_back(*hydrogen);
	cho.push_back(*carbon);
	cho.push_back(*oxygen);
		
	composed_element_type molecule3("O8H2", cho), molecule4("O6H7C5", cho);

	molecule3 += molecule4;
	CPPUNIT_ASSERT_EQUAL(molecule3.getElements().size(), 
				static_cast<elements_container::size_type>(3));
	CPPUNIT_ASSERT_EQUAL(molecule3.getElementAbundance("O"), 
				static_cast<elements_container::mapped_type>(14));
	CPPUNIT_ASSERT_EQUAL(molecule3.getElementAbundance("H"), 
				static_cast<elements_container::mapped_type>(9));
	CPPUNIT_ASSERT_EQUAL(molecule3.getElementAbundance("C"), 
				static_cast<elements_container::mapped_type>(5));

}
