#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <sstream>

#include <ims/masstofpeak.h>

using namespace ims;


template<typename PeakType>
class MassTOFPeakTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( MassTOFPeakTest );
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
	typedef typename PeakType::tof_type tof_type;
};


typedef MassTOFPeakTest<MassTOFPeak<double,double> > MTPeakTest;

CPPUNIT_TEST_SUITE_REGISTRATION( MTPeakTest );

template<typename PeakType>
void MassTOFPeakTest<PeakType>::setUp() {
}

template<typename PeakType>
void MassTOFPeakTest<PeakType>::tearDown() {
}

template<typename PeakType>
void MassTOFPeakTest<PeakType>::testConstructor() {
	PeakType a, b((mass_type)1.2, (tof_type)84.3);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, b.getMass());
	CPPUNIT_ASSERT_EQUAL((tof_type)84.3, b.getTOF());
	
	// check copy constructor
	PeakType c(b);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, c.getMass());
	CPPUNIT_ASSERT_EQUAL((tof_type)84.3, c.getTOF());
}

template<typename PeakType>
void MassTOFPeakTest<PeakType>::testGettersSetters() {
	PeakType peak;
	peak.setMass((mass_type)1.2);
	CPPUNIT_ASSERT_EQUAL((mass_type)1.2, peak.getMass());
	peak.setTOF((tof_type)12.2);
	CPPUNIT_ASSERT_EQUAL((tof_type)12.2, peak.getTOF());
}

template<typename PeakType>
void MassTOFPeakTest<PeakType>::testOperators() {
	mass_type mass_value = (mass_type)1.0;
	tof_type tof_value = (tof_type)3.4;
	PeakType peak1, peak2(mass_value, tof_value);
	CPPUNIT_ASSERT( peak1 != peak2 );

	peak1.setMass(peak2.getMass());
	CPPUNIT_ASSERT(peak1 != peak2 );

	peak1.setTOF(peak2.getTOF());
	CPPUNIT_ASSERT(peak2 == peak1);

	peak1.setMass(peak2.getMass()+1);
	CPPUNIT_ASSERT(peak1 != peak2);
	
	PeakType peak3;
	peak3 = peak2;
	CPPUNIT_ASSERT(peak2 == peak3);
}

template<typename PeakType>
void MassTOFPeakTest<PeakType>::testPrint() {
	// create peak
	mass_type mass = (mass_type)4.3;
	tof_type tof = (tof_type)11.7;
	PeakType peak(mass, tof);

	// generate correct string
	std::ostringstream oss_correct;
	oss_correct << "(" << mass << " " << tof << ")";

	// print peak output
	std::ostringstream oss;
	oss << peak;

	// compare
	CPPUNIT_ASSERT_EQUAL(oss_correct.str(), oss.str());
}
