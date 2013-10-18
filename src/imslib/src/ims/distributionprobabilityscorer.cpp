#include <ims/distributionprobabilityscorer.h>

#include <math.h> // erfc
#include <cmath>
#include <cassert>
#include <iostream>

namespace ims {

/*
TODO
- don't save variance, but deviation/sqrt(2)
*/

DistributionProbabilityScorer::DistributionProbabilityScorer(
			const IsotopeDistribution& distribution) {
	DistributionProbabilityScorer(distribution.getMasses(), 
								distribution.getAbundances());
}

DistributionProbabilityScorer::DistributionProbabilityScorer(
			const masses_container& masses,
			const abundances_container& abundances) :
											predicted_masses(masses),
											predicted_abundances(abundances), 
											mass_precision_ppm(2),
											isDebugMode(false) {

//  masses, absolute differences, wabi dataset
//	mass_dists.push_back(NormalDistribution(0.0001433646, 6.564365e-07));
//	mass_dists.push_back(NormalDistribution(0.0004040694, 1.065090e-06));
//	mass_dists.push_back(NormalDistribution(0.0007777777, 6.82385e-06));
//	mass_dists.push_back(NormalDistribution(0.001669122 , 1.294743e-05));

//  masses, absolute indirect differences, wabi dataset
//	mass_dists.push_back(NormalDistribution(0.0001433646, 6.564365e-07));
//	mass_dists.push_back(NormalDistribution(0.0002607048, 8.940188e-07));
//	mass_dists.push_back(NormalDistribution(0.0006075166, 6.255843e-06));
//	mass_dists.push_back(NormalDistribution(0.001465184 , 1.164616e-05));

//	masses, relative direct differences, wabi dataset
//	mass_dists.push_back(NormalDistribution(1.799871e-07, 8.513952e-13));
//	mass_dists.push_back(NormalDistribution(4.996237e-07, 1.356805e-12));
//	mass_dists.push_back(NormalDistribution(7.621271e-07, 1.716217e-11));
//	mass_dists.push_back(NormalDistribution(1.276235e-06, 9.733704e-12));

//	masses, relative indirect differences, wabi dataset
//	mass_dists.push_back(NormalDistribution(1.799871e-07, 8.513952e-13));
//	mass_dists.push_back(NormalDistribution(3.199036e-07, 1.016270e-12));
//	mass_dists.push_back(NormalDistribution(5.734557e-07, 1.839383e-11));
//	mass_dists.push_back(NormalDistribution(1.140815e-06, 8.336567e-12));

//  masses, absolute differences, after wabi dataset
//	mass_dists.push_back(NormalDistribution(-0.0001442095, 1.302513e-06));
//	mass_dists.push_back(NormalDistribution(-2.145408e-05, 3.417536e-06));
//	mass_dists.push_back(NormalDistribution(0.0001892252, 1.268451e-05));
//	mass_dists.push_back(NormalDistribution(8.855847e-05, 1.536373e-05));

//  masses, absolute indirect differences, after wabi dataset
//	mass_dists.push_back(NormalDistribution(-0.0001442095, 1.302513e-06));
//	mass_dists.push_back(NormalDistribution(0.0001227554, 1.600073e-06));
//	mass_dists.push_back(NormalDistribution(0.0003123833, 1.076185e-05));
//	mass_dists.push_back(NormalDistribution(0.0001227075, 1.775795e-05));

//	masses, relative direct differences, after wabi dataset
//	mass_dists.push_back(NormalDistribution(-3.43578e-07, 1.403349e-12));
//	mass_dists.push_back(NormalDistribution(-1.599803e-07, 3.07873e-12));
//	mass_dists.push_back(NormalDistribution(4.503311e-07, 3.545016e-11));
//	mass_dists.push_back(NormalDistribution(7.256986e-08, 1.103779e-11));


//  masses, relative indirect differences, after wabi dataset
//	mass_dists.push_back(NormalDistribution(-3.43578e-07, 1.403349e-12));
//	mass_dists.push_back(NormalDistribution(1.827445e-07, 1.493048e-12));
//	mass_dists.push_back(NormalDistribution(7.298317e-07, 3.296835e-11));
//	mass_dists.push_back(NormalDistribution(4.255736e-08, 1.167063e-11));

 
//  masses, absolute differences, two datasets merged
//	mass_dists.push_back(NormalDistribution(-1.860300e-06, 9.985758e-07));
//	mass_dists.push_back(NormalDistribution(1.891800e-04, 2.287362e-06));
//	mass_dists.push_back(NormalDistribution(4.550231e-04, 1.006261e-05));
//	mass_dists.push_back(NormalDistribution(7.393786e-04, 1.477482e-05));

//  masses, absolute indirect differences, two datasets merged
//	mass_dists.push_back(NormalDistribution(-1.860300e-06, 9.985758e-07));
//	mass_dists.push_back(NormalDistribution(1.910403e-04, 1.249108e-06));
//	mass_dists.push_back(NormalDistribution(4.456693e-04, 8.694753e-06));
//	mass_dists.push_back(NormalDistribution(6.754921e-04, 1.547299e-05));

//	masses, relative direct differences, two datasets merged
//	mass_dists.push_back(NormalDistribution(-8.441329e-08, 1.193348e-12));
//	mass_dists.push_back(NormalDistribution(1.665236e-07, 2.324581e-12));
//	mass_dists.push_back(NormalDistribution(5.911422e-07, 2.705023e-11));
//	mass_dists.push_back(NormalDistribution(5.681968e-07, 1.070368e-11));


//  masses, relative indirect differences, two datasets merged
	mass_dists.push_back(NormalDistribution(-8.441329e-08, 1.193348e-12));
	mass_dists.push_back(NormalDistribution(2.506383e-07, 1.255476e-12));
	mass_dists.push_back(NormalDistribution(6.592103e-07, 2.623022e-11));
	mass_dists.push_back(NormalDistribution(4.947810e-07, 1.044937e-11));



// intensities relative diff, wabi dataset
//	intensity_dists.push_back(NormalDistribution(1.03195767, 0.001582980));
//	intensity_dists.push_back(NormalDistribution(0.97171332, 0.006048766));
//	intensity_dists.push_back(NormalDistribution(0.84044536, 0.01912602));
//	intensity_dists.push_back(NormalDistribution(0.91226691, 0.02934473));

// intensities log relative diff, wabi dataset
//	intensity_dists.push_back(NormalDistribution(0.01334766, 0.0002736877));
//	intensity_dists.push_back(NormalDistribution(-0.01378487, 0.001143297));
//	intensity_dists.push_back(NormalDistribution(-0.08218637, 0.006394038));
//	intensity_dists.push_back(NormalDistribution(-0.04671003, 0.005966827));

// intensities relative diff, after wabi dataset
//	intensity_dists.push_back(NormalDistribution(1.03273570, 0.0020775130));
//	intensity_dists.push_back(NormalDistribution(0.96675791, 0.003281593));
//	intensity_dists.push_back(NormalDistribution(0.8859227, 0.02864359));
//	intensity_dists.push_back(NormalDistribution(0.91730298, 0.02712402));

// intensities log relative diff, after wabi dataset
//	intensity_dists.push_back(NormalDistribution(0.01355489, 0.0003884638));
//	intensity_dists.push_back(NormalDistribution(-0.01542977, 0.0006533951));
//	intensity_dists.push_back(NormalDistribution(-0.0599873, 0.00647435));
//	intensity_dists.push_back(NormalDistribution(-0.04383001, 0.005518833));

// intensities relative diff, two datasets merged
//	intensity_dists.push_back(NormalDistribution(1.03235058, 0.0018236865));
//	intensity_dists.push_back(NormalDistribution(0.96921084, 0.004634000));
//	intensity_dists.push_back(NormalDistribution(0.86538453, 0.02470876));
//	intensity_dists.push_back(NormalDistribution(0.9152293, 0.02762033));

// intensities log relative diff, two datasets merged
	intensity_dists.push_back(NormalDistribution(0.01345231, 0.0003299996));
	intensity_dists.push_back(NormalDistribution(-0.01461554, 0.0008920495));
	intensity_dists.push_back(NormalDistribution(-0.07001268, 0.006519163));
	intensity_dists.push_back(NormalDistribution(-0.0450159, 0.005619036));

////////////////////////////////////////////////////////////////////////////
/////////////////////////// old  ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//	masses without ions contribution, wabi dataset
//	mass_dists.push_back(NormalDistribution(1.799871e-07, 8.513952e-13));
//	mass_dists.push_back(NormalDistribution(2.729544e-07, 1.019279e-12));
//	mass_dists.push_back(NormalDistribution(5.743761e-07, 1.855474e-11));
//	mass_dists.push_back(NormalDistribution(1.143107e-06, 8.386615e-12));

// intensities without ions contribution, wabi dataset
//	intensity_dists.push_back(NormalDistribution(0.01119056, 0.000422112));
//	intensity_dists.push_back(NormalDistribution(-0.01550129, 0.001456923));
//	intensity_dists.push_back(NormalDistribution(-0.08294288, 0.006571717));
//	intensity_dists.push_back(NormalDistribution(-0.04612332, 0.005986772));
	
	/** 
	 * mean and variances for multiply charged molecules
	 */
	
//	mass_dists.push_back(NormalDistribution(-1.198511e-08, 1.060035e-12));
//	mass_dists.push_back(NormalDistribution(-1.213098e-06, 4.354947e-12));
//	mass_dists.push_back(NormalDistribution(-1.159330e-06, 1.939770e-11));
//////	mass_dists.push_back(NormalDistribution(-8.335625e-07, 7.226447e-11));
//
//	intensity_dists.push_back(NormalDistribution(-0.01169652, 0.004392741));
//	intensity_dists.push_back(NormalDistribution(0.0548551, 0.007927192));
//	intensity_dists.push_back(NormalDistribution(0.0814128, 0.05885608));
////	intensity_dists.push_back(NormalDistribution(0.04734685, 0.1313852));
}


void DistributionProbabilityScorer::setMassPrecision(double new_mass_precision_ppm) {
	for (std::vector<NormalDistribution>::iterator it = mass_dists.begin();
									it != mass_dists.end(); ++it) {
		it->mean *= new_mass_precision_ppm / mass_precision_ppm;		
		it->variance *= new_mass_precision_ppm * new_mass_precision_ppm / mass_precision_ppm / mass_precision_ppm;
	}
	mass_precision_ppm = new_mass_precision_ppm;
}


std::vector<DistributionProbabilityScorer::score_type> DistributionProbabilityScorer::
scores(const masses_container& measured_masses,
		const abundances_container& measured_abundances) const {

	// TODO a lot in this function can be simplified and/or precalculated
	using std::abs;

	double sqrt2 = sqrt(2.0);

	/*
	Formulas

	given:
	M_i: measured mass
	m_i: predicted (true) mass


	x_i: relative mass difference (for i>=1 it's additionally "relative to x_0")
	with
	x_0 = |M_0 - m_0| / m_0
	and
	x_i = |M_i - M_0 - m_i + m_0| / m_i

	Probability: (As an abbreviaton let x=x_i.)

	P(mass difference >= x) = erfc(z/sqrt(2))
	with
	z = (x - mean(x)) / stddev(x)
	where
	mean(x) is the empirical arithmetic mean of x and
	stddev(x) is the empirical standard deviation of x.

	These probabilities are calculated for every peak and then multiplied.
	*/

	std::vector<score_type> scores;
	// masses

	size_t i_max = std::min(predicted_masses.size(), measured_masses.size());
	assert(predicted_masses.size() > 0);
	assert(measured_masses.size() > 0);
	
	// first peak (only mass)
	// old mass scoring
//	double x = (measured_masses[0] - predicted_masses[0]) / predicted_masses[0];
	// new mass scoring: relative diff
	double x = (predicted_masses[0] - measured_masses[0]) / measured_masses[0];
	// new mass scoring: absolute diff	
//	double x = predicted_masses[0] - measured_masses[0];

	double prob = erfc(abs(x - mass_dists[0].mean) / (sqrt(mass_dists[0].variance) * sqrt2));

	scores.push_back(prob);
	// if (isDebugMode) {
 	// 	std::cout << "erfc[mass_0] = " << prob << '\n';
	// }
	
	// remaining peaks (only mass)
	for (size_type i = 1; i < i_max; ++i) {
		
		// old mass scoring: relative indirect diff
//		x = (measured_masses[i] - measured_masses[0] - predicted_masses[i] + predicted_masses[0]) / predicted_masses[i];
		// new mass scoring: relative indirect diff
		x = (predicted_masses[i] - predicted_masses[0] - measured_masses[i] + measured_masses[0]) / measured_masses[i];
		
		// new mass scoring: relative direct diff
//		x = (predicted_masses[i] - measured_masses[i]) / measured_masses[i];
		// old mass scoring: relative direct diff
//		x = (measured_masses[i] - predicted_masses[i]) / predicted_masses[i];		
//		// absolute indirect diff
//		x = (predicted_masses[i] - predicted_masses[0] - measured_masses[i] + measured_masses[0]);
//		// absolute direct diff
//		x = predicted_masses[i] - measured_masses[i];
		
		double mean = (i < mass_dists.size()) ? mass_dists[i].mean : mass_dists.back().mean;
		double variance = (i < mass_dists.size()) ? mass_dists[i].variance : mass_dists.back().variance;
		prob *= erfc(abs(x - mean) / (sqrt(variance) * sqrt2));
		// if (isDebugMode) {
 		// 	std::cout << "erfc[mass_" << i << "] = " << erfc(abs(x - mean) / (sqrt(variance) * sqrt2)) << '\n';
		// }
		scores.push_back(erfc(abs(x - mean) / (sqrt(variance) * sqrt2)));
	}

	// if (isDebugMode) {
 	// 	std::cout << "prob (after masses) = " << prob << '\n';
	// }

	// intensities
	i_max = std::min(predicted_masses.size(), std::min(measured_masses.size(), intensity_dists.size()));

	// remaining peaks
	for (size_type i = 0; i < i_max; ++i) {
		// score relative log
		x = log10(predicted_abundances[i] / measured_abundances[i]);
		// score relative
//		x = predicted_abundances[i] / measured_abundances[i];
		
		double mean = (i < intensity_dists.size()) ? intensity_dists[i].mean : intensity_dists.back().mean;
		double variance = (i < intensity_dists.size()) ? intensity_dists[i].variance : intensity_dists.back().variance;

		prob *= erfc(abs(x - mean) / (sqrt(variance) * sqrt2));
		// or perhaps:
//		 prob *= erfc((x - mean) / (sqrt(variance) * sqrt2));
		// if (isDebugMode) {
 		// 	std::cout << "erfc[abund_" << i << "] = " << erfc(abs(x - mean) / (sqrt(variance) * sqrt2)) << '\n';
		// }
		scores.push_back(erfc(abs(x - mean) / (sqrt(variance) * sqrt2)));
	}
	
	// if (isDebugMode) {
 	// 	std::cout << "prob (after abundances) = " << prob << '\n';
	// }

//////	i_max = std::min(predicted_masses.size(), measured_masses.size());
//////
//////	// "missing" peaks (those not present in the measured spectrum)
//////	if (predicted.size() > 0) {
//////		double min_intensity = predicted_abundances[0];
//////		for (size_type i = 1; i < predicted_abundances.size(); ++i) {
//////			double intensity = predicted_abundances[i];
//////			if (min_intensity > intensity) {
//////				min_intensity = intensity;
//////			}
//////		}
//////		for (size_type i = i_max; i < measured.size(); ++i) {
//////			x = log10(min_intensity / measured_abundances[i]);
//////			double mean = (i < intensity_dists.size()) ? intensity_dists[i].mean : intensity_dists.back().mean;
//////			double variance = (i < intensity_dists.size()) ? intensity_dists[i].variance : intensity_dists.back().variance;
//////			prob *= 0.5 * erfc(- (x - mean) / (sqrt(variance) * sqrt2));
//////
////////			std::cout << "prob_missing_" << i << " = " << 0.5 * erfc(- (x - mean) / (sqrt(variance) * sqrt2))
////////					  << "\tx = " << x << ", erfc_arg = " << (x - mean) / (sqrt(variance) * sqrt2) << '\n';
//////		}
//////	}
//////	std::cout << "prob (after missing peaks) = " << prob << '\n';
	return scores;
}

DistributionProbabilityScorer::score_type 
DistributionProbabilityScorer::score(const masses_container& measured_masses,
		const abundances_container& measured_abundances) const {
	std::vector<score_type> erfc_scores = this->scores(measured_masses, measured_abundances);
	score_type score = (erfc_scores.size() > 0) ? erfc_scores[0]: 1.0;
	for (std::vector<score_type>::size_type si = 1; si < erfc_scores.size(); ++si) {				
		score *= erfc_scores[si];
	}						
	return score;
}

DistributionProbabilityScorer::score_type 
DistributionProbabilityScorer::score(const IsotopeDistribution& distribution) const {
	return this->score(distribution.getMasses(), distribution.getAbundances());
}

std::ostream& operator <<(std::ostream& os, 
							const DistributionProbabilityScorer& scorer) {
	os << "predicted masses and abundances:\n";
	for (DistributionProbabilityScorer::masses_container::size_type i = 0; 
					i < scorer.getPredictedMasses().size() && 
					i < scorer.getPredictedAbundances().size(); ++i) {
						
		os 	<< i << ": [" << scorer.getPredictedMasses()[i] << ", " 
			<< scorer.getPredictedAbundances()[i] 	<< "]\n";
			
	}	
	return os;
}

} // namespace ims
