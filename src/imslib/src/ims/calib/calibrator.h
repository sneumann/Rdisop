#ifndef IMS_CALIBRATOR_H
#define IMS_CALIBRATOR_H

#include <map>
#include <memory>

#include <ims/transformation.h>
#include <ims/logger.h>
#include <ims/chebyshevfitter.h>

namespace ims {

/*
TODO
- match => calibrate
-
- einen "Matcher" bauen?
- nicht ueber ListA und ListB templatisieren, sondern ueber ListA und ListB
- vielleicht ein "Calibration"-Objekt bauen, was zurueckgeliefert wird. Dann
  kann man auch maximumError, den score, die rekalibrierte peak list etc.
  verfuegbar machen.
*/
/**
 * TODO somebody proof-read this, please.
 *
 * The Calibrator interface.
 *
 * A Calibrator for two lists of (usually floating-point) values.
 *
 * Using some optimization criterion, the two given lists are mapped onto
 * each other ("matched"). The results of this are:
 * - a mapping (sometimes)
 * - a score
 * - a transformation
 *
 * Let a and b be the two lists with elements a[i] and a[j].
 *
 * The following parameters can be used to restrict the search space
 * - abslimit: If |a[i]-a[j]| > abslimit, then the pair (a[i],a[j]) will never
 *   be considered for a match.
 * - minscale, maxscale, mintranslation, and maxtranslation place restrictions
 *   on the linear transformation that is obtained. Since this can't be easily
 *   incorporated into some of the underlying algorithms, this is not
 *   enforced: The scale or translation of the returned transformation @b may
 *   have values outside these intervals.
 *
 * @param ListA ...
 */
template <typename ListA, typename ListB=ListA>
class Calibrator {
	public:
		virtual int match(const ListA& a, const ListB& b) = 0;

		/** Returns the actual mapping computed by the last call to match().
		 * @b Only usable for one-to-one mappings.
		 */
		virtual std::unique_ptr<std::map<int,int> > getMapping() const = 0;
	
		virtual LinearTransformation getTransformation() const = 0;
		
		/** Sets absolute limit to restrict the search-space. 
		 *
		 * Abslimit must not be confused with epsilon. Abslimit restricts the search space to
		 * only consider matches for which |A[i]-B[j]| <= abslimit, while epsilon defines the
		 * optimization criterion: Find a transformation f for which |f(A[i])-B[j]|<=epsilon
		 * is true for most pairs (i,j). TODO move description to PointSetMatcherCalibrator
		*/
		virtual void setAbsLimit(double limit) = 0;
		
		/** Set scale limit to restrict the search-space.
		 * @param min Minimal scale.
		 * @param max Maximal scale.
		*/
		virtual void setScaleInterval(double min, double max) = 0;
		
		virtual void setTranslationInterval(double min, double max) = 0;
		
		/** Set value for minimal number of point pairs that should be considered.
		  * If implementation would consider fewer point pairs, it has to return
		  * score 0.
		  */
		virtual void setMinPointPairCount(size_t count) = 0;
		
		/** Can be called before match().
		  * @returns if false is returned, match(A,B) will surely return score 0.
		  */
		virtual bool inputValid(const ListA& a, const ListB& b) const = 0;

		void reducedLists(const ListA& a, const ListB& b, ListA& a_reduced, ListB& b_reduced) const;
		
		ListB recalibrated(const ListA& predicted, const ListB& measured, int minimum_score = 5, unsigned degree = 1);

		virtual ~Calibrator() { }
};


template <typename ListA, typename ListB>
void Calibrator<ListA,ListB>::reducedLists(
	const ListA& a,
	const ListB& b,
	ListA& a_reduced,
	ListB& b_reduced) const
{
	// TODO: use exceptions here
	std::unique_ptr<std::map<int,int> > mapping_ptr = getMapping();
	assert(mapping_ptr.get() != NULL);
	std::map<int,int> &mapping = *mapping_ptr; // TODO
	a_reduced.clear();
	b_reduced.clear();
	std::map<int,int>::const_iterator cit;
	for (cit = mapping.begin(); cit != mapping.end(); ++cit) {
		assert(cit->first >= 0 && cit->first < a.size());
		assert(cit->second >= 0 && cit->second < b.size());
		a_reduced.push_back(a[cit->first]);
		b_reduced.push_back(b[cit->second]);
	}
}


/**
 * Performs recalibration a measured peak list.
 *
 * @param minimum_score If the peak list matching resulted in a score lower than this,
 *   the recalibration is @b not performed and @c measured is returned unchanged.
 * @param degree degree of the Chebyshev polynomial used during recalibration
 *
 * @return usually the recalibrated peak list, but see above
 */
template <typename ListA, typename ListB>
ListB Calibrator<ListA,ListB>::recalibrated(const ListA& predicted, const ListB& measured, int minimum_score, unsigned degree) {
	ListB new_measured(measured);
	int calib_score = match(predicted, measured);
	if (calib_score >= minimum_score) {
		ListA predicted_reduced;
		ListB measured_reduced;
		reducedLists(predicted, measured, predicted_reduced, measured_reduced);
		ChebyshevFitter fitter(degree);
		std::unique_ptr<PolynomialTransformation> transformation = fitter.fit(
			predicted_reduced.template begin<typename ListA::peak_type::MassGetter>(),
			predicted_reduced.template end<typename ListA::peak_type::MassGetter>(),
			measured_reduced.template begin<typename ListB::peak_type::MassGetter>(),
			measured_reduced.template end<typename ListB::peak_type::MassGetter>());
		
		double max_error = fitter.getMaximumError(); // TODO make this available to the outside

		// now modify the new peaklist according to the transformation
		typename ListB::iterator it = new_measured.begin();
		for ( ; it != new_measured.end(); ++it) {
			it->setMass(transformation->transform(it->getMass()));
		}
	}
	// TODO make information available that no recalibration was done
	return new_measured;
}

} // namespace ims

#endif
