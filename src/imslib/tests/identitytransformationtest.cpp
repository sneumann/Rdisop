#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/transformation.h>

class IdentityTransformationTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( IdentityTransformationTest );
	CPPUNIT_TEST( testTransform );
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testTransform();
};

CPPUNIT_TEST_SUITE_REGISTRATION( IdentityTransformationTest );


void IdentityTransformationTest::setUp() {
}


void IdentityTransformationTest::tearDown() {
}

void IdentityTransformationTest::testTransform() {
	ims::IdentityTransformation id;
		
	CPPUNIT_ASSERT_EQUAL(5.0, id.transform(5.0));
	CPPUNIT_ASSERT_EQUAL(1298.34, id.transform(1298.34));
	CPPUNIT_ASSERT_EQUAL(0.0, id.transform(0.0));
	CPPUNIT_ASSERT_EQUAL(-34.06e7, id.transform(-34.06e7));
	CPPUNIT_ASSERT_EQUAL(-23.0, id.transform(-23.0));
}

/* test operator<< (tests only compilation) */
/*
void IdentityTransformationTest::testOutput() {
	std::ostringstream oss;
	ims::IdentityTransformation id;
	std::unique_ptr<ims::Transformation> idp(new ims::IdentityTransformation);
	oss << id;
	oss << *((ims::IdentityTransformation*)(idp.get())); // TODO hmm ...
	cout << oss;
}
*/
