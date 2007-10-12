#ifndef IMS_ISOTOPE_SPECIES_H
#define IMS_ISOTOPE_SPECIES_H

#include <vector>
#include <ostream>

namespace ims {

// full isotope distribution with multiple entries (pairs of mass and abundance) per isotope
class IsotopeSpecies {
	public:
		typedef double mass_type;
		typedef double abundance_type;

		struct Peak {
			Peak(mass_type mass = 0.0, abundance_type abundance = 0.0) : 
				mass(mass), abundance(abundance) { }
			bool operator ==(const Peak& peak) const {
				return (peak.mass == mass && peak.abundance == abundance);
			}
			mass_type mass;
			abundance_type abundance;
		};

		typedef Peak peak_type;
		typedef std::vector<peak_type> peaks_container;
		typedef peaks_container::size_type size_type;
		typedef std::vector<peak_type>::iterator peaks_iterator;
		typedef std::vector<peak_type>::const_iterator const_peaks_iterator;

		IsotopeSpecies(){}
		IsotopeSpecies(const peaks_container& isotopes) : 
				isotopes(isotopes) {}

		IsotopeSpecies(const IsotopeSpecies& distribution) : 
				isotopes(distribution.isotopes) {}

		size_type size() const { return isotopes.size(); }

		IsotopeSpecies& operator=(const IsotopeSpecies& distribution);
		
		IsotopeSpecies& operator*=(const IsotopeSpecies& distribution);
		
		IsotopeSpecies& operator*=(unsigned int pow);
		
		mass_type getMass(size_type i) const { return isotopes[i].mass; }		
		
		abundance_type getAbundance(size_type i) const { return isotopes[i].abundance; }

		void sortByMass();
		
		void filterAbundances(abundance_type threshold);

	private:
		typedef std::vector<abundance_type> abundances_container;
		typedef std::vector<int> indexes_container;
		typedef indexes_container::const_iterator indexes_iterator;

		void updateCoefficients(int i, indexes_container& indexes, size_type k, 
			int max_coefficient, int coefficient, peaks_container& temp_container);
		void updateIndexes(size_type until, indexes_container& indexes);

		peaks_container isotopes;
		
		class MassSortingCriteria {
			public:
				bool operator()(const peak_type& p1, const peak_type& p2) const {
					return p1.mass < p2.mass;
				}
		};

		class AbundanceRemoveCriteria {
			private:
				abundance_type threshold;
			public:
				AbundanceRemoveCriteria(abundance_type threshold) : threshold(threshold) {}
				bool operator()(const peak_type& p1) const {
					return p1.abundance < threshold;
				}
		};

};

std::ostream& operator<<(std::ostream& os, const IsotopeSpecies& distribution);

} // namespace ims

#endif // IMS_ISOTOPE_SPECIES_H
