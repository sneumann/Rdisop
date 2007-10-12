#ifndef IMS_MATHUTILS_H
#define IMS_MATHUTILS_H

namespace ims {

/**
 * Returns the greatest common divisor (gcd) of two numbers
 * by applying the Euclidean algorithm.
 *
 * @param a A number.
 * @param b A number.
 * @return The greatest common divisor.
 * @see gcd(T a, T b, T& a1, T& b1)
 */
template <typename T>
T gcd(T a, T b) {
	T c;
	while (b != 0) {
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}


/**
 * Returns the greatest common divisor by
 * applying the extended Euclidean algorithm (Knuth TAoCP vol. 2, p342).
 * Calculates u1, u2 and u3 (which is returned) so that
 * a * u1 + b * u2 = u3 = gcd(a, b, u1, u2)
 *
 * @param a A number.
 * @param b A number.
 * @param u1 A reference to the number to be returned (see the above formula).
 * @param u2 A reference to the number to be returned (see the above formula).
 * @return The greatest common divisor.
 * @see gcd(T, T)
 */
template <typename T>
T gcd(T a, T b, T& u1, T& u2) {
	u1 = 1;
	u2 = 0;
	T u3 = a;

	T v1 = 0;
	T v2 = 1;
	T v3 = b;

	while (v3 != 0) {
		T q = u3/v3;
		T t1 = u1-v1*q;
		T t2 = u2-v2*q;
		T t3 = u3-v3*q;

		u1 = v1;
		u2 = v2;
		u3 = v3;

		v1 = t1;
		v2 = t2;
		v3 = t3;
	}

	return u3;
}

} // namespace ims

#endif // IMS_MATHUTILS_H
