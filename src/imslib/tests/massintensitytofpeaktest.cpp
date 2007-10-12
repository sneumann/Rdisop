#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <sstream>

#include <ims/massintensitytofpeak.h>

using namespace ims;


template<typename PeakType>
class MassIntensityTOFPeakTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( MassIntensityTOFPeakTest );
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
	typedef typename PeakType::tof_type tof_type;
};


typedef MassIntensityTOFPeakTest<MassIntensityTOFPeak<double,double,double> > MITPeakTest;

CPPUNIT_TEST_SUITE_REGISTRATION( MITPeakTest );

template<typename PeakType>
void MassIntensityTOFPeakTest<PeakType>::setUp() {
}

template<typename PeakType>
void MassIntensityTOFPeakTest<PeakType>::tearDown() {
}

template<typename PeakType>
void MassIntensityTOFPeakTest<PeakType>::testConstructor() {
	PeakType a, b((mass_type)1.2, (intensity_type)2.3, (tof_type)84.3);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, b.getMass());
	CPPUNIT_ASSERT_EQUAL((intensity_type)2.3, b.getIntensity());
	CPPUNIT_ASSERT_EQUAL((tof_type)84.3, b.getTOF());
	
	// check copy constructor
	PeakType c(b);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, c.getMass());
	CPPUNIT_ASSERT_EQUAL((intensity_type)2.3, c.getIntensity());
	CPPUNIT_ASSERT_EQUAL((tof_type)84.3, c.getTOF());
}

template<typename PeakType>
void MassIntensityTOFPeakTest<PeakType>::testGettersSetters() {
	PeakType peak;
	peak.setMass((mass_type)1.2);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, peak.getMass());
	peak.setIntensity((intensity_type)2.3);
	CPPUNIT_ASSERT_EQUAL((intensity_type)2.3, peak.getIntensity());
	peak.setTOF((tof_type)12.2);
	CPPUNIT_ASSERT_EQUAL((tof_type)12.2, peak.getTOF());
}

template<typename PeakType>
void MassIntensityTOFPeakTest<PeakType>::testOperators() {
	mass_type mass_value = (mass_type)1.0;
	intensity_type intensity_value = (intensity_type)2.0;
	tof_type tof_value = (tof_type)3.4;
	PeakType peak1, peak2(mass_value, intensity_value, tof_value);
	CPPUNIT_ASSERT( peak1 != peak2 );

	peak1.setMass(peak2.getMass());
	CPPUNIT_ASSERT(peak1 != peak2 );

	peak1.setIntensity(peak2.getIntensity());
	CPPUNIT_ASSERT(peak2 != peak1);

	peak1.setTOF(peak2.getTOF());
	CPPUNIT_ASSERT(peak2 == peak1);

	peak1.setMass(peak2.getMass()+1);
	CPPUNIT_ASSERT(peak1 != peak2);
	
	PeakType peak3;
	peak3 = peak2;
	CPPUNIT_ASSERT(peak2 == peak3);
}

template<typename PeakType>
void MassIntensityTOFPeakTest<PeakType>::testPrint() {
	// create peak
	mass_type mass = (mass_type)4.3;
	intensity_type intensity = (intensity_type)7.3;
	tof_type tof = (tof_type)11.7;
	PeakType peak(mass, intensity, tof);

	// generate correct string
	std::ostringstream oss_correct;
	oss_correct << "(" << mass << " " << intensity << " " << tof << ")";

	// print peak output
	std::ostringstream oss;
	oss << peak;

	// compare
	CPPUNIT_ASSERT_EQUAL(oss_correct.str(), oss.str());
}
