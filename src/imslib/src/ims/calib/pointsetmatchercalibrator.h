#ifndef IMS_POINTSETMATCHERCALIBRATOR_H
#define IMS_POINTSETMATCHERCALIBRATOR_H

#include <map>
#include <algorithm>
#include <memory>

#include <ims/logger.h>
#include <ims/transformation.h>
#include <ims/calib/linearpointsetmatcher.h>
#include <ims/calib/calibrator.h>

namespace ims {

template <typename ListA, typename ListB=ListA>
class PointSetMatcherCalibrator : public Calibrator<ListA,ListB> {
	public:
		PointSetMatcherCalibrator(Logger& logger, double epsilon, bool oneToOne, bool restrict_oneToOne);
		virtual void setAbsLimit(double limit);
		virtual void setScaleInterval(double min, double max);
		virtual void setTranslationInterval(double min, double max);
		virtual void setMinPointPairCount(size_t count);
		virtual bool inputValid(const ListA& a, const ListB& b) const;
		virtual int match(const ListA& a, const ListB& b);
		virtual std::unique_ptr<std::map<int,int> > getMapping() const;
		virtual LinearTransformation getTransformation() const;
	private:
		Logger& logger;
		LinearPointSetMatcher lpsm;
		size_t min_pointpaircount;
};

// TODO is this function necessary?
template <typename ListA, typename ListB>
bool pointlist_size_greater_equal(int min_size, const ListA& a, const ListB& b, double abslimit, double minscale, double maxscale, double mintranslation, double maxtranslation, double epsilon)
{
	int count = 0;
	typename ListB::const_iterator b_begin = b.begin();
	typename ListA::const_iterator a_it = a.begin();
	for (; a_it != a.end(); ++a_it) {
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
		for ( ; b_it != b.end(); ++b_it) {
			if (fabs(a_it->getMass() - b_it->getMass()) <= abslimit) {
				// remember position of first pair found, we can start from
				// here next time because A is sorted
				if (!foundpair) {
					b_begin = b_it;
					foundpair = true;
				}
				if (min <= b_it->getMass() && b_it->getMass() <= max) {
					if (++count >= min_size) return true;
				}
			} else {
				if (foundpair) {
					// we are out of range now
					break;
				}
			}
		}
	}
	return false;
}


template <typename ListA, typename ListB>
PointSetMatcherCalibrator<ListA,ListB>::PointSetMatcherCalibrator(Logger& logger, double epsilon, bool oneToOne, bool restrict_oneToOne):
	logger(logger),
	lpsm(logger, epsilon, oneToOne, restrict_oneToOne),
	min_pointpaircount(5)
{
}


template <typename ListA, typename ListB>
void PointSetMatcherCalibrator<ListA,ListB>::setAbsLimit(double limit) {
	lpsm.setAbsLimit(limit);
}


template <typename ListA, typename ListB>
void PointSetMatcherCalibrator<ListA,ListB>::setScaleInterval(double min, double max) {
	lpsm.setScaleInterval(min, max);
}


template <typename ListA, typename ListB>
void PointSetMatcherCalibrator<ListA,ListB>::setTranslationInterval(double min, double max) {
	lpsm.setTranslationInterval(min, max);
}


template <typename ListA, typename ListB>
void PointSetMatcherCalibrator<ListA,ListB>::setMinPointPairCount(size_t count) {
	min_pointpaircount=std::max((size_t)2, count);
}


template <typename ListA, typename ListB>
bool PointSetMatcherCalibrator<ListA,ListB>::inputValid(const ListA& a, const ListB& b) const {
	std::pair<double,double> s = lpsm.getScaleInterval();
	std::pair<double,double> t = lpsm.getTranslationInterval();
	double e = lpsm.getEpsilon();
	double absl = lpsm.getAbsLimit();
	bool enough = pointlist_size_greater_equal(min_pointpaircount, a, b, absl, s.first, s.second, t.first, t.second, e);
	
	if (!enough) {
		logger(Details)<<"Not calculating matching: #pointpairs<" << min_pointpaircount << std::endl;
		return false;
	}
	return true;
}


template <typename ListA, typename ListB>
int PointSetMatcherCalibrator<ListA,ListB>::match(const ListA& a, const ListB& b) {
	return lpsm.match(
		a.template begin<typename ListA::peak_type::MassGetter>(),
		a.template end<typename ListA::peak_type::MassGetter>(),
		b.template begin<typename ListA::peak_type::MassGetter>(),
		b.template end<typename ListA::peak_type::MassGetter>());
}


template <typename ListA, typename ListB>
std::unique_ptr<std::map<int,int> > PointSetMatcherCalibrator<ListA,ListB>::getMapping() const {
	return lpsm.getMapping();
}


template <typename ListA, typename ListB>
LinearTransformation PointSetMatcherCalibrator<ListA,ListB>::getTransformation() const {
	return lpsm.getTransformation();
}

} // namespace ims

#endif
