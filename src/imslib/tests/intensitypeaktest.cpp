#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <sstream>
#include <ims/intensitypeak.h>

using namespace ims;

//class declaration////////////////////////////////////////////////////////////
template<typename PeakType>
class IntensityPeakTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( IntensityPeakTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testGetSetIntensity );
	CPPUNIT_TEST( testOperators );
	CPPUNIT_TEST( testPrint );
	CPPUNIT_TEST( testPropertyGetter );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testOperators();
	void testGetSetIntensity();
	void testPrint();
	void testPropertyGetter();
private:
	typedef typename PeakType::intensity_type intensity_type;
};

//suite registration////////////////////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION( IntensityPeakTest<IntensityPeak<double> > );
//CPPUNIT_TEST_SUITE_REGISTRATION( IntensityPeakTest<IntensityPeak<float> > );
//CPPUNIT_TEST_SUITE_REGISTRATION( IntensityPeakTest<IntensityPeak<std::complex<double> > > );
//CPPUNIT_TEST_SUITE_REGISTRATION( IntensityPeakTest<IntensityPeak<int> > );

//class definition////////////////////////////////////////////////////////////
template<typename PeakType>
void IntensityPeakTest<PeakType>::setUp() {
}

template<typename PeakType>
void IntensityPeakTest<PeakType>::tearDown() {
}

template<typename PeakType>
void IntensityPeakTest<PeakType>::testConstructor() {
	
	// seems like different architektures handle default initialisation of eg doubles differently, 
	// sparc/solaris takes default constructor, 386/linux makes no explicit init
	PeakType e(42.23);
	//weia, 42.23 makes problem with float, seems that we have to be explicit, so we take constructor-cast
	CPPUNIT_ASSERT_EQUAL(42.23, e.getIntensity());		
	
	// check copy constructor
	PeakType f(e);
	CPPUNIT_ASSERT_EQUAL(e, f);
	
}

template<typename PeakType>
void IntensityPeakTest<PeakType>::testGetSetIntensity() {
	PeakType peak;
	peak.setIntensity(23.42);
	CPPUNIT_ASSERT_EQUAL(23.42, peak.getIntensity());
}

template<typename PeakType>
void IntensityPeakTest<PeakType>::testOperators() {
	PeakType peak1, peak2(1);
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1.setIntensity(peak2.getIntensity());
	CPPUNIT_ASSERT(peak1 == peak2);

	peak2.setIntensity(peak2.getIntensity()+1);
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1 = peak2;
	CPPUNIT_ASSERT(peak1 == peak1);
}

template<typename PeakType>
void IntensityPeakTest<PeakType>::testPrint() {
	// create peak
	intensity_type intensity = (intensity_type)4.3;
	PeakType peak(intensity);

	// generate correct string
	std::ostringstream oss_correct;
	oss_correct << intensity;

	// print peak output
	std::ostringstream oss;
	oss << peak;

	// compare
	CPPUNIT_ASSERT_EQUAL(oss_correct.str(), oss.str());
}

template<typename PeakType>
void IntensityPeakTest<PeakType>::testPropertyGetter() {
	PeakType p(10.2);
	const PeakType p_const(10.7);
	CPPUNIT_ASSERT_EQUAL(10.7, PeakType::IntensityGetter::get(p_const));
	CPPUNIT_ASSERT_EQUAL(10.2, PeakType::IntensityGetter::get(p));
	PeakType::IntensityGetter::get(p)=11.2;
	CPPUNIT_ASSERT_EQUAL(11.2, PeakType::IntensityGetter::get(p));
}
