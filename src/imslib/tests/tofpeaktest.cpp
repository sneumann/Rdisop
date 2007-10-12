#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <sstream>
#include <ims/tofpeak.h>

using namespace ims;

//class declaration////////////////////////////////////////////////////////////
template<typename PeakType>
class TOFPeakTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( TOFPeakTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testGetSetTOF );
	CPPUNIT_TEST( testOperators );
	CPPUNIT_TEST( testPrint );
	CPPUNIT_TEST( testPropertyGetter );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testOperators();
	void testGetSetTOF();
	void testPrint();
	void testPropertyGetter();
private:
	typedef typename PeakType::tof_type tof_type;
};

//suite registration////////////////////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION( TOFPeakTest<TOFPeak<double> > );
//CPPUNIT_TEST_SUITE_REGISTRATION( TOFPeakTest<TOFPeak<float> > );
//CPPUNIT_TEST_SUITE_REGISTRATION( TOFPeakTest<TOFPeak<std::complex<double> > > );
//CPPUNIT_TEST_SUITE_REGISTRATION( TOFPeakTest<TOFPeak<int> > );

//class definition////////////////////////////////////////////////////////////
template<typename PeakType>
void TOFPeakTest<PeakType>::setUp() {
}

template<typename PeakType>
void TOFPeakTest<PeakType>::tearDown() {
}

template<typename PeakType>
void TOFPeakTest<PeakType>::testConstructor() {
	
	// seems like different architektures handle default initialisation of eg doubles differently, 
	// sparc/solaris takes default constructor, 386/linux makes no explicit init
	PeakType e(42.23);
	//weia, 42.23 makes problem with float, seems that we have to be explicit, so we take constructor-cast
	CPPUNIT_ASSERT_EQUAL(42.23, e.getTOF());		
	
	// check copy constructor
	PeakType f(e);
	CPPUNIT_ASSERT_EQUAL(e, f);
	
}

template<typename PeakType>
void TOFPeakTest<PeakType>::testGetSetTOF() {
	PeakType peak;
	peak.setTOF(23.42);
	CPPUNIT_ASSERT_EQUAL(23.42, peak.getTOF());
}

template<typename PeakType>
void TOFPeakTest<PeakType>::testOperators() {
	PeakType peak1, peak2(1);
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1.setTOF(peak2.getTOF());
	CPPUNIT_ASSERT(peak1 == peak2);

	peak2.setTOF(peak2.getTOF()+1);
	CPPUNIT_ASSERT(peak1 != peak2);

	peak1 = peak2;
	CPPUNIT_ASSERT(peak1 == peak1);
}

template<typename PeakType>
void TOFPeakTest<PeakType>::testPrint() {
	// create peak
	tof_type tof = (tof_type)4.3;
	PeakType peak(tof);

	// generate correct string
	std::ostringstream oss_correct;
	oss_correct << tof;

	// print peak output
	std::ostringstream oss;
	oss << peak;

	// compare
	CPPUNIT_ASSERT_EQUAL(oss_correct.str(), oss.str());
}

template<typename PeakType>
void TOFPeakTest<PeakType>::testPropertyGetter() {
	PeakType p(10.2);
	const PeakType p_const(10.7);
	CPPUNIT_ASSERT_EQUAL(10.7, PeakType::TOFGetter::get(p_const));
	CPPUNIT_ASSERT_EQUAL(10.2, PeakType::TOFGetter::get(p));
	PeakType::TOFGetter::get(p)=11.2;
	CPPUNIT_ASSERT_EQUAL(11.2, PeakType::TOFGetter::get(p));
}
