#ifndef IMS_DISTRIBUTEDATA_H
#define IMS_DISTRIBUTEDATA_H

namespace ims {

/**
 * An abstract interface for distributed objects.
 * Presents an abstract interface for objects that are distributed. To get the
 * probability with which the object element appears, one can use the function
 * @c getProbability(T& name) with element attribute @c name as a parameter.
 * There is functionality to check whether the whole object distribution is correct.
 */
template <typename T>
class DistributedData {
public:
	/**
	 * Gets the probability of the element with the attribute @c name.
	 *
	 * @param name The attribute of the element.
	 * @return The probability of the element.
	 */
	virtual double getProbability(const T& name) const = 0;

	/**
	 * Sets the probability @c probability for the element with the attribute @c name.
	 *
	 * @param name The attribute of the element.
	 * @param probabilty The probability of the element to be set.
	 */
	virtual void setProbability(const T& name, double probability) = 0;

	/**
	 * Returns true if the overall elements distribution is equal one, false - otherwise.
	 *
	 * @return true if the overall elements distribution is equal one, false - otherwise.
	 */
	virtual bool isDistributionCorrect() const = 0;

	/**
	 * Normalizes elements distribution, i.e. scales the elements probabilities that the overall sum is equal 1.
	 */
	virtual void normalizeDistribution() = 0;

	/**
	 * Default virtual destructor.
	 */
	virtual ~DistributedData(){}
};

}

#endif // IMS_DISTRIBUTEDATA_H
