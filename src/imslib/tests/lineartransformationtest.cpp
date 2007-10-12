#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/transformation.h>

class LinearTransformationTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LinearTransformationTest );
	CPPUNIT_TEST( testTransform );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testTransform();
};

CPPUNIT_TEST_SUITE_REGISTRATION( LinearTransformationTest );


void LinearTransformationTest::setUp() {
}


void LinearTransformationTest::tearDown() {
}



void LinearTransformationTest::testTransform() {
	ims::LinearTransformation lt0(1.05, 82.9);
		
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.05*1.0 + 82.9, lt0.transform(1.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.05*-11.2 + 82.9, lt0.transform(-11.2), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.05*2e8 + 82.9, lt0.transform(2e8), 1.0e-10);
	
	ims::LinearTransformation lt1(-66.1,-51.0);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -66.1*81.7 - 51.0, lt1.transform(81.7), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -66.1*7.0 - 51.0, lt1.transform(7.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -66.1*-15.123 - 51.0, lt1.transform(-15.123), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -51.0, lt1.transform(0.0), 1.0e-10);
}
