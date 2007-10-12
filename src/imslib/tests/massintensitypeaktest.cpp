#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <sstream>

#include <ims/massintensitypeak.h>

using namespace ims;


template<typename PeakType>
class MassIntensityPeakTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( MassIntensityPeakTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testGettersSetters );
	CPPUNIT_TEST( testOperators );
	CPPUNIT_TEST( testPrint );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testOperators();
	void testGettersSetters();
	void testPrint();
private:
	typedef typename PeakType::mass_type mass_type;
	typedef typename PeakType::intensity_type intensity_type;
};


typedef MassIntensityPeakTest<MassIntensityPeak<double,double> > DoubleMIPeakTest;
typedef MassIntensityPeakTest<MassIntensityPeak<int,double> > IntegerMIPeakTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DoubleMIPeakTest );
CPPUNIT_TEST_SUITE_REGISTRATION( IntegerMIPeakTest );


template<typename PeakType>
void MassIntensityPeakTest<PeakType>::setUp() {
}

template<typename PeakType>
void MassIntensityPeakTest<PeakType>::tearDown() {
}

template<typename PeakType>
void MassIntensityPeakTest<PeakType>::testConstructor() {
	PeakType a, b((mass_type)1.2, (intensity_type)2.3);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, b.getMass());
	CPPUNIT_ASSERT_EQUAL((intensity_type)2.3, b.getIntensity());
	
	// check copy constructor
	PeakType c(b);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, c.getMass());
	CPPUNIT_ASSERT_EQUAL((intensity_type)2.3, c.getIntensity());
	
}

template<typename PeakType>
void MassIntensityPeakTest<PeakType>::testGettersSetters() {
	PeakType peak;
	peak.setMass((mass_type)1.2);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, peak.getMass());
	peak.setIntensity((intensity_type)2.3);
	CPPUNIT_ASSERT_EQUAL((intensity_type)2.3, peak.getIntensity());
}

template<typename PeakType>
void MassIntensityPeakTest<PeakType>::testOperators() {
	mass_type mass_value = (mass_type)1.0;
	intensity_type intensity_value = (intensity_type)1.0;
	PeakType peak1, peak2(mass_value, intensity_value);
	CPPUNIT_ASSERT( peak1 != peak2 );

	peak1.setMass(peak2.getMass());
	CPPUNIT_ASSERT( peak1 != peak2 );

	peak1.setIntensity(peak2.getIntensity());
	CPPUNIT_ASSERT_EQUAL(peak2, peak1);

	peak1.setMass(peak2.getMass()+1);
	CPPUNIT_ASSERT( peak1 != peak2);
	
	peak1 = peak2;
	CPPUNIT_ASSERT_EQUAL(peak2, peak1);
}

template<typename PeakType>
void MassIntensityPeakTest<PeakType>::testPrint() {
	// create peak
	mass_type mass = (mass_type)4.3;
	intensity_type intensity = (intensity_type)7.3;
	PeakType peak(mass, intensity);

	// generate correct string
	std::ostringstream oss_correct;
	oss_correct << "(" << mass << " " << intensity << ")";

	// print peak output
	std::ostringstream oss;
	oss << peak;

	// compare
	CPPUNIT_ASSERT_EQUAL(oss_correct.str(), oss.str());
}
