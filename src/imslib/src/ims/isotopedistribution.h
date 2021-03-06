#ifndef IMS_ISOTOPE_DISTRIBUTION_H
#define IMS_ISOTOPE_DISTRIBUTION_H

#include <vector>
#include <ostream>

namespace ims {

/**
 * @brief Represents a distribution of isotopes restricted to the first 
 * K elements.
 * 
 * Represents a distribution of isotopes of chemical elements as a list 
 * of peaks each as a pair of mass and abundance. @c IsotopeDistribution
 * unlike @c IsotopeSpecies has one abundance per a nominal mass.
 * Here is an example in the format (mass; abundance %) 
 * for molecule H2O (values are taken randomly):
 * 
 * - IsotopeDistribution
 * 		(18.00221; 99.03 %)
 * 		(19.00334; 0.8 %)
 * 		(20.00476; 0.17 %)
 * 
 * - IsotopeSpecies
 * 		(18.00197; 98.012 %)
 * 		(18.00989; 1.018 %)
 * 		(19.00312; 0.683 %)
 * 		(19.00531; 0.117 %)
 * 		(20.00413; 0.134 %)
 * 		(20.00831; 0.036 %)
 * 
 * To the sake of faster computations distribution is restricted 
 * to the first K elements, where K can be set by adjusting size 
 * @c SIZE of distribution. @note For the elements most abundant in
 * living beings (CHNOPS) this restriction is negligible, since abundances
 * decrease dramatically in isotopes order and are usually of no interest
 * starting from +10 isotope.
 * 
 * @c IsotopeDistribution implements folding with other distribution using an 
 * algorithm described in details in paper: 
 * Boecker et al. "Decomposing metabolic isotope patterns" WABI 2006. 
 * 
 * Folding with itself is done using Russian Multiplication Scheme.
 * 
 * @author Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>  
 */
class IsotopeDistribution {
	public:
		/**
		 * Type of isotope mass.
		 */
		typedef double mass_type;
		
		/**
		 * Type of isotope abundance.
		 */
		typedef double abundance_type;
		
		/**
		 * Type of isotope nominal mass.
		 */
		typedef unsigned int nominal_mass_type;

		/**
		 * Structure that represents an isotope peak - pair of mass and abundance.
		 */
		struct Peak {
			Peak(mass_type mass = 0.0, abundance_type abundance = 0.0) : 
										mass(mass), abundance(abundance) { }
			bool operator ==(const Peak& peak) const {
				return (peak.mass == mass && peak.abundance == abundance);
			}
			mass_type mass;
			abundance_type abundance;
		};
		
		/**
		 * Type of isotope peak.
		 */
		typedef Peak peak_type;
		
		/**
		 * Type of container to store peaks.
		 */
		typedef std::vector<peak_type> peaks_container;
		
		/**
		 * Type of iterator over container with peaks.
		 */
		typedef peaks_container::iterator peaks_iterator;
		
		/**
		 * Type of const iterator over container with peaks.
		 */
		typedef peaks_container::const_iterator const_peaks_iterator;
		
		/**
		 * Type of peaks container's size.
		 */
		typedef peaks_container::size_type size_type;

		/**
		 * Type of container with isotope masses.
		 */
		typedef std::vector<mass_type> masses_container;
		
		/**
		 * Type of iterator over container with isotope masses.
		 */
		typedef masses_container::iterator masses_iterator;
		
		/**
		 * Type of const iterator over container with isotope masses.
		 */
		typedef masses_container::const_iterator const_masses_iterator;

		/**
		 * Type of container with isotope abundances.
		 */
		typedef std::vector<abundance_type> abundances_container;
		
		/**
		 * Type of iterator over container with isotope abundances.
		 */
		typedef abundances_container::iterator abundances_iterator;
		
		/**
		 * Type of const iterator over container with isotope abundances.
		 */		
		typedef abundances_container::const_iterator const_abundances_iterator;

		/**
		 * Error to be allowed for isotope distribution.
		 */
		static abundance_type ABUNDANCES_SUM_ERROR;
		
		/**
		 * Length of isotope distribution.
		 */
		static size_type SIZE;

		/**
		 * Constructor with nominal mass.
		 */
		IsotopeDistribution(nominal_mass_type nominalMass = 0) :
					nominalMass(nominalMass) {}

		/**
		 * Constructor with single isotope.
		 */
		IsotopeDistribution(mass_type mass);

		/**
		 * Constructor with isotopes and nominal mass.
		 */
		IsotopeDistribution(const peaks_container& peaks,
				    nominal_mass_type nominalMass = 0) :
					peaks(peaks),
					nominalMass(nominalMass) {}

		/**
		 * Copy constructor.
		 */
		IsotopeDistribution(const IsotopeDistribution& distribution) :
					peaks(distribution.peaks),
					nominalMass(distribution.nominalMass) {}

		/**
		 * Destructor.
		 */
		~IsotopeDistribution() {}

		/**
		 * Gets size of isotope distribution. @note Size is not smaller than
		 * predefined @c SIZE.
		 * 
		 * @return Size of isotope distribution.
		 */
		size_type size() const { return std::min(peaks.size(), SIZE); }

		/**
		 * Assignment operator.
		 * 
		 * @param distribution Isotope distribution to be assigned to this one.
		 * @return Reference to this object.
		 */
		IsotopeDistribution& operator =(
									const IsotopeDistribution& distribution);

		/**
		 * Equality operator. Returns true, if a given @c distribution is equal
		 * to this one, false - otherwise.
		 * 
		 * @return true, if a given distribution is equal to this distribution,
		 * 		   false - otherwise
		 */
		bool operator ==(const IsotopeDistribution& distribution) const;
		
		/**
		 * Inequality operator. Returns true, if a given @c distribution is 
		 * unequal to this one, false - otherwise.
		 * 
		 * @return true, if a given distribution is unequal to this 
		 * 		   distribution, false - otherwise
		 */		
		bool operator !=(const IsotopeDistribution& distribution) const;

		/**
		 * Operator for folding this distributoin with a given @c distribution.
		 * @note Operator is unary, so result is stored in this 
		 * object itself.
		 * 
		 * @param distribution Distribution to be folded with this one.
		 * @return Reference to this object.
		 * 
		 * @see IsotopeDistribution& operator *=(unsigned int)
		 */
		IsotopeDistribution& operator *=(
								const IsotopeDistribution& distribution);
								
		/**
		 * Operator for folding this distribution with itself @c pow times.
		 * @note Operator is unary, so result is stored in this object itself.
		 * 
		 * @param pow Number of times this distribution is to be folded with
		 * 															itself.
		 * @return Reference to this object.
		 * 
		 * @see IsotopeDistribution& operator *=(const IsotopeDistribution&)
		 */
		IsotopeDistribution& operator *=(unsigned int pow);

		/**
		 * Gets a mass of isotope @c i.
		 * 
		 * @param i An index of isotope.
		 * @return Mass of isotope @c i.
		 */
		mass_type getMass(size_type i) const 
						{ return peaks[i].mass + nominalMass + i; }

		/**
		 * Gets an abundance of isotope @c i.
		 * 
		 * @param i An index of isotope.
		 * @return An abundance of isotope @c i.
		 */
		abundance_type getAbundance(size_type i) const 
										{ return peaks[i].abundance; }
						
		/**
		 * Gets an average mass of all isotopes.
		 * 
		 * @return An average mass of all isotopes.
		 */
		mass_type getAverageMass() const;
		
		/**
		 * Gets a nominal mass of distribution.
		 * 
		 * @return A nominal mass of distribution.
		 */
		nominal_mass_type getNominalMass() const { return nominalMass; }
		
		/**
		 * Sets a nominal mass for distribution.
		 * 
		 * @param A new nominal mass for distributoin.
		 */
		void setNominalMass(nominal_mass_type nominalMass) 
									{ this->nominalMass = nominalMass; }

		/**
		 * Gets masses of isotopes.
		 * 
		 * @return Masses of isotopes.
		 */
		masses_container getMasses() const;

		/**
		 * Gets abundances of isotopes.
		 * 
		 * @return Abundances of isotopes.
		 */
		abundances_container getAbundances() const;

		/**
		 * Normalizes distribution, 
		 * i.e. scaling abundances to be summed up to 1 with an error 
		 * @c ABUNDANCES_SUM_ERROR allowed.
		 */
		void normalize();

		/**
		 * Returns true if the distribution has no peaks, false - otherwise.
		 * 
		 * @return True if the distribution has no peaks, false - otherwise.
		 */
		bool empty() const { return peaks.empty(); }
				
	private:
		/**
		 * Container for isotopes.
		 */
		peaks_container peaks;
		
		/**
		 * Nominal mass of distribution.
		 */
		nominal_mass_type nominalMass;

		/**
		 * Sets peaks/isotopes container minimum size.
		 */
		void setMinimumSize();
};

/**
 * Prints isotope distribution to the stream @c os.
 *
 * @param os Output stream to which distribution is printed out.
 * @param distribution Distribution to be printed out.
 */
std::ostream& operator <<(std::ostream& os, 
							const IsotopeDistribution& distribution);

} // namespace ims

#endif // IMS_ISOTOPE_DISTRIBUTION_H
