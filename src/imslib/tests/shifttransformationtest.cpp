#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/transformation.h>

class ShiftTransformationTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ShiftTransformationTest );
	CPPUNIT_TEST( testTransform );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testTransform();
};

CPPUNIT_TEST_SUITE_REGISTRATION( ShiftTransformationTest );


void ShiftTransformationTest::setUp() {
}


void ShiftTransformationTest::tearDown() {
}



void ShiftTransformationTest::testTransform() {
	ims::ShiftTransformation st0(82.9);
		
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0 + 82.9, st0.transform(1.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -11.2 + 82.9, st0.transform(-11.2), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 2e8 + 82.9, st0.transform(2e8), 1.0e-10);
	
	ims::ShiftTransformation st1(-51.0);
	
	CPPUNIT_ASSERT_DOUBLES_EQUAL(81.7 - 51.0, st1.transform(81.7), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0 - 51.0, st1.transform(7.0), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-15.123 - 51.0, st1.transform(-15.123), 1.0e-10);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-51.0, st1.transform(0.0), 1.0e-10);
}
