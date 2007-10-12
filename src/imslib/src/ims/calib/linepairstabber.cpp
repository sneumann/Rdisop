/**
 * @file linepairstabber.cpp Maximum Line Pair Stabbing
 *
 * Solution to the "Maximum Line Pair Stabbing Problem": Given a set of points
 * in Euclidean 2D space, return the two parallel lines with distance epsilon
 * which contain the maximum number of points in the area bounded by those
 * two lines.
 *
 * The solution implemented so far does not exactly solve the problem above, but
 * a variant where the two lines don't have distance epsilon but instead where their
 * ordinates differ by epsilon. So the higher the absolute slope the smaller the
 * actual distance between the lines.
 *
 * @author Marcel Martin (Marcel.Martin@CeBiTec.Uni-Bielefeld.DE)
 *
*/

#include <utility>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <ims/calib/linepairstabber.h>

namespace ims {
namespace LinePairStabber {

/**
 * Class for event points: Those are the points at which lines intersect.
 * Since a dual representation for the problem is used the coordinates for
 * an event are (m,b) (slope and ordinate). The score should be either -1
 * or +1 to represent if this event represents a line leaving or entering
 * the active interval.
*/
struct Event {
	double m;
	double b;
	int score;
	
//	Event() : m(0), b(0), score(0) { }
	Event(double m, double b, int score) : m(m), b(b), score(score) { }
	bool operator== (const Event& ev) const {
		return m == ev.m && b == ev.b && score == ev.score;
	}
	bool operator< (const Event& ev) const {
		return (m == ev.m) ? (b < ev.b) : (m < ev.m);
	}
};


/** Helper function */
inline double sqr(double x) {
	return x*x;
}


/**
 * Solves a variant of the Maximum Line Pair Stabbing Problem.
 * epsilon is the ordinate difference of the two lines.
 *
 * TODO: The delta function should be a parameter.
 * TODO: use iterators
 * TODO: deal with special cases
 * @return the lower of the two lines as a pair where the first value
 * is the slope and the second the ordinate. The upper line has the same
 * slope but epsilon must be added to the ordinate.
 */
std::pair<double,double> stab_ordinate(const std::vector<std::pair<double,double> >& points, double epsilon)
{
	int maximum_score = -1;
	Event maximum_event(0, 0, -1);
	
	for (size_t i = 0; i < points.size(); ++i) {
		std::vector<Event> events;
		double p_x = points[i].first;
		double p_y = points[i].second;
		//cout << "p[" << i << "] = " << p_x << "," << p_y << endl;
		for (size_t j = 0; j < points.size(); ++j) {
			if (i == j)
				continue;
			double q_x = points[j].first;
			double q_y = points[j].second;
			//cout << "p[" << i << "] = " << p_x << "," << p_y << " with q[" << j << "] = " << q_x << "," << q_y << endl;
			/* Calculate intersection of line p* with line q* */
			double dx = p_x - q_x;
			double dy = p_y - q_y;

			/* If the slopes are equal, there's no intersection */
			if (fabs(dx) > 1e-8) { // TODO used to be: if (dx != 0)
				double m = dy / dx;
				int score;
				//assert(fabs(dx) > 1e-8);
				if (p_x > q_x) {
					score = +1;
				}
				else {
					score = -1;
				}
				events.push_back(Event(m, p_x*m - p_y, score));

				/* Calculate intersections of line p* shifted by epsilon with line q* */
				m = (p_y + epsilon - q_y) / dx;
				if (p_x > q_x) {
					score = -1;
				}
				else {
					score = +1;
				}
				events.push_back(Event(m, p_x*m - p_y, score));
			}
			
		}
		//cout << events.size() << endl;
		// sort events by x coordinate (from left to right)
		std::sort(events.begin(), events.end());
		
		// search for subsequence with highest score sum
		int cur_score = 0;
		std::vector<Event>::const_iterator cit;
		for (cit = events.begin(); cit != events.end(); ++cit) {
			assert(cur_score >= 0);
			cur_score += cit->score;
			if (cur_score > maximum_score) {
				maximum_event = *cit;
				maximum_score = cur_score;
			}
		}
	}
	//FIXME assert(maximum_score > -1); // only true if _any_ maximum was found
	return std::pair<double,double>(maximum_event.m,-maximum_event.b);
	// TODO: make the score available
	//score = maximum_score;
}

}
}

/*
	TODO: The following code contains an incomplete implementation for
	the actual "Maximum Line Pair Stabbing Problem".
	
	I've found various problems with the first implementation (which is
	still in calib), but since this has currently lower priority than
	the above version for now, it must wait.
*/
#if 0

/**
 * stabber.cpp
 *
 * Solution to the "Maximum Line Pair Stabbing Problem": Given a set of points
 * in Euclidean 2D space, return the two parallel lines with distance epsilon
 * which contain the maximum number of points in the area bounded by those
 * two lines.
 *
 * Author: Marcel Martin <Marcel.Martin@CeBiTec.Uni-Bielefeld.DE>
 *
*/

#include <utility>
#include <algorithm>
#include <math.h>
#include <ims/calib/stabber.h>
#include <iostream>

using namespace std;

/** Class for event points: Those are the points at which lines intersect.
 * Since a dual representation for the problem is used the coordinates for
 * an event are (m,b) (slope and ordinate). The score should be either -1
 * or +1 to represent if this event represents a line leaving or entering
 * the active interval.
*/
struct Event {
	Event() : m(0), b(0), score(0) { }
	Event(double m, double b, int score) : m(m), b(b), score(score) { }
	double m;
	double b;
	int score;
	bool operator== (const Event& ev) const {
		return m == ev.m && b == ev.b && score == ev.score;
	}
	bool operator< (const Event& ev) const {
		return (m == ev.m) ? (b < ev.b) : (m < ev.m);
	}
};


/** Helper function */
inline double sqr(double x) {
	return x*x;
}


/**
 * Solves the Maximum Line Pair Stabbing Problem using O(n^2 log n) time and O(n) space
 *
 * @param
 * TODO: The delta function should be a parameter.
 * - die Schleife muss ber alle Punkte gehen (also n*(m-1) innere) DONE
 * - m[2] und m[3] sind quatsch
 * - als allererstes: 
 * - we don't have to store p_x*m - p_y for each event. storing p_x and p_y or their
 *   indices in points is sufficient.
 * 
 * 
 */
LinePairStabber::LinePairStabber(const vector<pair<double,double> >& points, double epsilon)
	: epsilon(epsilon)
{
	int maximum_score = -1;
	Event maximum_event;
	
	for (size_t i = 0; i < points.size()-1; ++i) {
		vector<Event> events;
		double p_x = points[i].first;
		double p_y = points[i].second;
		//cout << "p[" << i << "] = " << p_x << "," << p_y << endl;
		for (size_t j = 0; j < points.size(); ++j) {
			if (i == j)
				continue;
			double q_x = points[j].first;
			double q_y = points[j].second;
			cout << "p[" << i << "] = " << p_x << "," << p_y << " with q[" << j << "] = " << q_x << "," << q_y << endl;
			cout.flush();
			
			/*
			The m array will contain the m coordinate of all event points.
			There will be 0, 1, 2, or 3 event points.
			*/
			double m[3];
			unsigned m_count = 0;
			
			/*
			First step: Calculate intersection of line p* with line q*
			
			There will be either one or no resulting event point.
			
			Note again that we're working with the dual representation of the point (p_x,p_y)
			as the line p*: y = p_x * m - p_y. That is, p_x is the slope and -p_y is the ordinate.
			
			TODO: How should the case be handled that the points coincide?
			
			TODO: it's probably smarter to calculate the intersection between the two lines not
			as the first, but as the second step.
			*/
			double dx = p_x - q_x;
			double dy = p_y - q_y;
			
			/* If the slopes are equal, there's no solution. */
			if (dx != 0.0) {
				m[m_count] = dy / dx;
				m_count++;
				
				int score;
				assert(fabs(dx) > 1e-8);
				
				/* compare slopes of the two lines */
				if (p_x > q_x) {
					score = +1;
				}
				else {
					score = -1;
				}
				printf("push_back (line/line): %f %f %d\n", m, p_x*m - p_y, score);
				events.push_back(Event(m, p_x*m - p_y, score));
			}

			/*
			Second step: Calculate intersections of line p* shifted by delta(m) with line q*.
			
			We try to solve the equation dx*m-dy = epsilon*sqrt(1+m*m) by squaring it and
			solving the resulting quadratic equation. Therefore we have to check each
			of the possible two solutions to the quadratic equation to see if it's also a
			solution to the original equation.
			*/
			
			/*
			t3 is the term that decides how many solutions we have since it's the one
			under the square root.
			*/
			double t3 = sqr(dx) + sqr(dy) - sqr(epsilon);
			
			if (t3 < 0.0) {
				/* no solution to the quadratic equation */
				assert(false);
			}
			else {
				/* define some temporaries for subterms */
				double t1 = 1.0/(sqr(dx) - sqr(epsilon));
				double t2 = dx*dy;
				
				if (t3 == 0.0) {
					/*
					exactly one solution to the quadratic equation
					This will most likely not happen since we're checking for exact
					floating-point equality */
					assert(false);
					/* TODO: solution is t1*t2 */
				} else {
					/* t3 > 0.0: two solutions to the quadratic equation */
					
					assert(false);
					double t4 = epsilon * sqrt(t3);
					/* TODO: solutions are t1 * (t2 + t4); t1 * (t2 - t4); */
					m[0] = t1 * (t2 + t4);
					m[1] = t1 * (t2 - t4);
					for (int k = 0; k < 2; ++k) {
						// check if this m[k] is really a solution to the original equation
						int score;
						printf("k: %d. m[k]: %f. dx: %f. dy: %f. %f\n", k, m[k], dx, dy, dx * m[k] - dy);
						if (dx * m[k] - dy > 0.0) {
							// compare slopes of the curve and q*
							if (p_x - epsilon*m[k]/sqrt(1.0 + sqr(m[k])) > q_x) {
								score = -1;
							}
							else {
								score = +1;
							}
							printf("push_back (line/delta): %f %f %d\n", m[k], p_x*m[k] - p_y, score);
							events.push_back(Event(m[k], p_x*m[k] - p_y, score));
						}
					}
				}
			}
		}
		assert(events.size() > 0);
		//cout << events.size() << endl;
		// sort events by x coordinate (from left to right)
		sort(events.begin(), events.end());
		
		// search for subsequence with highest score sum
		int cur_score = 0;
		vector<Event>::const_iterator cit;
		for (cit = events.begin(); cit != events.end(); ++cit) {
			if (cur_score < 0) {
				cur_score = 0;
			}
			cur_score += cit->score;
			if (cur_score > maximum_score) {
				maximum_event = *cit;
				maximum_score = cur_score;
			}
		}
		printf("m,b,score\n");
		for (cit = events.begin(); cit != events.end(); ++cit) {
			printf("%.5f  %.5f  %+d", cit->m, cit->b, cit->score);
			//cout << setprecision(5) << cit->m << "\t" << setprecision(5) << cit->b << "\t" << cit->score;
			if (*cit == maximum_event) {
				printf(" <======= (%d)", maximum_score);
				//cout << " <======= (" << maximum_score << ")";
			}
			//cout << "\n";
			printf("\n");
		}
		//break;
	}
	slope = maximum_event.m;
	ordinate = -maximum_event.b;
	score = maximum_score;
}
#endif
