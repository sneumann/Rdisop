#ifndef IMS_DISTRIBUTIONPROBABILITYSCORER_H
#define IMS_DISTRIBUTIONPROBABILITYSCORER_H

#include <vector>
#include <ims/isotopedistribution.h>

namespace ims {

// Comments:
// IsotopeDistribution is vector of pairs (mass, intensity).
// There exist simple getters to get mass and intensity:
// IsotopeDistribution::getMass(size_type index) and IsotopeDistribution::getAbundance(size_type index)
// Better performance will be reached if we cache the vector in this class itself, but I would first try
// like this with a given IsotopeDistribution interface and then see. If you think differently, give me a sign // ;-)
class DistributionProbabilityScorer {
	public:
		typedef std::vector<double> masses_container;
		typedef std::vector<double> abundances_container;
		typedef masses_container::size_type size_type;
		typedef double score_type;

		DistributionProbabilityScorer(const masses_container& predicted_masses, 
										const abundances_container& predicted_abundances);		

		DistributionProbabilityScorer(const IsotopeDistribution& distribution);		

		std::vector<score_type> scores(const masses_container& measured_masses, 
							const abundances_container& measured_abundances) const;

		score_type score(const masses_container& measured_masses, 
							const abundances_container& measured_abundances) const;

		score_type score(const IsotopeDistribution& distribution) const;

		void setMassPrecision(double new_mass_precision_ppm);
		
		double getMassPrecision() const { return mass_precision_ppm; }

		const masses_container& getPredictedMasses() const { return predicted_masses; }		
		
		const abundances_container& getPredictedAbundances() const { return predicted_abundances; }		
		
		void setDebugMode(bool isDebugMode) { this->isDebugMode = isDebugMode; }

	private:
		struct NormalDistribution {
			NormalDistribution(double mean, double variance) : mean(mean), variance(variance) { }
			double mean;
			double variance;
		};
		
		masses_container predicted_masses;
		abundances_container predicted_abundances;
		double mass_precision_ppm;
		std::vector<NormalDistribution> mass_dists;
		std::vector<NormalDistribution> intensity_dists;
		
		bool isDebugMode;
		
};

std::ostream& operator<<(std::ostream& os, const DistributionProbabilityScorer& scorer);

} // namespace ims

#endif
