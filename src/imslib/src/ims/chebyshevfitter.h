#ifndef IMS_CHEBYSHEVFITTER_H
#define IMS_CHEBYSHEVFITTER_H

#include <cmath>
#include <stdexcept>
#include <memory>

#include <ims/base/exception/exception.h>
#include <ims/transformation.h>

namespace ims {

// FIXME: getMaximumError needs a test case
class ChebyshevFitter {
	public:
		ChebyshevFitter(size_t degree) : degree(degree), has_error(false) {}
		size_t getDegree() const { return degree; }
		double getMaximumError() const;

		template <typename RandomAccessIteratorA, typename RandomAccessIteratorB>
		std::unique_ptr<PolynomialTransformation> fit(
			RandomAccessIteratorA x,
			RandomAccessIteratorA x_end,
			RandomAccessIteratorB y,
			RandomAccessIteratorB y_end);

	private:
		size_t degree;
		double error;
		bool has_error;
};


inline double ChebyshevFitter::getMaximumError() const {
	if (has_error) {
		return error;
	} else {
		throw Exception("no maximum error available");
	}
}


/*
   Code converted from the ALGOL code in
   J. Boothroyd, Algorithm 318 Chebysev curve-fit (revised).
   Comm. ACM, 10(12):801-803, 1967.
*/


/**
 * Estimates an polynomial P (whose order is determined at construction time)
 * such that the maximum error |P[x[i]]-y[i]|, for 0<=i<=n, is minimized,
 * where n=min(x.size(), y.size()). In other words: if one of the two given
 * lists is longer than the other, the supernumerous entries will be ignored.
 * Values x must be a strictly monotonic sequence.
 * Size of x and y must be > order+1.
 */
template <typename RandomAccessIteratorA, typename RandomAccessIteratorB>
std::unique_ptr<PolynomialTransformation> ChebyshevFitter::fit(
	const RandomAccessIteratorA x,
	const RandomAccessIteratorA x_end,
	const RandomAccessIteratorB y,
	const RandomAccessIteratorB y_end)
{
	const size_t m = degree;

	double *a = new double[m+2];

	size_t x_size = x_end - x;
	size_t y_size = y_end - y;

	size_t n = std::min(x_size, y_size);
	int i, j, k, mplusone, ri, ione, imax, rj, jone;
	double d, h, aione, denom, ai, rhi, xj, hmax, himax, xi, hi, abshi, nexthi, prevh, rhione;
	int *r = new int[m+2];
	double *rx = new double[m+2];
	double *rh = new double[m+2];
	mplusone = m+1;
	prevh = 0;
	// index vector for initial reference set
	r[0] = 1;
	r[mplusone] = n;
	d = (n-1)/(double)mplusone;
	h = d;
	for (i = 1; i <= (int)m; i++) {
		r[i] = int(h+1);
		h = h+d;
	}
	bool fit = false;
	while (!fit) {
		h = -1.0;
		// select m+2 reference pairs and set alternating deviation vector
		for (i=0;i<=mplusone;i++) {
			ri = r[i];
			rx[i] = x[ri-1];
			a[i] = y[ri-1];
			rh[i] = -h;
			h = -h;
		}

		// compute m+1 leading divide differences
		for (j=0; j<=(int)m; j++) {
			ione = mplusone;
			aione = a[ione];
			rhione = rh[ione];
			for (i=m; i>= j; i--) {
				denom = rx[ione]-rx[i-j];
				ai = a[i];
				rhi = rh[i];
				a[ione] = (aione-ai)/(1.0*denom);
				rh[ione] = (rhione-rhi)/(1.0*denom);
				ione = i;
				aione = ai;
				rhione = rhi;
			}
		}
		// equate (m+1)st difference to zero to determine h
		h = -a[mplusone]/(1.0*rh[mplusone]);
		// with h known, combine the function and deviation differences
		for (i=0; i<=mplusone; i++)
			a[i] = a[i] +rh[i]*h;
		// compute polynomial coefficients
		for (j=m-1; j>=0; j--) {
			xj = rx[j];
			i = j;
			ai = a[i];
			for (ione=j+1; ione<=(int)m; ione++) {
				aione = a[ione];
				a[i] = ai - xj*aione;
				ai = aione;
				i = ione;
			}
		}

		// if the reference deviation is not increasing monotonically the exit
		hmax = fabs(h);
		if (hmax <= prevh) {
			a[mplusone] = -hmax;
			fit = true;
			break;
		}

		// find the index, imax, and value, himax, of the largest
		// absolute error for all sample points
		a[mplusone] = hmax;
		prevh = hmax;
		imax = r[0];
		himax = h;
		j = 0;
		rj = r[j];
		for (i=1; i<=(int)n; i++) {
			if (i!=rj) {
				xi = x[i-1];
				hi = a[m];
				for (k=m-1; k>=0; k--)
					hi = hi*xi+a[k];
				hi = hi -y[i-1];
				abshi = fabs(hi);
				if (abshi > hmax) {
					hmax = abshi;
					himax = hi;
					imax = i;
				}
			} else if (j<mplusone) {
				++j;
				rj = r[j];
			}
		}


		// if the maximum error occurs at a nonreference point,
		// exchange this point with the nearest reference point
		// having an error of the same sign and repeat

		if (imax != r[0]) {
			for (i=0; i<=mplusone; i++) {
				if (imax < r[i])
					break;
			}

			// did we do the full loop? swap
			if (i > mplusone)
				i = mplusone;

			nexthi = (i%2==0?h:-h);
			if (himax*nexthi >= 0) {
				r[i] = imax;
			} else if (imax < r[0]) {
				jone = mplusone;
				for (j=m; j>=0; j--) {
					r[jone] = r[j];
					jone = j;
				}
				r[0] = imax;
			} else if (imax > r[mplusone]) {
				j = 0;
				for (jone = 1; jone <= mplusone; ++jone) {
					r[j] = r[jone];
					j = jone;
				}
				r[mplusone] = imax;
			} else
				r[i-1] = imax;
		} else
			break;
	}

	// Copy estimated coefficients (array a) into a PolynomialTransformation.
	std::unique_ptr<PolynomialTransformation> transformation(new PolynomialTransformation(m));
	for (size_t ic = 0; ic <= m; ++ic) {
		transformation->setCoefficient(ic, a[ic]);
	}
	this->error = hmax;
	this->has_error = true;

	delete [] a;
	delete [] r;
	delete [] rx;
	delete [] rh;

	return transformation;
}

} // namespace ims

#endif
