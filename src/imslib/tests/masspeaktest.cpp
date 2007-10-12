#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <sstream>
#include <ims/masspeak.h>

using namespace ims;

//class declaration////////////////////////////////////////////////////////////
template<typename PeakType>
class MassPeakTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( MassPeakTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testGetSetMass );
	CPPUNIT_TEST( testOperators );
	CPPUNIT_TEST( testPrint );
	CPPUNIT_TEST( testPropertyGetter );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testOperators();
	void testGetSetMass();
	void testPrint();
	void testPropertyGetter();
private:
	typedef typename PeakType::mass_type mass_type;
};

//suite registration////////////////////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION( MassPeakTest<MassPeak<double> > );
//CPPUNIT_TEST_SUITE_REGISTRATION( MassPeakTest<MassPeak<float> > );
//CPPUNIT_TEST_SUITE_REGISTRATION( MassPeakTest<MassPeak<std::complex<double> > > );
//CPPUNIT_TEST_SUITE_REGISTRATION( MassPeakTest<MassPeak<int> > );

//class definition////////////////////////////////////////////////////////////
template<typename PeakType>
void MassPeakTest<PeakType>::setUp() {
}

template<typename PeakType>
void MassPeakTest<PeakType>::tearDown() {
}

template<typename PeakType>
void MassPeakTest<PeakType>::testConstructor() {
	
	// seems like different architektures handle default initialisation of eg doubles differently, 
	// sparc/solaris takes default constructor, 386/linux makes no explicit init
	PeakType e(42.23);
	//weia, 42.23 makes problem with float, seems that we have to be explicit, so we take constructor-cast
	CPPUNIT_ASSERT_EQUAL(42.23, e.getMass());
	
	// check copy constructor
	PeakType f(e);
	CPPUNIT_ASSERT_EQUAL(e, f);
	
}

template<typename PeakType>
void MassPeakTest<PeakType>::testGetSetMass() {
	PeakType peak;
	peak.setMass(23.42);
	CPPUNIT_ASSERT_EQUAL(23.42, peak.getMass());
}

template<typename PeakType>
void MassPeakTest<PeakType>::testOperators() {
	PeakType peak1, peak2(1);
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1.setMass(peak2.getMass());
	CPPUNIT_ASSERT(peak1 == peak2);

	peak2.setMass(peak2.getMass()+1);
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1 = peak2;
	CPPUNIT_ASSERT(peak1 == peak1);
}

template<typename PeakType>
void MassPeakTest<PeakType>::testPrint() {
	// create peak
	mass_type mass = (mass_type)4.3;
	PeakType peak(mass);

	// generate correct string
	std::ostringstream oss_correct;
	oss_correct << mass;

	// print peak output
	std::ostringstream oss;
	oss << peak;

	// compare
	CPPUNIT_ASSERT_EQUAL(oss_correct.str(), oss.str());
}

template<typename PeakType>
void MassPeakTest<PeakType>::testPropertyGetter() {
	PeakType p(10.2);
	const PeakType p_const(10.7);
	CPPUNIT_ASSERT_EQUAL(10.7, PeakType::MassGetter::get(p_const));
	CPPUNIT_ASSERT_EQUAL(10.2, PeakType::MassGetter::get(p));
	PeakType::MassGetter::get(p)=11.2;
	CPPUNIT_ASSERT_EQUAL(11.2, PeakType::MassGetter::get(p));
}
