#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <sstream>
#include <ims/fragmentpeak.h>

using namespace ims;

//class declaration////////////////////////////////////////////////////////////
template<typename PeakType>
class FragmentPeakTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( FragmentPeakTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testGettersSetters );
	CPPUNIT_TEST( testOperators );
	CPPUNIT_TEST( testPrint );
	CPPUNIT_TEST( testPropertyGetter );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testOperators();
	void testGettersSetters();
	void testPrint();
	void testPropertyGetter();
private:
	typedef typename PeakType::mass_type mass_type;
};

//suite registration////////////////////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION( FragmentPeakTest<FragmentPeak<double> > );
//CPPUNIT_TEST_SUITE_REGISTRATION( FragmentPeakTest<FragmentPeak<float> > );
//CPPUNIT_TEST_SUITE_REGISTRATION( FragmentPeakTest<FragmentPeak<std::complex<double> > > );
//CPPUNIT_TEST_SUITE_REGISTRATION( FragmentPeakTest<FragmentPeak<int> > );

//class definition////////////////////////////////////////////////////////////
template<typename PeakType>
void FragmentPeakTest<PeakType>::setUp() {
}

template<typename PeakType>
void FragmentPeakTest<PeakType>::tearDown() {
}

template<typename PeakType>
void FragmentPeakTest<PeakType>::testConstructor() {
	
	// seems like different architektures handle default initialisation of eg doubles differently, 
	// sparc/solaris takes default constructor, 386/linux makes no explicit init
	PeakType e(42.23, 11, 12, 13);
	//weia, 42.23 makes problem with float, seems that we have to be explicit, so we take constructor-cast
	CPPUNIT_ASSERT_EQUAL(42.23, e.getMass());
	CPPUNIT_ASSERT_EQUAL((size_t)11, e.getStart());
	CPPUNIT_ASSERT_EQUAL((size_t)12, e.getLength());
	CPPUNIT_ASSERT_EQUAL((size_t)13, e.getMiscleavageCount());
	
	// check copy constructor
	PeakType f(e);
	CPPUNIT_ASSERT_EQUAL(e, f);
	
}

template<typename PeakType>
void FragmentPeakTest<PeakType>::testGettersSetters() {
	PeakType peak;
	peak.setMass(23.42);
	CPPUNIT_ASSERT_EQUAL(23.42, peak.getMass());
	peak.setStart(2);
	CPPUNIT_ASSERT_EQUAL((size_t)2, peak.getStart());
	peak.setLength(3);
	CPPUNIT_ASSERT_EQUAL((size_t)3, peak.getLength());
	peak.setMiscleavageCount(4);
	CPPUNIT_ASSERT_EQUAL((size_t)4, peak.getMiscleavageCount());
}

template<typename PeakType>
void FragmentPeakTest<PeakType>::testOperators() {
	PeakType peak1, peak2(7.3, 21, 22, 23);
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1.setMass(peak2.getMass());
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1.setStart(peak2.getStart());
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1.setLength(peak2.getLength());
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1.setMiscleavageCount(peak2.getMiscleavageCount());
	CPPUNIT_ASSERT(peak1 == peak2);

	peak2.setMass(peak2.getMass()+1);
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1 = peak2;
	CPPUNIT_ASSERT(peak1 == peak1);
}

template<typename PeakType>
void FragmentPeakTest<PeakType>::testPrint() {
	// create peak
	mass_type mass = (mass_type)4.3;
	size_t start = 5;
	size_t length = 7;
	size_t miscleavage_count = 3;
	PeakType peak(mass, start, length, miscleavage_count);

	// generate correct string
	std::ostringstream oss_correct;
	oss_correct << "(" << mass << " " << start << " " << length << " " << miscleavage_count << ")";

	// print peak output
	std::ostringstream oss;
	oss << peak;

	// compare
	CPPUNIT_ASSERT_EQUAL(oss_correct.str(), oss.str());
}

template<typename PeakType>
void FragmentPeakTest<PeakType>::testPropertyGetter() {
	PeakType p(10.2, 5, 6, 7);
	CPPUNIT_ASSERT_EQUAL((size_t)5, PeakType::FragmentStartGetter::get(p));
	CPPUNIT_ASSERT_EQUAL((size_t)6, PeakType::FragmentLengthGetter::get(p));
	CPPUNIT_ASSERT_EQUAL((size_t)7, PeakType::MiscleavageCountGetter::get(p));
	PeakType::FragmentStartGetter::get(p)=1;
	PeakType::FragmentLengthGetter::get(p)=2;
	PeakType::MiscleavageCountGetter::get(p)=3;
	CPPUNIT_ASSERT_EQUAL((size_t)1, PeakType::FragmentStartGetter::get(p));
	CPPUNIT_ASSERT_EQUAL((size_t)2, PeakType::FragmentLengthGetter::get(p));
	CPPUNIT_ASSERT_EQUAL((size_t)3, PeakType::MiscleavageCountGetter::get(p));

	const PeakType p_const(10.7, 8, 9, 10);
	CPPUNIT_ASSERT_EQUAL((size_t)8, PeakType::FragmentStartGetter::get(p_const));
	CPPUNIT_ASSERT_EQUAL((size_t)9, PeakType::FragmentLengthGetter::get(p_const));
	CPPUNIT_ASSERT_EQUAL((size_t)10, PeakType::MiscleavageCountGetter::get(p_const));
}
