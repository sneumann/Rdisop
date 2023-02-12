/**
 * matchmatrixtest.cpp
 *
 * Author: Tobias Marschall <Tobias.Marschall@CeBiTec.Uni-Bielefeld.DE>
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <map>

#include <ims/calib/matchmatrix.h>

using namespace std;

class MatchMatrixTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( MatchMatrixTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testMatchMatrix );
	CPPUNIT_TEST_EXCEPTION( testBadMatrix1Throws, ims::InvalidMatchMatrix );
	CPPUNIT_TEST_EXCEPTION( testBadMatrix2Throws, ims::InvalidMatchMatrix );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testMatchMatrix();

	// The next two intentionally violate the staircase property
	void testBadMatrix1Throws();
	void testBadMatrix2Throws();
};

CPPUNIT_TEST_SUITE_REGISTRATION( MatchMatrixTest );

void MatchMatrixTest::setUp() {
}

void MatchMatrixTest::tearDown() {
}


void MatchMatrixTest::testConstructor() {
	ims::MatchMatrix mm(23);
	CPPUNIT_ASSERT_EQUAL((size_t)23, mm.getRows());
}

void MatchMatrixTest::testMatchMatrix() {
	ims::MatchMatrix mm(8);

	mm.set(1,0);
	mm.set(1,1);
	mm.set(1,2);
	mm.set(1,3);
	mm.set(3,3);
	mm.set(3,4);
	mm.set(3,5);
	mm.set(4,3);
	mm.set(4,4);
	mm.set(4,5);
	mm.set(5,5);
	mm.set(6,6);
	mm.set(6,7);

	mm.unset(1,0);
	mm.unset(1,3);

	
	unique_ptr<std::map<int,int> > m1 = mm.countMatches();
	CPPUNIT_ASSERT_EQUAL((size_t)5, m1->size());
	CPPUNIT_ASSERT_EQUAL(1, (*m1)[1]);
	CPPUNIT_ASSERT_EQUAL(3, (*m1)[3]);
	CPPUNIT_ASSERT_EQUAL(4, (*m1)[4]);
	CPPUNIT_ASSERT_EQUAL(5, (*m1)[5]);
	CPPUNIT_ASSERT_EQUAL(6, (*m1)[6]);

	unique_ptr<std::map<int,int> > m2 = mm.countMatchesRestrictive();
	CPPUNIT_ASSERT_EQUAL((size_t)1, m2->size());
	CPPUNIT_ASSERT_EQUAL(5, (*m2)[5]);
}

void MatchMatrixTest::testBadMatrix1Throws() {
	ims::MatchMatrix mm(8);
	mm.set(4,3);
	mm.set(4,5);	// <-- Exception should be thrown
}

void MatchMatrixTest::testBadMatrix2Throws() {
	ims::MatchMatrix mm(4);
	mm.set(2,2);
	mm.set(2,3);
	mm.set(2,4);
	mm.set(2,5);
	mm.unset(2,4);	// <-- Exception should be thrown
}
