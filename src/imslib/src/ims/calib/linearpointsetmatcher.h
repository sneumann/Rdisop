#ifndef IMS_LINEARPOINTSETMATCHER_H
#define IMS_LINEARPOINTSETMATCHER_H

#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <memory>
#include <algorithm>

#include <ims/logger.h>
#include <ims/transformation.h>
#include <ims/calib/matchmatrix.h>

namespace ims {

/**
 * linearpointsetmatcher.h
 *
 * Author: Tobias Marschall <Tobias.Marschall@CeBiTec.Uni-Bielefeld.DE>
 *
 * TODO: reconsider "logger vs runtime" question
 * TODO: This one deserves a better name... :-)
 * TODO: Internal class, hide somehow? but HOW???
 *
 * @ingroup recalibration
 */
class RepresentativeScale {
public:
	// TODO: shouldn't be public
	// Represents a begin/end of match range of A[k]->B[l]
	int k,l;

	double scale;
	bool end;
	bool operator<(const RepresentativeScale &y) const;
};


/**
 * Class to solve the "Linear Point Set Matching Problem": Given two sets of
 * points, lets say A and B, find the optimal linear transformation which
 * maps the points in A within epsilon distance to as many points in B as
 * possible.
 *
 * The "one-to-one"-variant of this problem is also implemented: Find the
 * optimal linear transformation which maps as many points in A into epsilon
 * distance to different points in B.
 *
 * @ingroup recalibration
 */
class LinearPointSetMatcher {
public:
	/** Constructs LinearPointSetMatcher.
	  * @param logger Log facility.
	  * @param epsilon Two points from A and B match iff |f(A[i])-B[j]| <= epsilon.
	  * @param oneToOne Calculate one-to-one instead of many-to-one mappings.
	  * @param restrict_oneToOne If oneToOne is true, mapping can be further restricted to
	  *   only count matches which are non-ambiguous.
	  */
	LinearPointSetMatcher(Logger& logger, double epsilon, bool oneToOne, bool restrict_oneToOne) :
		logger(logger),
		epsilon(epsilon),
		abslimit(std::numeric_limits<float>::infinity()),
		maxscale(std::numeric_limits<float>::infinity()),
		minscale(-std::numeric_limits<float>::infinity()),
		maxtranslation(std::numeric_limits<float>::infinity()),
		mintranslation(-std::numeric_limits<float>::infinity()),
		do_verification(false),
		oneToOne(oneToOne),
		restrict_oneToOne(restrict_oneToOne) {}
	
	/** Set absolute limit.
	  *
	  * Must not be confused with neither epsilon nor translationlimit. Abslimit restricts the
	  * space of point-pairs: only matches for which |A[i]-B[j]|<=abslimit is true are considered.
	  * Epsilon defines the optimization criterion: Find a transformation f for which
	  * |f(A[i])-B[j]|<=epsilon is true for most pairs (i,j).
	  * Translationlimit limits the search-space: Only transformations f(x)=s*x+t are considered,
	  * for which t is in [t_min,t_max].
	  */
	void setAbsLimit(double limit);

	/** Unset absolute limit. */
	void unsetAbsLimit();

	/** Set translation limit to restrict the search space.
	  *
	  * Only transformations f(x)=s*x+t are considered, for which t is in [min,max].
	  * Must not be confused with neither epsilon nor abslimit. See setAbsLimit() for details.
	  */
	void setTranslationInterval(double min, double max);

	/** Unset translation limit, equivalent to setTranslationInterval(-inf,inf). */
	void unsetTranslationInterval();

	/** Returns translationlimit: Interval [pair.first, pair.second]. */
	std::pair<double,double> getTranslationInterval() const;

	/** Get absolute limit.
	  * @returns Returns infinity if abslimit wasn't set.
	  */
	double getAbsLimit() const;

	/** Set scale interval to restrict the search-space.
	  * Only transformations f(x)=s*x+t are considered, for which s is in [min,max].
	  * @param min Minimal scale.
	  * @param max Maximal scale.
	  */
	void setScaleInterval(double min, double max);

	/** Unsets scale interval, equivalent to setScaleInterval(-inf,inf). */
	void unsetScaleInterval();

	/** @return scale interval: Interval [pair.first, pair.second]. */
	std::pair<double,double> getScaleInterval() const;

	/** Gets epsilon. */
	double getEpsilon() const;

	/** @return true iff one-to-one mapping is turned on. */
	bool one2One() const; // TODO rename oneToOne

	/** @return true iff restricted one-to-one mapping is turned on. */
	bool restrictOne2One() const; // TODO rename? sth. like uniqueOneToOne?

	/** Calculates the best linear transformation f mapping A to B, such that
	  * |{(i,j): |f(A[i])-B[j]|<=epsilon}| is maximized.
	  * The search-space is eventually limited by abslimit and scalelimit.
	  * @param a_first Start of list A. 
	  * @param a_last End of list A.
	  * @param b_first Start of list B.
	  * @param b_last End of list B.
	  */
	// TODO: maybe use references? e.g. const RandomAccessIterator& a_first
	// TODO a_first => a_begin (or just a to make the code easier to read)
	// and a_last => a_end (which is more precise)
	template <typename RandomAccessIterator>
	int match(RandomAccessIterator a_first, RandomAccessIterator a_last, RandomAccessIterator b_first, RandomAccessIterator b_last);

	/** Returns the actual mapping computed by the last call to match().
	  * @b Only usable for one-to-one mappings.
	  */
	// TODO: do we need something like this for the many2one case?
	// TODO: int,int -> size_t,size_t
	std::unique_ptr<std::map<int,int> > getMapping() const;

	/** Returns the transformation previously computed by match().
	  * Apply this transformation to A to map it to B.
	  */
	LinearTransformation getTransformation() const;

protected:
	Logger &logger;
	double epsilon;
	double abslimit;
	double maxscale, minscale;
	double maxtranslation, mintranslation;
	bool do_verification;
	bool oneToOne;
	bool restrict_oneToOne;

	void swap(double& d1, double& d2); // TODO what is this useful for? why not use std::swap?

	// temp variables used by functions match and countMatches
	struct {
		int bestscore, centerA, centerB;
		double bestscale,besttranslation;
		std::unique_ptr<std::map<int,int> > mapping;
	} results;


	/** Count out matches in many-to-one case, when A[i] is mapped to B[j]+diff (e.g. diff=epsilon or diff=-epsilon). */
	template <typename RandomAccessIterator>
	void countMatchesManyToOne(
		RandomAccessIterator a_first, RandomAccessIterator a_last, RandomAccessIterator b_first, RandomAccessIterator b_last,
		const std::vector<RepresentativeScale>& v, int i, int j, float diff
	);
	/** Count out matches in one-to-one case, when A[i] is mapped to B[j]+diff (e.g. diff=epsilon or diff=-epsilon). */
	template <typename RandomAccessIterator>
	void countMatchesOneToOne(
		RandomAccessIterator a_first, RandomAccessIterator a_last, RandomAccessIterator b_first, RandomAccessIterator b_last,
		const std::vector<RepresentativeScale>& v, int i, int j, float diff
	);
};

template <typename RandomAccessIterator>
void LinearPointSetMatcher::countMatchesManyToOne(
	RandomAccessIterator a_first,
	RandomAccessIterator a_last,
	RandomAccessIterator b_first,
	RandomAccessIterator b_last,
	const std::vector<RepresentativeScale>& v, int i, int j, float diff)
{
	int score = 0;

	// Step 1: Find initial score value
	// find out, which B[x] are in epsilon distance to f(A[i]):=B[j]+diff
	// side condition: abslimit
	RandomAccessIterator it = b_first;
	// iterate over B
	for (int l=0; it != b_last; it++,l++) {
		if (l == j) {
			// this matches by definition (see above)
			score++;
		} else {
			if ( (fabs(b_first[j]+diff-(*it)) <= epsilon) && (fabs(a_first[i]-*(it)) <= abslimit) ) score++;
		}
	}

	// Step 2: Find best scale by going through the list of possible scales
	std::vector<RepresentativeScale>::const_iterator p;
	for (p = v.begin(); p != v.end(); ++p) {
		if (p->end) {
			// the end of a scale range, by crossing this scale value, one point less matches
			--score;
			#ifndef NDEBUG
			logger(Everything) << "- A[" << (p->k) << "] -> B[" << p->l << "] (scale:"<<(p->scale)<<", score: "<<score<<")"<<std::endl;
			#endif
		} else {
			// the begin of a scale range, by crossing this scale value, one point more matches
			++score;
			#ifndef NDEBUG
			logger(Everything)<<"+ A["<<(p->k)<<"] -> B["<<(p->l)<<"] (scale:"<<(p->scale)<<", score: "<<score<<")"<<std::endl;
			#endif
		}
		// do we have a new maximum score?
		if (results.bestscore < score) {
			results.bestscore = score;
			results.centerA = i;
			results.centerB = j;
			results.bestscale = p->scale;
			results.besttranslation = -results.bestscale*a_first[i] + b_first[j] + diff;
		}
	}
}

template <typename RandomAccessIterator>
void LinearPointSetMatcher::countMatchesOneToOne(
	RandomAccessIterator a_first, RandomAccessIterator a_last, RandomAccessIterator b_first, RandomAccessIterator b_last,
	const std::vector<RepresentativeScale>& v, int i, int j, float diff
) {
	int m = a_last - a_first;

	// match_matrix is updated during traversal through representative scales
	MatchMatrix match_matrix(m);

	// Step 1: Initialize match matrix
	// find out, which B[x] are in epsilon distance to f(A[i]):=B[j]+diff
	// side condition: abslimit
	RandomAccessIterator it = b_first;
	// iterate over B
	for (int l=0; it != b_last; it++,l++) {
		if (l==j) {
			// this matches by definition (see above)
			match_matrix.set(i,l);
		} else {
			if ((fabs(b_first[j]+diff-(*it)) <= epsilon) && (fabs(a_first[i]-(*it))<=abslimit)) {
				match_matrix.set(i,l);
			};
		}
	}

	// Step 2: Go through list of possible scales, successively updating match matrix and computing actual score
	std::vector<RepresentativeScale>::const_iterator p;
	for (p=v.begin();p!=v.end();p++) {
		if (p->end) {
			// the end of a scale range, by crossing this scale value, one point less matches
			match_matrix.unset(p->k,p->l);
			#ifndef NDEBUG
			logger(Everything)<<"- A["<<(p->k)<<"] -> B["<<(p->l)<<"] (scale:"<<(p->scale)<<", score: ";
			#endif
		} else {
			// the begin of a scale range, by crossing this scale value, one point more matches
			match_matrix.set(p->k,p->l);
			#ifndef NDEBUG
			logger(Everything)<<"+ A["<<(p->k)<<"] -> B["<<(p->l)<<"] (scale:"<<(p->scale)<<", score: ";
			#endif
		}

		std::unique_ptr<std::map<int,int> > mapping(new std::map<int,int>());
		// evaluate match matrix: count out score using...
		if (!restrict_oneToOne) {
			// ... greedy counting scheme
			mapping=match_matrix.countMatches();
		} else {
			// ... restricted counting scheme to avoid ambiguous matches
			mapping=match_matrix.countMatchesRestrictive();
		}

		int score = mapping->size();
		#ifndef NDEBUG
		logger(Everything)<<score<<")"<<std::endl;
		#endif

		// new highscore? :-)
		if (results.bestscore < score) {
			results.bestscore = score;
			results.centerA = i;
			results.centerB = j;
			results.bestscale = (*p).scale;
			results.besttranslation=-results.bestscale*a_first[i] + b_first[j] + diff;
			results.mapping=std::move(mapping);
		}
	}
}


template <typename RandomAccessIterator>
int LinearPointSetMatcher::match(RandomAccessIterator a_first, RandomAccessIterator a_last, RandomAccessIterator b_first, RandomAccessIterator b_last) {
	int i, j, k, l;
	int m = a_last - a_first;
	int n = b_last - b_first;
	std::vector<RepresentativeScale> v;
	RepresentativeScale rs;

	// clear results
	results.bestscore = 0;
	results.centerA = -1;
	results.centerB = -1;
	results.bestscale = 0.0;
	results.besttranslation = 0.0;
	if (oneToOne) {
		results.mapping = std::unique_ptr<std::map<int,int> >(new std::map<int,int>);
	} else {
		results.mapping = std::unique_ptr<std::map<int,int> >(new std::map<int,int>());
	}

	#ifndef NDEBUG
	logger(Everything)<<"Compare: m="<<m<<", n="<<n<<std::endl;;
	#endif
	// The outer (double-)loop, iterate over all pairs (A[i],B[j])
	for (i=0; i<m; i++) {
		for (j=0; j<n; j++) {
			// verify if abslimit-condition is satisfied
			if (fabs(a_first[i] - b_first[j]) > abslimit) continue;
			// verity if scalelimit+translationlimit condition can (theoretically) still be satisfied
			if (b_first[j] < minscale*a_first[i]+mintranslation-epsilon) continue;
			if (b_first[j] > maxscale*a_first[i]+maxtranslation+epsilon) continue;

			// part a): consider transformations which map A[i] to B[j]-epsilon
			// to simplify further comments the condition f(A[i])=B[j]-epsilon will be referred to as (*)
			#ifndef NDEBUG
			logger(Everything)<<"Investigating A["<<i<<"] -> B["<<j<<"]-epsilon"<<std::endl;
			logger(Everything)<<"Populating list of RepresentativeScales"<<std::endl;
			#endif
			v.clear();

			// Looking for a linear transformation, we just have one degree of
			// freedom left (since we have to meet condition (*) ).
			// For all pairs (A[k],B[l]) we calculate an interval in which 'scale' has
			// to lie in order to satisfy the condition |f(A[k])-B[l]|<=epsilon
			// side conditions: scalelimit, abslimit
			for (k=0; k<m; k++) {
				// k==i makes no sense because of (*)
				if (k==i)
					continue;
				for (l=0; l<n; l++) {
					// respect abslimit
					if (fabs(a_first[k]-b_first[l]) > abslimit)
						continue;
					// calculate gradient for our transformation
					double scale1 = (b_first[l]-b_first[j])/(a_first[k]-a_first[i]);
					double scale2 = (b_first[l]-b_first[j]+2*epsilon)/(a_first[k]-a_first[i]);

					if (k<i) swap(scale1,scale2); // k<i means also A[k]<A[i], because A ist monotonic

					// respect scalelimit
					if (scale1 > maxscale) {
						if (k>i) {
							// A[k]-A[i] is positive, scale1 will grow monotone now
							break;
						} else {
							// A[k]-A[i] is negative
							continue;
						}
					}
					//TODO: Maybe introduce a class interval or something else to handle intersections more convenient
					scale1=std::max(scale1, minscale);
					scale2=std::min(scale2, maxscale);
					// respect translationlimit
					double s1=(b_first[j]-epsilon-maxtranslation)/a_first[i];
					double s2=(b_first[j]-epsilon-mintranslation)/a_first[i];
					scale1=std::max(scale1, s1);
					scale2=std::min(scale2, s2);
					// after considering scalelimit, is there still a proper interval to store into our list?
					if (scale1 <= scale2) {
						#ifndef NDEBUG
						logger(Everything)<<"A["<<k<<"]->B["<<l<<"] for scale in ["<<scale1<<".."<<scale2<<"]"
							<<" (trans_limit ==> s in ["<<s1<<".."<<s2<<"])" << std::endl;
						#endif
						rs.l=l;
						rs.k=k;
						rs.scale = scale1;
						rs.end = false;
						v.push_back(rs);
						rs.scale = scale2;
						rs.end = true;
						v.push_back(rs);
					}
				}
			}
			// sort list of possible scales
			sort(v.begin(), v.end());
			// evaluate the list according to chosen strategy
			// this is the single point where one-to-one and many-to-one case differ
			if (oneToOne) {
				countMatchesOneToOne(a_first, a_last, b_first, b_last, v, i, j, -epsilon);
			} else {
				countMatchesManyToOne(a_first, a_last, b_first, b_last, v, i, j, -epsilon);
			}

			// part b): consider transformations which map A[i] to B[j]+epsilon
			// TODO: it follows almost the same code as above --> use function or template or whatsoever
			// ----dupe start, for comments see above... ---------------------------------------
			#ifndef NDEBUG
			logger(Everything)<<"Investigating A["<<i<<"] -> B["<<j<<"]+epsilon"<<std::endl;
			logger(Everything)<<"Populating list of RepresentativeScales"<<std::endl;
			#endif
			v.clear();
			for (k=0; k<m; k++) {
				if (k!=i) {
					for (l=0; l<n; l++) {
						if (fabs(a_first[k]-b_first[l]) <= abslimit) {
							double scale1 = (b_first[l]-b_first[j]-2*epsilon)/(a_first[k]-a_first[i]);
							double scale2 = (b_first[l]-b_first[j])/(a_first[k]-a_first[i]);

							if (k<i) swap(scale1,scale2);

							if (scale1 > maxscale) {
								if (k>i) {
									// A[k]-A[i] is positive, scale1 will grow monotone now
									break;
								} else {
									// A[k]-A[i] is negative
									continue;
								}
							}
							scale1=std::max(scale1, minscale);
							scale2=std::min(scale2, maxscale);
							// respect translationlimit
							double s1=(b_first[j]+epsilon-maxtranslation)/a_first[i];
							double s2=(b_first[j]+epsilon-mintranslation)/a_first[i];
							scale1=std::max(scale1, s1);
							scale2=std::min(scale2, s2);
							if (scale1 <= scale2) {
								#ifndef NDEBUG
								logger(Everything)<<"A["<<k<<"]->B["<<l<<"] for scale in ["<<scale1<<".."<<scale2<<"]"
									<<" (trans_limit ==> s in ["<<s1<<".."<<s2<<"])" << std::endl;
								#endif
								rs.l=l;
								rs.k=k;
								rs.scale = scale1;
								rs.end = false;
								v.push_back(rs);
								rs.scale = scale2;
								rs.end = true;
								v.push_back(rs);
							}
						}
					}
				}
			}
			sort(v.begin(),v.end());
			if (oneToOne) {
				countMatchesOneToOne(a_first, a_last, b_first, b_last, v, i, j, +epsilon);
			} else {
				countMatchesManyToOne(a_first, a_last, b_first, b_last, v, i, j, +epsilon);
			}
			// ----dupe end -------------------------------------------------------
		}
	}
	logger(Messages)<<"score="<<results.bestscore<<", translation="<<results.besttranslation
			<<", scale="<<results.bestscale<<", i="<<results.centerA<<", j="<<results.centerB<<std::endl;

	return results.bestscore;
}

}

#endif
