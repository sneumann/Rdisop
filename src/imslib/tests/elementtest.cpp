/**
 * elementtest.cpp
 * 
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <ims/element.h>

using namespace ims;

class ElementTest : public CppUnit::TestFixture {
	typedef Element element_type;
	typedef element_type::name_type name_type;
	typedef element_type::isotopes_type isotopes_type;
	typedef isotopes_type::mass_type mass_type;
	typedef isotopes_type::abundance_type abundance_type;
	typedef isotopes_type::nominal_mass_type nominal_mass_type;	
	typedef isotopes_type::peaks_container peaks_container;
	
	CPPUNIT_TEST_SUITE(ElementTest);
	CPPUNIT_TEST(testConstructorNameIsotopeDistribution);
	CPPUNIT_TEST(testConstructorNameMass);
	CPPUNIT_TEST(testConstructorNameNominalMass);
	CPPUNIT_TEST(testCopyConstructor);
	CPPUNIT_TEST(testGetSequence);
	CPPUNIT_TEST(testSetSequence);
	CPPUNIT_TEST(testGetName);
	CPPUNIT_TEST(testSetName);
	CPPUNIT_TEST(testGetIsotopeDistribution);
	CPPUNIT_TEST(testSetIsotopeDistribution);
	CPPUNIT_TEST(testGetMass);
	CPPUNIT_TEST(testGetNominalMass);
	CPPUNIT_TEST(testGetAverageMass);
	CPPUNIT_TEST(testOperatorAssign);
	CPPUNIT_TEST(testOperatorEqual);	
	CPPUNIT_TEST(testOperatorNotEqual);								
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp() ;
		void tearDown() ;
		void testConstructorNameIsotopeDistribution();
		void testConstructorNameMass();
		void testConstructorNameNominalMass();
		void testCopyConstructor();
		void testGetSequence();
		void testSetSequence();
		void testGetName();
		void testSetName();
		void testGetIsotopeDistribution();
		void testSetIsotopeDistribution();
		void testGetMass();
		void testGetNominalMass();
		void testGetAverageMass();
		void testOperatorAssign();
		void testOperatorEqual();
		void testOperatorNotEqual();
};


CPPUNIT_TEST_SUITE_REGISTRATION(ElementTest);


void ElementTest::setUp() {
}


void ElementTest::tearDown() {	
}


void ElementTest::testConstructorNameIsotopeDistribution() {
	
	name_type name = "H";

	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	
	element_type e(name, distributionH);

	CPPUNIT_ASSERT_EQUAL(e.getName(), name);
	CPPUNIT_ASSERT_EQUAL(e.getSequence(), name);	
	CPPUNIT_ASSERT_EQUAL(e.getIsotopeDistribution(), distributionH);
	
}


void ElementTest::testConstructorNameMass() {
	
	name_type name = "A";
	mass_type mass = 1.1;
	
	element_type e(name, mass);

	CPPUNIT_ASSERT_EQUAL(e.getName(), name);
	CPPUNIT_ASSERT_EQUAL(e.getMass(), mass);
	
}


void ElementTest::testConstructorNameNominalMass() {
	
	name_type name = "A";
	nominal_mass_type nominal_mass = 1;
	
	element_type e(name, nominal_mass);

	CPPUNIT_ASSERT_EQUAL(e.getName(), name);
	CPPUNIT_ASSERT_EQUAL(e.getSequence(), name);
	CPPUNIT_ASSERT_EQUAL(e.getNominalMass(), nominal_mass);
	
}

void ElementTest::testCopyConstructor() {
	
	// checks for type of constructors with isotope distribution
	name_type name = "H";

	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
		
	element_type e1(name, distributionH);

	element_type e1_copy(e1);
	
	CPPUNIT_ASSERT_EQUAL(e1_copy.getName(), name);
	CPPUNIT_ASSERT_EQUAL(e1_copy.getSequence(), name);	
	CPPUNIT_ASSERT_EQUAL(e1_copy.getIsotopeDistribution(), distributionH);
	
	// checks for type of constructors with mass
	mass_type mass_e2 = 1.1;
	
	element_type e2(name, mass_e2);
	
	element_type e2_copy(e2);

	CPPUNIT_ASSERT_EQUAL(e2_copy.getName(), name);
	CPPUNIT_ASSERT_EQUAL(e2_copy.getSequence(), name);	
	CPPUNIT_ASSERT_EQUAL(e2_copy.getMass(), mass_e2);

	// checks for type of constructors with nominal mass
	nominal_mass_type nominal_mass_e3 = 1;
	
	element_type e3(name, nominal_mass_e3);
	
	element_type e3_copy(e3);

	CPPUNIT_ASSERT_EQUAL(e3_copy.getName(), name);
	CPPUNIT_ASSERT_EQUAL(e3_copy.getSequence(), name);
	CPPUNIT_ASSERT_EQUAL(e3_copy.getNominalMass(), nominal_mass_e3);	
}


void ElementTest::testGetSequence() {
	name_type name("Dummy");
	
	element_type e(name);
	
	CPPUNIT_ASSERT_EQUAL(e.getSequence(), name);
}


void ElementTest::testSetSequence() {
	element_type e;
	e.setSequence("Dummy");
	
	CPPUNIT_ASSERT_EQUAL(e.getSequence(), static_cast<name_type>("Dummy"));
}


void ElementTest::testGetName() {
	name_type name("Dummy");
	
	element_type e(name);
	
	CPPUNIT_ASSERT_EQUAL(e.getName(), name);	
}


void ElementTest::testSetName() {
	element_type e;
	e.setName("Dummy");
	
	CPPUNIT_ASSERT_EQUAL(e.getName(), static_cast<name_type>("Dummy"));
}


void ElementTest::testGetIsotopeDistribution() {
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	
	element_type e("Dummy", distributionH);
	
	CPPUNIT_ASSERT_EQUAL(e.getIsotopeDistribution(), distributionH);
}


void ElementTest::testSetIsotopeDistribution() {
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);

	element_type e;
	e.setIsotopeDistribution(distributionH);	
	
	CPPUNIT_ASSERT_EQUAL(e.getIsotopeDistribution(), distributionH);
}


void ElementTest::testGetMass() {
	// checks case when it is set explicitly in constructor
	mass_type mass = 1.1;
	element_type e1("Dummy", mass);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL(e1.getMass(), mass, 1.0e-8);
	
	// checks case when it is set through isotope distribution
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	
	element_type e2("Dummy", distributionH);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(e2.getMass(), 1.007825, 1.0e-8);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(e2.getMass(0), 1.007825, 1.0e-8);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(e2.getMass(1), 2.014102, 1.0e-8);
}


void ElementTest::testGetNominalMass() {
	// checks case when it is set explicitly in constructor
	nominal_mass_type nominal_mass = 1;
	element_type e1("Dummy", nominal_mass);
	
	CPPUNIT_ASSERT_EQUAL(e1.getNominalMass(), nominal_mass);
	
	// checks case when it is set through isotope distribution
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	
	element_type e2("Dummy", distributionH);

	CPPUNIT_ASSERT_EQUAL(e2.getNominalMass(), massH);		
}


void ElementTest::testGetAverageMass() {
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	
	element_type e("Dummy", distributionH);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL(e.getAverageMass(), 
			1.007825 * 0.99985 + 2.014102 * 0.00015,  1.0e-8);
}


void ElementTest::testOperatorAssign() {
	// checks for type of constructors with isotope distribution
	name_type name = "H";
	
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	
	element_type e1(name, distributionH);

	element_type e1_assigned = e1;
	
	CPPUNIT_ASSERT_EQUAL(e1_assigned.getName(), name);	
	CPPUNIT_ASSERT_EQUAL(e1_assigned.getSequence(), name);
	CPPUNIT_ASSERT_EQUAL(e1_assigned.getIsotopeDistribution(), distributionH);
	
	// checks for type of constructors with mass
	mass_type mass_e2 = 1.1;
	
	element_type e2(name, mass_e2);
	
	element_type e2_assigned = e2;

	CPPUNIT_ASSERT_EQUAL(e2_assigned.getName(), name);
	CPPUNIT_ASSERT_EQUAL(e2_assigned.getSequence(), name);
	CPPUNIT_ASSERT_EQUAL(e2_assigned.getMass(), mass_e2);

	// checks for type of constructors with nominal mass
	nominal_mass_type nominal_mass_e3 = 1;
	
	element_type e3(name, nominal_mass_e3);
	
	element_type e3_assigned = e3;

	CPPUNIT_ASSERT_EQUAL(e3_assigned.getName(), name);
	CPPUNIT_ASSERT_EQUAL(e3_assigned.getSequence(), name);
	CPPUNIT_ASSERT_EQUAL(e3_assigned.getNominalMass(), nominal_mass_e3);	
}


void ElementTest::testOperatorEqual() {
	// checks for type of constructors with isotope distribution
	name_type name = "H";

	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	
	element_type e1(name, distributionH);
	element_type e1_equal(name, distributionH);	

	CPPUNIT_ASSERT(e1 == e1_equal);	
	
	// checks for type of constructors with mass
	mass_type mass_e2 = 1.1;
	
	element_type e2(name, mass_e2);
	element_type e2_equal(name, mass_e2);

	CPPUNIT_ASSERT(e2 == e2_equal);		

	// checks for type of constructors with nominal mass
	nominal_mass_type nominal_mass_e3 = 1;
	
	element_type e3(name, nominal_mass_e3);
	element_type e3_equal(name, nominal_mass_e3);

	CPPUNIT_ASSERT(e3 == e3_equal);
}

void ElementTest::testOperatorNotEqual() {
	// checks case with different names
	element_type e1("H"), e1_not_equal("O");
	CPPUNIT_ASSERT(e1 != e1_not_equal);
		
	// checks case with different isotope distributions
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	isotopes_type distributionH(peaksH, massH);
	isotopes_type another_distributionH(peaksH, massH+1);
	
	element_type e3("H", distributionH), 
				 e3_not_equal("H", another_distributionH);
				 
	CPPUNIT_ASSERT(e3 != e3_not_equal);
}
