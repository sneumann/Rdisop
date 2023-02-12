/* TODO: This test should perhaps have a dependency on PolynomialTransformationTest */

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <memory>
#include <cmath>
//#include <iostream>
//#include <iomanip>

#include <ims/chebyshevfitter.h>
#include <ims/transformation.h>

class ChebyshevFitterTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ChebyshevFitterTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testFit );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testFit();
};

CPPUNIT_TEST_SUITE_REGISTRATION( ChebyshevFitterTest );

void ChebyshevFitterTest::setUp() {
}


void ChebyshevFitterTest::tearDown() {
}


void ChebyshevFitterTest::testConstructor() {
	ims::ChebyshevFitter fitter(2);
	CPPUNIT_ASSERT_EQUAL((size_t)2, fitter.getDegree());
}


void ChebyshevFitterTest::testFit() {
	ims::ChebyshevFitter fitter2(2);
	std::vector<double> a;
	std::vector<double> b;
	
	/* test identity transformation */
	a.push_back(-31.7);  b.push_back(-31.7);
	a.push_back(0.0);    b.push_back(0.0);
	a.push_back(2.0);    b.push_back(2.0);
	a.push_back(17.0);   b.push_back(17.0);
	a.push_back(22.0);   b.push_back(22.0);
	a.push_back(500.0);  b.push_back(500.0);
	a.push_back(2000.0); b.push_back(2000.0);
	a.push_back(3000.0); b.push_back(3000.0);
	
	std::unique_ptr<ims::PolynomialTransformation> pt1 = fitter2.fit(a.begin(), a.end(), b.begin(), b.end());
	
	std::vector<double>::const_iterator cit;
	for (cit = a.begin(); cit != a.end(); ++cit) {
		// TODO: it seems that CPPUNIT_ASSERT_DOUBLES_EQUAL does not fail if one of the doubles is NaN
		CPPUNIT_ASSERT_DOUBLES_EQUAL( *cit, pt1->transform(*cit), 1.0e-10 );
	}
	
	/* test a different transformation */
	// create a transformation
	ims::PolynomialTransformation pt_tmp(5);
	pt_tmp.setCoefficient(0, 3.7);
	pt_tmp.setCoefficient(1, 2.529);
	pt_tmp.setCoefficient(2, -19.3);
	pt_tmp.setCoefficient(3, 1409.7);
	pt_tmp.setCoefficient(4, 13.62);
	pt_tmp.setCoefficient(5, -7.0);
	
	// transform a to b using pt_tmp
	b.clear();
	//std::vector<double>::iterator it;
	for (cit = a.begin(); cit != a.end(); ++cit) {
		b.push_back(pt_tmp.transform(*cit));
	}
	
	// estimate 5th degree polynomial
	ims::ChebyshevFitter fitter5(5);
	std::unique_ptr<ims::PolynomialTransformation> pt2 = fitter5.fit(a.begin(), a.end(), b.begin(), b.end());
	
	// ... and compare to original
	for (size_t i = 0; i <= 5; ++i) {
		CPPUNIT_ASSERT( !isnan(pt2->getCoefficient(i)) );
		// leave 1.0e-5 in here, 1.0e-8 is too strict
		CPPUNIT_ASSERT_DOUBLES_EQUAL( pt_tmp.getCoefficient(i), pt2->getCoefficient(i), 1.0e-5 );
	}
}
