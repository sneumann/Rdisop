/**
 * linearpointsetmatchertest.cpp
 *
 * Author: Tobias Marschall <Tobias.Marschall@CeBiTec.Uni-Bielefeld.DE>
 *
 */

// TODO: how to do logging right in a testcase? 
// TODO: a log that's only printed if the test fails would be nice 

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <vector>
#include <map>
#include <cmath>
#include <limits>
//#include <iostream>

#include <ims/calib/linearpointsetmatcher.h>
#include <ims/transformation.h>
#include <ims/logger.h>

using namespace std;

class LinearPointSetMatcherTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LinearPointSetMatcherTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testMatchingOneToOne1 );
	CPPUNIT_TEST( testMatchingOneToOne2 );
	CPPUNIT_TEST( testMatchingOneToOne3 );
	CPPUNIT_TEST( testMatchingOneToOne4 );
	CPPUNIT_TEST( testMatchingOneToOne5 );
	CPPUNIT_TEST( testMatchingOneToOne6 );	
	CPPUNIT_TEST( testMatchingManyToOne1 );
	CPPUNIT_TEST( testMatchingManyToOne2 );
	CPPUNIT_TEST( testMatchingManyToOne3 );
	CPPUNIT_TEST( testMatchingManyToOne4 );
	CPPUNIT_TEST_SUITE_END();

private:
	ims::Logger logger;
	vector<double> pointsets[8];

	void matchAndVerify(ims::LinearPointSetMatcher& lpsm, int n);
	void verifyManyToOne(const vector<double>& A, const vector<double>& B, double epsilon, const ims::Transformation& t, int score, double abslimit);
	void verifyOneToOne(const vector<double>& A, const vector<double>& B, double epsilon, const ims::Transformation& t, map<int,int>& mapping, double abslimit);

public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testMatchingOneToOne1();
	void testMatchingOneToOne2();
	void testMatchingOneToOne3();
	void testMatchingOneToOne4();
	void testMatchingOneToOne5();
	void testMatchingOneToOne6();
	void testMatchingManyToOne1();
	void testMatchingManyToOne2();
	void testMatchingManyToOne3();
	void testMatchingManyToOne4();
};

CPPUNIT_TEST_SUITE_REGISTRATION( LinearPointSetMatcherTest );

void LinearPointSetMatcherTest::setUp() {
	logger = ims::Logger(ims::Silent);

	// some peaklists (SELDI) from cvs module calib
	pointsets[0].push_back(1009.66);
	pointsets[0].push_back(1016.29);
	pointsets[0].push_back(1039.43);
	pointsets[0].push_back(1055.50);
	pointsets[0].push_back(1060.24);
	pointsets[0].push_back(1070.54);
	pointsets[0].push_back(1081.96);
	pointsets[0].push_back(1103.02);
	pointsets[0].push_back(1114.87);
	pointsets[0].push_back(1223.99);
	pointsets[0].push_back(1242.32);
	pointsets[0].push_back(1268.47);
	pointsets[0].push_back(1289.42);
	pointsets[0].push_back(1311.42);
	pointsets[0].push_back(1337.79);
	pointsets[0].push_back(1352.94);
	pointsets[0].push_back(1475.59);
	pointsets[0].push_back(1497.35);
	pointsets[0].push_back(1518.91);
	pointsets[0].push_back(1683.02);
	pointsets[0].push_back(10951.90);
	pointsets[0].push_back(13132.40);
	pointsets[0].push_back(13667.90);
	pointsets[0].push_back(16421.20);
	pointsets[0].push_back(21872.60);
	pointsets[0].push_back(26289.60);
	pointsets[0].push_back(27588.30);

	pointsets[1].push_back(1010.25);
	pointsets[1].push_back(1018.18);
	pointsets[1].push_back(1038.75);
	pointsets[1].push_back(1060.66);
	pointsets[1].push_back(1098.48);
	pointsets[1].push_back(1269.25);
	pointsets[1].push_back(3236.74);
	pointsets[1].push_back(6843.46);
	pointsets[1].push_back(9391.13);
	pointsets[1].push_back(10964.40);
	pointsets[1].push_back(13142.30);
	pointsets[1].push_back(13681.60);
	pointsets[1].push_back(16428.30);
	pointsets[1].push_back(21888.20);
	pointsets[1].push_back(26261.40);

	pointsets[2].push_back(1010.82);
	pointsets[2].push_back(1058.96);
	pointsets[2].push_back(1098.15);
	pointsets[2].push_back(1140.52);
	pointsets[2].push_back(3238.84);
	pointsets[2].push_back(4563.90);
	pointsets[2].push_back(6846.10);
	pointsets[2].push_back(9382.85);
	pointsets[2].push_back(10952.80);
	pointsets[2].push_back(13153.60);
	pointsets[2].push_back(13683.00);
	pointsets[2].push_back(16427.00);
	pointsets[2].push_back(18766.10);
	pointsets[2].push_back(21886.40);
	pointsets[2].push_back(26251.20);
	pointsets[2].push_back(27640.00);

	pointsets[3].push_back(1059.23);
	pointsets[3].push_back(6859.99);
	pointsets[3].push_back(10939.50);
	pointsets[3].push_back(13122.60);
	pointsets[3].push_back(13668.60);
	pointsets[3].push_back(16386.60);
	pointsets[3].push_back(21834.40);

	pointsets[4].push_back(1009.72);
	pointsets[4].push_back(1060.54);
	pointsets[4].push_back(1070.69);
	pointsets[4].push_back(1081.84);
	pointsets[4].push_back(1109.59);
	pointsets[4].push_back(1116.16);
	pointsets[4].push_back(1126.92);
	pointsets[4].push_back(1224.36);
	pointsets[4].push_back(1268.63);
	pointsets[4].push_back(1281.59);
	pointsets[4].push_back(1338.51);
	pointsets[4].push_back(1395.51);
	pointsets[4].push_back(1454.12);
	pointsets[4].push_back(1476.63);
	pointsets[4].push_back(1499.01);
	pointsets[4].push_back(1509.48);
	pointsets[4].push_back(1682.41);
	pointsets[4].push_back(3235.14);
	pointsets[4].push_back(6837.17);
	pointsets[4].push_back(9238.15);
	pointsets[4].push_back(10936.90);
	pointsets[4].push_back(13127.90);
	pointsets[4].push_back(13668.80);
	pointsets[4].push_back(16406.00);
	pointsets[4].push_back(18753.10);
	pointsets[4].push_back(21857.50);
	pointsets[4].push_back(26252.20);
	pointsets[4].push_back(27639.70);

	pointsets[5].push_back(1013.67);
	pointsets[5].push_back(1063.70);
	pointsets[5].push_back(1121.57);
	pointsets[5].push_back(6552.64);
	pointsets[5].push_back(6860.45);
	pointsets[5].push_back(8240.55);
	pointsets[5].push_back(9415.60);
	pointsets[5].push_back(10982.60);
	pointsets[5].push_back(13176.30);
	pointsets[5].push_back(13710.20);
	pointsets[5].push_back(16461.40);
	pointsets[5].push_back(18815.70);
	pointsets[5].push_back(21932.20);
	pointsets[5].push_back(22574.70);
	pointsets[5].push_back(25337.00);
	pointsets[5].push_back(26337.30);

	pointsets[6].push_back(1010.96);
	pointsets[6].push_back(1061.62);
	pointsets[6].push_back(1083.63);
	pointsets[6].push_back(1117.62);
	pointsets[6].push_back(1238.54);
	pointsets[6].push_back(1270.88);
	pointsets[6].push_back(3270.91);
	pointsets[6].push_back(4396.15);
	pointsets[6].push_back(4564.17);
	pointsets[6].push_back(6343.84);
	pointsets[6].push_back(6538.37);
	pointsets[6].push_back(6845.44);
	pointsets[6].push_back(8583.22);
	pointsets[6].push_back(9249.77);
	pointsets[6].push_back(9388.95);
	pointsets[6].push_back(10957.70);
	pointsets[6].push_back(13152.20);
	pointsets[6].push_back(13687.40);
	pointsets[6].push_back(16428.40);
	pointsets[6].push_back(18790.40);
	pointsets[6].push_back(21880.70);
	pointsets[6].push_back(26282.20);
	pointsets[6].push_back(27678.60);

	pointsets[7].push_back(1012.10);
	pointsets[7].push_back(1058.95);
	pointsets[7].push_back(1062.80);
	pointsets[7].push_back(1084.04);
	pointsets[7].push_back(1272.40);
	pointsets[7].push_back(4358.28);
	pointsets[7].push_back(6593.70);
	pointsets[7].push_back(6855.14);
	pointsets[7].push_back(8713.52);
	pointsets[7].push_back(13710.20);
	pointsets[7].push_back(16457.40);
	pointsets[7].push_back(21969.50);
}

void LinearPointSetMatcherTest::tearDown() {
	for (int i=0; i<=7; i++) {
		pointsets[i].clear();
	}
}

void LinearPointSetMatcherTest::testConstructor() {
	ims::LinearPointSetMatcher lpsm1(logger, 2.34, true, true);
	ims::LinearPointSetMatcher lpsm2(logger, 7.89, false, false);
	CPPUNIT_ASSERT( lpsm1.one2One() );
	CPPUNIT_ASSERT( lpsm1.restrictOne2One() );
	CPPUNIT_ASSERT_EQUAL(2.34, lpsm1.getEpsilon());
	CPPUNIT_ASSERT( !lpsm2.one2One() );
	CPPUNIT_ASSERT( !lpsm2.restrictOne2One() );
	CPPUNIT_ASSERT_EQUAL(7.89, lpsm2.getEpsilon());
	
	// test abslimit getter/setter
	CPPUNIT_ASSERT_EQUAL(std::numeric_limits<double>::infinity(), lpsm1.getAbsLimit());
	lpsm1.setAbsLimit(10.0);
	CPPUNIT_ASSERT_EQUAL(10.0, lpsm1.getAbsLimit());
	lpsm1.unsetAbsLimit();
	CPPUNIT_ASSERT_EQUAL(std::numeric_limits<double>::infinity(), lpsm1.getAbsLimit());
}

void LinearPointSetMatcherTest::testMatchingOneToOne1() {
	ims::LinearPointSetMatcher lpsm(logger, 2.7, true, false);
	// Use fewer peaklists to reduces time (algorithm consumes a lot of time without abs-/scalelimit)
	matchAndVerify(lpsm,4);
}

void LinearPointSetMatcherTest::testMatchingOneToOne2() {
	ims::LinearPointSetMatcher lpsm(logger, 8.3, true, true);
	// Use fewer peaklists to reduces time (algorithm consumes a lot of time without abs-/scalelimit)
	matchAndVerify(lpsm,4);
}

void LinearPointSetMatcherTest::testMatchingOneToOne3() {
	ims::LinearPointSetMatcher lpsm(logger, 3.0, true, false);
	lpsm.setAbsLimit(8.1);
	matchAndVerify(lpsm,8);
}

void LinearPointSetMatcherTest::testMatchingOneToOne4() {
	ims::LinearPointSetMatcher lpsm(logger, 3.6, true, false);
	lpsm.setAbsLimit(7.1);
	lpsm.setScaleInterval(0.98, 1.02);
	matchAndVerify(lpsm,8);
}

void LinearPointSetMatcherTest::testMatchingOneToOne5() {
	ims::LinearPointSetMatcher lpsm(logger, 14.3, true, true);
	lpsm.setAbsLimit(10.2);
	matchAndVerify(lpsm,8);
}

void LinearPointSetMatcherTest::testMatchingOneToOne6() {
	ims::LinearPointSetMatcher lpsm(logger, 3.6, true, false);
	lpsm.setTranslationInterval(-10.0,25.0);
	lpsm.setScaleInterval(0.4, 2.0);
	matchAndVerify(lpsm,8);
}

void LinearPointSetMatcherTest::testMatchingManyToOne1() {
	ims::LinearPointSetMatcher lpsm(logger, 2.7, false, false);
	// Use fewer peaklists to reduces time (algorithm consumes a lot of time without abs-/scalelimit)
	matchAndVerify(lpsm,4);
}

void LinearPointSetMatcherTest::testMatchingManyToOne2() {
	ims::LinearPointSetMatcher lpsm(logger, 8.3, false, false);
	lpsm.setAbsLimit(10.2);
	matchAndVerify(lpsm,8);
}

void LinearPointSetMatcherTest::testMatchingManyToOne3() {
	ims::LinearPointSetMatcher lpsm(logger, 11.1, false, false);
	lpsm.setAbsLimit(10.2);
	lpsm.setScaleInterval(0.95, 1.05);
	matchAndVerify(lpsm,8);
}

void LinearPointSetMatcherTest::testMatchingManyToOne4() {
	ims::LinearPointSetMatcher lpsm(logger, 3.5, false, false);
	lpsm.setTranslationInterval(-20.0,20.0);
	lpsm.setScaleInterval(0.6, 1.5);
	matchAndVerify(lpsm,8);
}

void LinearPointSetMatcherTest::matchAndVerify(ims::LinearPointSetMatcher& lpsm, int n) {
	const double accuracy = 0.0001;
	CPPUNIT_ASSERT(n<=8);
	// Test all sets against all others
	for (int i=0; i<n; i++) {
		for (int j=i+1; j<n; j++) {
			int score = lpsm.match(pointsets[i].begin(), pointsets[i].end(), pointsets[j].begin(), pointsets[j].end());
			ims::LinearTransformation t = lpsm.getTransformation();
			std::pair<double,double> translation_limit = lpsm.getTranslationInterval();
			std::pair<double,double> scale_limit = lpsm.getScaleInterval();
			//cout << i << "," << j << ": score="<< score <<", f(x)=x*" << t->getScale() << "+" << t->getTranslation() << endl;
			if (score!=0) {
				CPPUNIT_ASSERT(t.getScale() >= scale_limit.first - accuracy);
				CPPUNIT_ASSERT(t.getScale() <= scale_limit.second + accuracy);
				CPPUNIT_ASSERT(t.getTranslation() >= translation_limit.first - accuracy);
				CPPUNIT_ASSERT(t.getTranslation() <= translation_limit.second + accuracy);
				// in one-to-one case the verification procedure is different...
				if (lpsm.one2One()) {
					unique_ptr<map<int,int> > m = lpsm.getMapping();
					CPPUNIT_ASSERT( m.get() != 0 );
					verifyOneToOne(pointsets[i], pointsets[j], lpsm.getEpsilon(), t, *m, lpsm.getAbsLimit());
				} else {
					verifyManyToOne(pointsets[i], pointsets[j], lpsm.getEpsilon(), t, score, lpsm.getAbsLimit());
				}
			}
		}
	}
};

void LinearPointSetMatcherTest::verifyManyToOne(const vector<double>& A, const vector<double>& B, double epsilon, const ims::Transformation& t, int score, double abslimit) {
	const double accuracy = 0.0001;
	int m = A.size();
	int n = B.size();

	int sure_score = 0;
	int unsure_score = 0;

	// apply transformation to elements of set A and see how many points in B are in epsilon distance
	for (int i=0 ; i<m ; i++) {
		double a = t.transform(A[i]);
		for (int j=0 ; j<n ; j++) {

			if (abslimit > 0) {
				// don't count matches which don't respect abslimit
				if (fabs(A[i]-B[j]) > abslimit) continue;
			}
		
			double diff=fabs(a-(double)B[j]);
			// if difference is below epsilon-accuracy the match is sure
			if (diff <= epsilon-accuracy) {
				sure_score++;
				continue;
			} 
				
			// if the difference is exactly (in terms of accuracy) epsilon, the match is unsure
			if (fabs(diff-epsilon) < accuracy) {
				unsure_score++;
			}
		}
	}

	int max_valid_score = unsure_score+sure_score;
	// two points are explicitly mapped into epsilon distance by the algorithm and hence are sure matches
	int min_valid_score = min(sure_score+2, max_valid_score);

	// if score is in the interval [min_valid_score..max_valid_score] we are satisfied
	CPPUNIT_ASSERT( (score>=min_valid_score) && (score<=max_valid_score) );
}

void LinearPointSetMatcherTest::verifyOneToOne(const vector<double>& A, const vector<double>& B, double epsilon, const ims::Transformation& t, map<int,int>& mapping, double abslimit) {
	const double accuracy = 0.0001;
	map<int,int>::const_iterator iter = mapping.begin();
	// iterate over all pairs in the mapping
	for ( ; iter!=mapping.end(); iter++) {
		// apply transformation ...
		double a = t.transform(A[iter->first]);
		// ... calculate difference ...
		double diff = fabs(a-B[iter->second]);
		// ... and see if difference is small enough
		CPPUNIT_ASSERT(diff-accuracy <= epsilon);
		// if abslimit is present, check if its respected
		if (abslimit>0.0) {
			CPPUNIT_ASSERT(fabs(A[iter->first]-B[iter->second]) <= abslimit);
		}
	}
}
