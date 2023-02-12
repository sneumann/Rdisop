#ifndef IMS_GEOMETRICCALIBRATOR_H
#define IMS_GEOMETRICCALIBRATOR_H

#include <vector>
#include <utility>
#include <cmath>
#include <memory>

#include <ims/calib/matchmatrix.h>
#include <ims/calib/calibrator.h>

namespace ims {
// TODO not sure if inheritance is the right way to do it here

/*
 For a GeometricCalibrator, the parameters epsilon, abslimit, minscale,
 maxscale, mintranslation, maxtranslation are @b only used during the
 conversion of the two value lists to a point list, they
 are not (and cannot) be passed on to the geometric algorithms. This class
 provides the necessary methods to convert lists of values to points and to
 get a mapping out of the estimated linear transformation.

 Note that the geometric algorithms may also have parameters which may
 be called epsilon but that those are @b different
 epsilons.
 */
template <typename ListA, typename ListB=ListA>
class GeometricCalibrator : public Calibrator<ListA,ListB> {
	public:
		GeometricCalibrator(double epsilon);

		virtual int match(const ListA& a, const ListB& b);

		virtual void setAbsLimit(double limit) { abslimit = limit; }

		virtual void setScaleInterval(double min, double max) {
			maxscale = max;
			minscale = min;
		}

		virtual void setTranslationInterval(double min, double max) {
			mintranslation = min;
			maxtranslation = max;
		}

		virtual void setMinPointPairCount(size_t count) {
			min_pointpaircount = std::max((size_t)2, count);
		}

		virtual std::unique_ptr<std::map<int,int> > getMapping() const;

		virtual LinearTransformation getTransformation() const;

		virtual bool inputValid(const ListA& a, const ListB& b) const {
			(void)a; (void)b;
			return true;
		}

	protected:
		/** overwrite this */
		virtual LinearTransformation estimateLinearTransformation() = 0;
		std::vector<std::pair<double,double> > points; // TODO make this private

	private:
		void convertToPoints(const ListA& a, const ListB& b);
		std::unique_ptr<std::map<int,int> > realMatch(const ListA& a, const ListB& b, const LinearTransformation& t, double epsilon, bool restricted); // TODO parameters

		double epsilon;
		double abslimit;
		double minscale;
		double maxscale;
		double mintranslation;
		double maxtranslation;
		size_t min_pointpaircount;
		bool have_mapping;
		bool have_transformation;
		std::unique_ptr<std::map<int,int> > mapping;
		LinearTransformation transformation;
};


template <typename ListA, typename ListB>
GeometricCalibrator<ListA,ListB>::GeometricCalibrator(double epsilon) :
	epsilon(epsilon),
	abslimit(std::numeric_limits<double>::infinity()),
	minscale(-std::numeric_limits<double>::infinity()),
	maxscale(std::numeric_limits<double>::infinity()),
	mintranslation(-std::numeric_limits<double>::infinity()),
	maxtranslation(std::numeric_limits<double>::infinity()),
	min_pointpaircount(5),
	have_mapping(false),
	have_transformation(false),
	transformation(0, 0) // TODO
{
}


template <typename ListA, typename ListB>
std::unique_ptr<std::map<int,int> > GeometricCalibrator<ListA,ListB>::realMatch(const ListA& a, const ListB& b, const LinearTransformation& t, double epsilon, bool restricted)
{
	// TODO: one could optimize (similar to calc_pointlist above)
	const double accuracy = 0.0001;

	MatchMatrix mm(a.size());
	
	for (size_t i = 0; i < a.size(); i++) {
		for (size_t j = 0; j < b.size(); j++) {
			if (fabs(t.transform(a[i].getMass()) - b[j].getMass()) < epsilon+accuracy) {
				mm.set(i, j);
			}
		}
	}
	
	return restricted ? mm.countMatchesRestrictive() : mm.countMatches();
}


template <typename ListA, typename ListB>
void GeometricCalibrator<ListA,ListB>::convertToPoints(const ListA& a, const ListB& b) {
	points.clear();
	typename ListB::const_iterator b_begin = b.begin();
	typename ListA::const_iterator a_it = a.begin();
	for ( ; a_it != a.end(); ++a_it) {
		// first interval: [min, max]
		double min = minscale * a_it->getMass() + mintranslation - epsilon;
		double max = maxscale * a_it->getMass() + maxtranslation + epsilon;
		//TODO: uncomment, test
		// second interval is: [a-abslimit, a+abslimit]
		/*
		// now intersect the two intervals
		//min = std::max(min, *it_a - abslimit);
		//max = std::min(max, *it_a + abslimit);
		*/
		bool foundpair = false;
		typename ListB::const_iterator b_it = b_begin;
		for (; b_it != b.end(); ++b_it) {
			if (fabs(a_it->getMass() - b_it->getMass()) <= abslimit) {
				// remember position of first pair found, we can start from here
				// next time because A is sorted
				if (!foundpair) {
					b_begin = b_it;
					foundpair = true;
				}
				if (min <= b_it->getMass() && b_it->getMass() <= max) {
					points.push_back(std::make_pair(a_it->getMass(), b_it->getMass()));
				}
			} else {
				if (foundpair) {
					// we are out of range now
					break;
				}
			}
		}
	}
}


template <typename ListA, typename ListB>
int GeometricCalibrator<ListA,ListB>::match(const ListA& a, const ListB& b)
{
	convertToPoints(a, b);
	size_t n = points.size();
	
	if (n < min_pointpaircount) {
		// TODO throw instead or return -1 or sth.
		//logger(Details)<<"Not calculating matching: #pointpairs<" << min_pointpaircount << std::endl;
		return 0;
	}
	
	transformation = estimateLinearTransformation();
	have_transformation = true;
	mapping = realMatch(a, b, transformation, epsilon, true); // TODO: hardcoded
	have_mapping = true;
	return mapping->size();
}

// TODO a bit ugly
template <typename ListA, typename ListB>
std::unique_ptr<std::map<int,int> > GeometricCalibrator<ListA,ListB>::getMapping() const {
	assert(have_mapping);
	return std::unique_ptr<std::map<int,int> >(new std::map<int,int>(*mapping));
}

template <typename ListA, typename ListB>
LinearTransformation GeometricCalibrator<ListA,ListB>::getTransformation() const {
	assert(have_transformation); // TODO a bit ugly
	return transformation;
}

} // namespace ims

#endif
