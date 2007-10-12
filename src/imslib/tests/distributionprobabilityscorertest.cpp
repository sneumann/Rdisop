#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/distributionprobabilityscorer.h>

using namespace ims;

class DistributionProbabilityScorerTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(DistributionProbabilityScorerTest);
	CPPUNIT_TEST(testScore);
	CPPUNIT_TEST_SUITE_END();

public:
	void testScore();
};

CPPUNIT_TEST_SUITE_REGISTRATION(DistributionProbabilityScorerTest);

void DistributionProbabilityScorerTest::testScore() {
	typedef IsotopeDistribution::peaks_container peaks_container;
	typedef IsotopeDistribution::mass_type mass_type;
	typedef IsotopeDistribution::abundance_type abundance_type;
	typedef IsotopeDistribution::nominal_mass_type nominal_mass_type;

// Hydrogen
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	IsotopeDistribution distribution(peaksH, massH);

	ims::DistributionProbabilityScorer dps(distribution);

	peaks_container measured_peaks;
	measured_peaks.push_back(peaks_container::value_type(0.007825, 0.99985));
	measured_peaks.push_back(peaks_container::value_type(0.014102, 0.00015));

	IsotopeDistribution measured_distribution(measured_peaks);

	//printf("dps.score(measured_distribution) = %.30f\n", dps.score(measured_distribution));
	//CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, dps.score(measured_distribution), 1e-10);
}
