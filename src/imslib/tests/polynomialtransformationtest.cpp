#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <memory>

#include <ims/transformation.h>

class PolynomialTransformationTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PolynomialTransformationTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testTransform );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testTransform();
};

CPPUNIT_TEST_SUITE_REGISTRATION( PolynomialTransformationTest );


void PolynomialTransformationTest::setUp() {
}


void PolynomialTransformationTest::tearDown() {
}


void PolynomialTransformationTest::testConstructor() {
}


void PolynomialTransformationTest::testTransform() {
	ims::PolynomialTransformation pt0(0);
	pt0.setCoefficient(0, 5.0);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, pt0.transform(0.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, pt0.transform(1.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, pt0.transform(100.0), 1.0e-10);
	
	ims::PolynomialTransformation pt1(1);
	pt1.setCoefficient(0, 9.0);
	pt1.setCoefficient(1, 0.5);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, pt1.transform(0.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5*1.0 + 9.0, pt1.transform(1.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5*100.0 + 9.0, pt1.transform(100.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5*(-20.0) + 9.0, pt1.transform(-20.0), 1.0e-10);

	ims::PolynomialTransformation pt2(2);
	pt2.setCoefficient(0, 9.0);
	pt2.setCoefficient(1, 0.5);
	pt2.setCoefficient(2, 2.0);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, pt2.transform(0.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0*1.0 + 0.5*1.0 + 9.0, pt2.transform(1.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 100.0*100.0*2.0 + 0.5*100.0 + 9.0, pt2.transform(100.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( (-20.0)*(-20.0)*2.0 + 0.5*(-20.0) + 9.0, pt2.transform(-20.0), 1.0e-10);
	
	ims::PolynomialTransformation pt3(3);
	pt3.setCoefficient(0, 9.0);
	pt3.setCoefficient(1, 0.5);
	pt3.setCoefficient(2, 2.789);
	pt3.setCoefficient(3, 6.3);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, pt3.transform(0.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.3 + 2.789 + 0.5 + 9.0, pt3.transform(1.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1e6*6.3 + 100.0*100.0*2.789 + 0.5*100.0 + 9.0, pt3.transform(100.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( (-20.0)*(-20.0)*(-20.0)*6.3 + (-20.0)*(-20.0)*2.789 + 0.5*(-20.0) + 9.0, pt3.transform(-20.0), 1.0e-10);
	
}
