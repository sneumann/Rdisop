#ifndef IMS_DISTRIBUTEDALPHABET_H
#define IMS_DISTRIBUTEDALPHABET_H

#include <map>
#include <string>

#include <ims/alphabet.h>
#include <ims/distributeddata.h>
#include <ims/base/parser/distributedalphabetparser.h>

namespace ims {

/**
 * @brief Presents an extension to @c Alphabet which elements have a certain 
 * distribution.
 * 
 * Extends @c Alphabet where elements are distributed with certain 
 * probabilities over the alphabet. One can get and set the probability 
 * for every single element as well as check the overall alphabet
 * distribution on correctness (i.e. on equality to one the sum of distribution
 * probabilities).
 *
 * To load data from file, default function @c load(str::string& fname) 
 * can be used. Then elements have to be stored in a flat file @c fname 
 * in a predefined format. To read more on this format, please, 
 * @see AlphabetParser. If one wants to load data stored differently or 
 * in its own file format (i.e. xml) one has to define a new parser derived 
 * from @c DistributedAlphabetParser and pass its pointer together with the 
 * file name to function @c load(const std::string& fname, 
 * AlphabetParser<>* parser). If there is any error happened while loading 
 * data, @c IOException will be thrown. 
 */
class DistributedAlphabet : public Alphabet,
			public DistributedData<Alphabet::name_type> {
	public:
		
		/**
		 * Type of container for distribution of elements.
		 */
		typedef std::map<name_type, double> container;
		
		/**
		 * Iterator for elements container.
		 */
		typedef container::iterator iterator;
		
		/**
		 * Constant iterator for elements container.
		 */
		typedef container::const_iterator const_iterator;

		/**
		 * The precision value that is used to calculate 
		 * the distribution correctness.
		 */
		static const double DISTRIBUTION_PRECISION;

		/**
		 * Returns true if the distribution correct 
		 * (i.e. equal one with a certain precision), false - otherwise.
		 */
		virtual bool isDistributionCorrect() const;

		/**
		 * @see DistributedData<std::string>::normalizeDistribution()
		 */
		virtual void normalizeDistribution();

		/**
		 * @see DistributedData<std::string>::getProbability(
		 * 								const std::string& name) const
		 */
		virtual double getProbability(const name_type& name) const;

		/**
		 * Gets the probability of the element with the index @c index.
	 	*
	 	* @param index Index of the element.
	 	* @return Probability of the element.
	 	*/
		virtual double getProbability(size_type index) const;

		/**
		 * @see DistributedData<std::string>::setProbability(
		 * 			const std::string& name, double probability) const
		 */
		virtual void setProbability(const name_type& name, double probability);

		/**
		 * Adds new element with symbol @c name, mass @c value and 
		 * @c probability.
		 *
		 * @param name The name of the element to be added.
		 * @param value The mass of the element to be added.
		 * @param probability The probability of the element to be added
		 */
		void push_back(const name_type& name, mass_type value, 
						double probability);

		/**
		 * @see Alphabet<RealType, ScaledType>::load(const std::string& name)
		 */
		virtual void load(const std::string& name) /*throw (IOException)*/;

		/**
		 * @see Alphabet<RealType, ScaledType>::load(const std::string& name, 
		 * 											AlphabetParser<>* parser)
		 */
		virtual void load(const std::string& name, AlphabetParser<>* parser) 
														/*throw (IOException)*/;

		virtual ~DistributedAlphabet() {}
	private:
		/**
		 * Distribution of alphabet elements.
		 */
		container elementsDistribution;

		/**
		 * Sets the distribution of alphabet elements.
		 *
		 * @param The distribution of alphabet elements.
		 */
		void setDistribution(const container& elementsDistribution);

		/**
		 * Gets sum of the elements distributions.
		 *
		 * @return Sum of the elements distributions
		 */
		double getDistributionSum() const;
};

} // namespace ims

#endif // IMS_DISTRIBUTEDALPHABET_H
