/**
 * isotopedistributiontest.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 *
 */


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/isotopedistribution.h>

using namespace ims;

class IsotopeDistributionTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE( IsotopeDistributionTest );
		CPPUNIT_TEST( testConstructor );
		CPPUNIT_TEST_SUITE_END();
	public:
		void setUp();
		void testConstructor();
		void tearDown();
};

CPPUNIT_TEST_SUITE_REGISTRATION(IsotopeDistributionTest);

void IsotopeDistributionTest::setUp() {
	IsotopeDistribution::SIZE = 10;
	IsotopeDistribution::ABUNDANCES_SUM_ERROR = 0.0001;
}

void IsotopeDistributionTest::tearDown() {
}

void IsotopeDistributionTest::testConstructor() {
	typedef IsotopeDistribution::peaks_container peaks_container;
	typedef IsotopeDistribution::mass_type mass_type;
	typedef IsotopeDistribution::abundance_type abundance_type;
	typedef IsotopeDistribution::nominal_mass_type nominal_mass_type;	

	abundance_type abundance0 = 0.99762, abundance1 = 0.00038, abundance2 = 0.002;
	mass_type massShift0 = -0.005085, massShift1 = -0.000869, massShift2 = -0.000839;
	
// Oxygen		
	nominal_mass_type massO = 16;
	peaks_container peaksO;
	peaksO.push_back(peaks_container::value_type(massShift0, abundance0));
	peaksO.push_back(peaks_container::value_type(massShift1, abundance1));
	peaksO.push_back(peaks_container::value_type(massShift2, abundance2));
	
	IsotopeDistribution distributionO(peaksO, massO);
	
	CPPUNIT_ASSERT_EQUAL(massO, distributionO.getNominalMass());

	CPPUNIT_ASSERT_DOUBLES_EQUAL(distributionO.getAbundance(0), abundance0, 1.0e-5);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(distributionO.getAbundance(1), abundance1, 1.0e-5);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(distributionO.getAbundance(2), abundance2, 1.0e-5);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(distributionO.getMass(0), massO + massShift0, 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(distributionO.getMass(1), massO + massShift1 + 1, 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(distributionO.getMass(2), massO + massShift2 + 2, 1.0e-6);
}

