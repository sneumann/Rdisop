#include <vector>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/weights.h>

class WeightsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( WeightsTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testDivideByGCD );
	CPPUNIT_TEST_SUITE_END();

public:
//	void setUp();
//	void tearDown();
	void testConstructor();
	void testDivideByGCD();
};

CPPUNIT_TEST_SUITE_REGISTRATION( WeightsTest );

void WeightsTest::testConstructor() {
	std::vector<double> masses;
	masses.push_back(3.0);
	masses.push_back(5.0);
	masses.push_back(8.0);
	ims::Weights weights(masses, 0.1);
	CPPUNIT_ASSERT_EQUAL((size_t)3, weights.size());
	CPPUNIT_ASSERT_EQUAL((ims::Weights::weight_type)30, weights[0]);
	CPPUNIT_ASSERT_EQUAL((ims::Weights::weight_type)50, weights[1]);
	CPPUNIT_ASSERT_EQUAL((ims::Weights::weight_type)80, weights[2]);
}


void WeightsTest::testDivideByGCD() {
	std::vector<double> masses;
	masses.push_back(3.0);
	masses.push_back(5.0);
	masses.push_back(8.0);
	ims::Weights weights(masses, 0.1);

	CPPUNIT_ASSERT(weights.divideByGCD());
	CPPUNIT_ASSERT_EQUAL((ims::Weights::weight_type)3, weights[0]);
	CPPUNIT_ASSERT_EQUAL((ims::Weights::weight_type)5, weights[1]);
	CPPUNIT_ASSERT_EQUAL((ims::Weights::weight_type)8, weights[2]);
}
