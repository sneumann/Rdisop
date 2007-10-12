#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/utils/math.h>


class RoundTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( RoundTest );
	CPPUNIT_TEST( testRound );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp() {};
	void tearRound() {};
	void testRound();
};

CPPUNIT_TEST_SUITE_REGISTRATION( RoundTest );

void RoundTest::testRound() {
	CPPUNIT_ASSERT_EQUAL(2, (int)round(2.499));
	CPPUNIT_ASSERT_EQUAL(3, (int)round(2.5));
	CPPUNIT_ASSERT_EQUAL(279, (int)round(279.0));
	CPPUNIT_ASSERT_EQUAL(90343, (int)round(90342.789));
	CPPUNIT_ASSERT_EQUAL(-7, (int)round(-7.0));
	CPPUNIT_ASSERT_EQUAL(-7, (int)round(-7.499));
	CPPUNIT_ASSERT_EQUAL(-8, (int)round(-7.5));
	CPPUNIT_ASSERT_EQUAL(-8, (int)round(-7.9));
}
