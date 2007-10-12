/**
 * isotopespeciestest.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/isotopespecies.h>

using namespace ims;

class IsotopeSpeciesTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE( IsotopeSpeciesTest );
		CPPUNIT_TEST( testFold );
		CPPUNIT_TEST_SUITE_END();
		
		typedef IsotopeSpecies isotopes_type;
		typedef isotopes_type::peaks_container peaks_container;
		typedef isotopes_type::mass_type mass_type;
		typedef isotopes_type::abundance_type abundance_type;

		isotopes_type *distributionH;
		isotopes_type *distributionO;
		isotopes_type *distributionC;
		isotopes_type *distributionN;
		isotopes_type *distributionP;
		isotopes_type *distributionS;		
	public:
		void setUp();
		void testFold();
		void tearDown();
};

CPPUNIT_TEST_SUITE_REGISTRATION(IsotopeSpeciesTest);

void IsotopeSpeciesTest::setUp() {
	peaks_container containerH;
	containerH.push_back(peaks_container::value_type(1.007825, 0.99985));
	containerH.push_back(peaks_container::value_type(2.014102, 0.00015));
	distributionH = new isotopes_type(containerH);
	
	peaks_container containerO;
	containerO.push_back(peaks_container::value_type(15.994915, 0.99762));
	containerO.push_back(peaks_container::value_type(16.999132, 0.00038));
	containerO.push_back(peaks_container::value_type(17.999161, 0.002));
	distributionO = new isotopes_type(containerO);
		
	peaks_container containerC;
	containerC.push_back(peaks_container::value_type(12.0, 0.9889));
	containerC.push_back(peaks_container::value_type(13.003355, 0.0111));
	distributionC = new isotopes_type(containerC);	
	
	peaks_container containerN;
	containerN.push_back(peaks_container::value_type(14.003074, 0.99634));
	containerN.push_back(peaks_container::value_type(15.000109, 0.00366));
	distributionN = new isotopes_type(containerN);	

	peaks_container containerS;
	containerS.push_back(peaks_container::value_type(31.972071, 0.9502));
	containerS.push_back(peaks_container::value_type(32.971459, 0.0075));
	containerS.push_back(peaks_container::value_type(33.967867, 0.0421));
	containerS.push_back(peaks_container::value_type(35.967081, 0.0002));
	distributionS = new isotopes_type(containerS);	
	
	peaks_container containerP;
	containerP.push_back(peaks_container::value_type(30.973762, 1.0));
	distributionP = new isotopes_type(containerP);	
}

void IsotopeSpeciesTest::tearDown() {
	delete distributionP;
	delete distributionS;
	delete distributionN;
	delete distributionC;
	delete distributionO;
	delete distributionH;
}

void IsotopeSpeciesTest::testFold() {

	isotopes_type isodistr(*distributionC);
	isodistr *= 10;

	isotopes_type elementsH(*distributionH);
	elementsH *= 16;	
	isodistr *= elementsH;

	isotopes_type elementsN(*distributionN);
	elementsN *= 5;
	isodistr *= elementsN;

	isotopes_type elementsO(*distributionO);
	elementsO *= 13;
	isodistr *= elementsO;

	isotopes_type elementsP(*distributionP);
	elementsP *= 3;
	isodistr *= elementsP;

	isodistr.filterAbundances(0.000001);

	isodistr.sortByMass();
// atp isotope species
//506.995751 0.84931
//507.992786 0.0155995
//507.999106 0.0953316
//507.999968 0.0042056
//508.002028 0.00203865
//508.989821 0.000114608
//508.996141 0.00175098
//508.997003 7.72452e-05
//508.999063 3.74443e-05
//508.999997 0.0221347
//509.002461 0.00481526
//509.003323 0.000472061
//509.004185 9.61164e-06
//509.005383 0.00022883
//509.006245 1.0095e-05
//509.008305 2.29382e-06
//509.993176 1.28642e-05
//509.997032 0.000406554
//509.999496 8.8443e-05
//510.000358 8.67046e-06
//510.002418 4.20297e-06
//510.003352 0.00248453
//510.004214 0.000101175
//510.005816 0.000144132
//510.006274 5.31313e-05
//510.006678 2.38441e-05
//510.00754 1.07887e-06
//510.008738 1.15584e-05
//510.0096 1.13312e-06
//510.994067 2.9869e-06
//511.000387 4.5634e-05
//511.001249 1.85831e-06
//511.002851 2.6473e-06
//511.004243 0.00026625
//511.006707 0.000125495
//511.007569 1.13565e-05
//511.009171 2.83118e-06
//511.009629 5.96378e-06
//512.001278 4.89028e-06
//512.003742 2.305e-06
//512.007598 2.98855e-05
//512.00846 1.11558e-06
//512.010062 3.75636e-06
//513.008489 1.95716e-06
//513.010953 1.50954e-06
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(0), static_cast<mass_type>(506.995751), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(1), static_cast<mass_type>(507.992786), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(2), static_cast<mass_type>(507.999106), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(3), static_cast<mass_type>(507.999968), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(4), static_cast<mass_type>(508.002028), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(5), static_cast<mass_type>(508.989821), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(6), static_cast<mass_type>(508.996141), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(7), static_cast<mass_type>(508.997003), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(8), static_cast<mass_type>(508.999063), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(9), static_cast<mass_type>(508.999997), 1.0e-6);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(10), static_cast<mass_type>(509.002461), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(11), static_cast<mass_type>(509.003323), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(12), static_cast<mass_type>(509.004185), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(13), static_cast<mass_type>(509.005383), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(14), static_cast<mass_type>(509.006245), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(15), static_cast<mass_type>(509.008305), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(16), static_cast<mass_type>(509.993176), 1.0e-6);	
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(17), static_cast<mass_type>(509.997032), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(18), static_cast<mass_type>(509.999496), 1.0e-6);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(19), static_cast<mass_type>(510.000358), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getMass(20), static_cast<mass_type>(510.002418), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(0), static_cast<abundance_type>(0.84931), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(1), static_cast<abundance_type>(0.0155995), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(2), static_cast<abundance_type>(0.0953316), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(3), static_cast<abundance_type>(0.0042056), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(4), static_cast<abundance_type>(0.00203865), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(5), static_cast<abundance_type>(0.000114608), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(6), static_cast<abundance_type>(0.00175098), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(7), static_cast<abundance_type>(7.72452e-05), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(8), static_cast<abundance_type>(3.74443e-05), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(9), static_cast<abundance_type>(0.0221347), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(10), static_cast<abundance_type>(0.00481526), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(11), static_cast<abundance_type>(0.000472061), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(12), static_cast<abundance_type>(9.61164e-06), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(13), static_cast<abundance_type>(0.00022883), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(14), static_cast<abundance_type>(1.0095e-05), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(15), static_cast<abundance_type>(2.29382e-06), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(16), static_cast<abundance_type>(1.28642e-05), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(17), static_cast<abundance_type>(0.000406554), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(18), static_cast<abundance_type>(8.8443e-05), 1.0e-6);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(19), static_cast<abundance_type>(8.67046e-06), 1.0e-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(isodistr.getAbundance(20), static_cast<abundance_type>(4.20297e-06), 1.0e-6);
	
}

