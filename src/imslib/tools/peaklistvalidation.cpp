/**
 * peaklistvalidation.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */

#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <numeric>
#include <fstream>
#include <istream>
#include <sstream>
#include <iomanip>

#include <ims/utils/math.h>
#include <ims/base/exception/ioexception.h>
#include <ims/alphabet.h>
#include <ims/base/parser/moleculesequenceparser.h>
#include <ims/base/parser/moleculeionchargemodificationparser.h>

#include <ims/isotopedistribution.h>
#include <ims/element.h>
#include <ims/utils/compose_f_gx_t.h>

using namespace std;
using namespace ims;

typedef Alphabet alphabet_t;

void initializeCHNOPS(alphabet_t&);

int main(int argc,char **argv) {
	
	typedef MoleculeSequenceParser::sequence_type sequence_type;
	typedef vector<sequence_type> sequences_type;
	typedef sequence_type::size_type size_type;
	typedef MoleculeSequenceParser::container parser_container;
	typedef IsotopeDistribution::nominal_mass_type nominal_mass_type;
	typedef IsotopeDistribution::mass_type mass_type;
	typedef IsotopeDistribution::abundance_type abundance_type;
	typedef IsotopeDistribution::masses_container masses_container;
	typedef IsotopeDistribution::abundances_container abundances_container;
	typedef alphabet_t::element_type element_type;

	typedef vector<pair<mass_type, abundance_type> > peaklist_container_type;
	typedef MoleculeIonChargeModificationParser ion_parser_type;
	typedef ion_parser_type::ion_name_type ion_name_type;
	typedef ion_parser_type::ions_type ions_type;
	typedef map<ion_name_type, peaklist_container_type> ion_peaklist_map_type;
	
	try {		
		// initializes alphabet chnops
		alphabet_t chnops;
		initializeCHNOPS(chnops);
	
		sequences_type not_chnops;
		not_chnops.push_back("Cl");		
		not_chnops.push_back("Br");
		not_chnops.push_back("Si");	
		not_chnops.push_back("F");				
		not_chnops.push_back("Na");		
		not_chnops.push_back("K");
		
		MoleculeSequenceParser parser;
		ion_parser_type ion_parser;
		
		// initializes the counter that counts a total amount of peaklists (incl. various ion modifications)
		size_type n_spectra = 0;
		size_type n_peaklists = 0;
		size_type n_singly_charged = 0;
		size_type n_singly_charged_not_within_error = 0;
		size_type n_multiply_charged = 0;				
		size_type n_multiply_charged_not_within_error = 0;		

		int start_i = 1;
		
		double precision_ppm = 2.0;
		istringstream ppm_string(argv[start_i]);
		if ((ppm_string >> precision_ppm)) {
			++start_i;
		}		

		//extracts the index
		size_type peak_index = 0;
		istringstream peak_index_string(argv[start_i]);
		if ((peak_index_string >> peak_index)) {
			++start_i;
		}		

		// header for peaklists
		cout << std::setprecision(10);

		// header for direct mass differences
//		if (peak_index == 0) {
//			cout << std::setprecision(8) << "#  m_0      " 
////				 << "M_" << peak_index << "-m_" << peak_index << "   "
//				 << "(M_" << peak_index << "-m_" << peak_index << ")/m_" << peak_index << "   ";
//		}
//		// header for indirect mass differences
//		else {
//			cout 
////				 << "M_" << peak_index << "-M_0-m_" << peak_index << "+m_0   " 
//				 << "(M_" << peak_index << "-M_0-m_" << peak_index << "+m_0)/m_" << peak_index << "   ";			
//		}
//		cout << '\n';

		// header for intensities relation
//		cout << "g_" << peak_index << "   f_" << peak_index 
////			 << "   f_"  << peak_index << "/g_" << peak_index 
//			 << "   log(f_" << peak_index << "/g_" << peak_index << ')';
//		cout << '\n';
//		cout << std::setprecision(8) << "#  m_0    g_" << peak_index << "   f_" << peak_index << "   f_" 
//			 << peak_index << "/g_" << peak_index << "   log(f_" << peak_index << "/g_" << peak_index << ')';
//		cout << '\n';

		vector<int> number_of_peaklists;
		number_of_peaklists.reserve(20);
		number_of_peaklists.resize(20);
		for (int i = start_i; i < argc; ++i) {			
			//////////////////////////////////////////////////////////////////////////////
			/////////////////////// Gets peaklist out of file ////////////////////////////
			//////////////////////////////////////////////////////////////////////////////
			string peaklist_filename = argv[i];	
			ifstream ifs(peaklist_filename.c_str());
			if (!ifs) {
				throw IOException("unable to open peaklist file: " + peaklist_filename + "!");
			}
			// variables to handle parsing of molecule sequence in peaklist file
			sequence_type molecule_sequence;
			bool is_molecule_sequence_found = false;
			const sequence_type molecule_header1("molecule:"), molecule_header2("SumFormula:");
			
			// variables to handle parsing of the whole peaklist file
			const string delimits(" \t"), comments("#");
			sequence_type line;
			mass_type mass;
			abundance_type intensity;
//			abundance_type area;	// area - area under curve
			ion_name_type ion_modification;
			// container to store mass, intenisty and ion_modification			
			ion_peaklist_map_type ion_peaklist_map;
//			ion_peaklist_map_type ion_peaklist_map_with_area;
			while (getline(ifs, line)) {
				size_type start_pos = line.find_first_not_of(delimits);
				if (start_pos == string::npos) {
					continue; // skips white lines
				}
				if (comments.find(line[start_pos]) != string::npos) {
					// checks if the molecule is not yet found, 
					// if not tries to extract it out commenting line with a certainer header
					if (!is_molecule_sequence_found) {
						bool is_header1 = true;
						size_type molecule_index = line.find(molecule_header1);
						if (molecule_index == string::npos) {
							is_header1 = false;
							molecule_index = line.find(molecule_header2);
						}
						if (molecule_index != string::npos) {
							size_type molecule_start_pos = molecule_index + 
								((is_header1) ? molecule_header1.size(): molecule_header2.size());
							molecule_start_pos = line.find_first_not_of(delimits, molecule_start_pos+1);
							
							// parses the molecule name
							// needs to use the simple letter-by-letter technique		
							size_type molecule_range = 1;

							for (; molecule_start_pos + molecule_range < line.size() &&
								   (isalpha(line[molecule_start_pos + molecule_range]) ||
							        isdigit(line[molecule_start_pos + molecule_range])); ++molecule_range) {
							}
							molecule_sequence = line.substr(molecule_start_pos, molecule_range);
							is_molecule_sequence_found = true;
						}
					}
					continue; // skips comments
				}
				///////////////////////////////////////////////////////////////////
				// splits line
				sequences_type line_elements;
				
				size_type end_pos = line.find_first_of(delimits, start_pos);
				while (start_pos != string::npos) {
					sequence_type line_element = line.substr(start_pos, end_pos - start_pos);
					line_elements.push_back(line_element);
					start_pos = line.find_first_not_of(delimits, end_pos);
					end_pos = line.find_first_of(delimits, start_pos);
				}
				///////////////////////////////////////////////////////////////////////////////////
				///////////////////////////// parses intensity ////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////
				if (line_elements.size() < 3) {
					// TODO: here should be another exception, i.e. PeaklistParserException
					throw IOException("peaklist file " + peaklist_filename + 
					" has wrong format: data table must contain at least three columns: mass, intensity and ion modification!");
				}
				istringstream mass_string(line_elements[0]), intensity_string(line_elements[1]);
				if (!(mass_string >> mass) || !(intensity_string >> intensity)) {
					// TODO: here should be another exception, i.e. PeaklistParserException
					throw IOException("peaklist file " + peaklist_filename + 
					" has wrong format: 1st and 3d columns in data table must be numbers!");					
				}
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////

				///////////////////////////////////////////////////////////////////////////////////
				///////////////////////////// parses area under curve /////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////
//				if (line_elements.size() < 4) {
//					// TODO: here should be another exception, i.e. PeaklistParserException
//					throw IOException("peaklist file " + peaklist_filename + 
//					" has wrong format: data table must contain at least 4 columns: mass, intensity, area under curve and ion modification!");
//				}
//				istringstream mass_string(line_elements[0]), intensity_string(line_elements[1]), area_string(line_elements[2]);
//				if (!(mass_string >> mass) || !(intensity_string >> intensity) || !(area_string >> area)) {
//					// TODO: here should be another exception, i.e. PeaklistParserException
//					throw IOException("peaklist file " + peaklist_filename + 
//					" has wrong format: 1st, 2nd and 3d columns in data table must be numbers!");					
//				}
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////

				string ion_modification = line_elements[line_elements.size()-1];
				// ends spliting lines
				//////////////////////////////////////////////////////////////////				
				ion_peaklist_map[ion_modification].push_back(make_pair(mass, intensity));
//				ion_peaklist_map_with_area[ion_modification].push_back(make_pair(mass, area));
			}
			
			//////////////////////////////////////////////////////////////////////////////
			////////////////// Gets isotope distribution of the molecule /////////////////
			if (!is_molecule_sequence_found) {
				throw IOException("peaklist file: " + peaklist_filename + " doesn't contain a molecule sequence!");
			}

			////////////////////
			// filters out molecules which contain elements not from CHNOPS set
			bool isFromCHNOPS = true;
			for (sequences_type::const_iterator it = not_chnops.begin(); it != not_chnops.end(); ++it) {
				if (molecule_sequence.find(*it) != sequence_type::npos) {
					isFromCHNOPS = false;
					break;
				}
			}
			if (!isFromCHNOPS) {
				continue;
			}

			////////////////////
			// filters out molecules which contain elements only from CHNOPS set			
////			bool isNotCHNOPS = false;
////			for (sequences_type::const_iterator it = not_chnops.begin(); it != not_chnops.end(); ++it) {
////				if (molecule_sequence.find(*it) != sequence_type::npos) {
////					isNotCHNOPS = true;
////					break;
////				}
////			}
////			if (!isNotCHNOPS) {
////				continue;
////			}
			
			///////////////////
			
			parser.parse(molecule_sequence);
			parser_container molecule_elements(parser.getElements());

			// initializes theoretical isotope distribution
			IsotopeDistribution molecule_isotope_distribution;
			for (parser_container::const_iterator it = molecule_elements.begin(); it != molecule_elements.end(); ++it) {
				IsotopeDistribution element_isotope_distribution = chnops.getElement(it->first).getIsotopeDistribution();
				element_isotope_distribution *= it->second;
				molecule_isotope_distribution *= element_isotope_distribution;
//				cout << "element " << chnops.getElement(it->first).getName() << "*" 
//					 << it->second << '\n' /*<< molecule_isotope_distribution*/;			
			}
			//////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////
			// normalizes measured abundances (intensities)
			for (ion_peaklist_map_type::iterator it = ion_peaklist_map.begin(); it != ion_peaklist_map.end(); ++it) {
				
				abundance_type sum = 0.0;
				
				for (peaklist_container_type::const_iterator peaklist_it = it->second.begin(); 
					 peaklist_it != it->second.end(); ++peaklist_it) {
					sum += peaklist_it->second;
				}
				// checks if the sum of abundances is 1
				if (fabs(sum-1) > IsotopeDistribution::ABUNDANCES_SUM_ERROR) {
					// finds the scale
					abundance_type scale = 1/sum;
					// substitutes the old values with new normalized ones
					for (peaklist_container_type::size_type i = 0; i < it->second.size(); ++i) {
						it->second[i].second *= scale;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////
			// normalizes measured abundances (intensities) for container with area under curve
//			for (ion_peaklist_map_type::iterator it = ion_peaklist_map_with_area.begin(); it != ion_peaklist_map_with_area.end(); ++it) {
//				
//				abundance_type sum = 0.0;
//				
//				for (peaklist_container_type::const_iterator peaklist_it = it->second.begin(); 
//					 peaklist_it != it->second.end(); ++peaklist_it) {
//					sum += peaklist_it->second;
//				//				// checks if the sum of abundances is 1
//				if (fabs(sum-1) > IsotopeDistribution::ABUNDANCES_SUM_ERROR) {
//					// finds the scale
//					abundance_type scale = 1/sum;
//					// substitutes the old values with new normalized ones
//					for (peaklist_container_type::size_type i = 0; i < it->second.size(); ++i) {
//						it->second[i].second *= scale;
//					}
//				}
//			}
			//////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////
			///////////// Loops to generate output for every peaklist (spectrum) ///////// 
			// starts output by printing out the header	
//			cout << std::setprecision(8) << 
////			"#\n# "<< peaklist_filename << '\n' <<
//			"#\n# " << molecule_sequence << '\n';
			++n_spectra;

			for (ion_peaklist_map_type::const_iterator it = ion_peaklist_map.begin(); it != ion_peaklist_map.end(); ++it) {
				
				// stores measured peaklist in a temporary variable
				peaklist_container_type peaklist = it->second;
//				peaklist_container_type peaklist_with_area = ion_peaklist_map_with_area[it->first];
								
				ion_parser.parse(it->first);

				// filters only simple (singly charged) peaklists
				if (ion_parser.getMassFactor() != 1 || ion_parser.getCharge() != 1) {
					continue;
				}

				// calculates the mass modification, contributed by ions
				// and calculates an isotope distribution of ions
				mass_type modification_mass = 0.0;
				ions_type ions = ion_parser.getIons();

				IsotopeDistribution ions_distribution;				
				sequence_type ions_string;
				for (ions_type::const_iterator ions_it = ions.begin(); ions_it != ions.end(); ++ions_it) {
					mass_type ion_mass = 0.0;
					// TODO: when the notion of ComposedElement will be integrated, this has to be changed!					
					if (ions_it->is_ion_cation) {
						ion_mass -= element_type::ELECTRON_MASS_IN_U; // this is bad! 
					} else {
						ion_mass += element_type::ELECTRON_MASS_IN_U;
					}

					// calculates mass of ions sequence					
					for (ion_parser_type::ion_elements_type::const_iterator 
							elements_it = ions_it->elements.begin(); 
							elements_it != ions_it->elements.end(); 
							++elements_it) { 
						
						ion_mass += chnops.getElement(
							elements_it->first).getMass() * 
							elements_it->second;

					}
					
					if (ions_it->is_ion_added) {
						// calculates ions sequence that contributed to
						// the isotope distribution of measured mass spectra
						// only those ions have contributed that were added
						// to the molecule
						ostringstream ions_stream;						
						IsotopeDistribution ion_distribution;
						for (ion_parser_type::ion_elements_type::const_iterator 
							elements_it = ions_it->elements.begin(); 
							elements_it != ions_it->elements.end(); 
							++elements_it) { 
						
							ions_stream << elements_it->first;
							
							IsotopeDistribution ion_element_distribution = chnops.getElement(elements_it->first).
																					getIsotopeDistribution();							
							
							if (elements_it->second != 1) {
								ions_stream << elements_it->second;
								ion_element_distribution *= elements_it->second;								
							}
							ion_distribution *= ion_element_distribution;
						}
						ions_string += ions_stream.str();
						ions_distribution *= ion_distribution;
						modification_mass += ion_mass;
					} else {
						modification_mass -= ion_mass;
					}
					
//					std::cout << "modification mass " << modification_mass << '\n';
				}
				
				// modifies the masses with respect to ions modification and molecule charge
				for (peaklist_container_type::size_type masses_i = 0; masses_i < peaklist.size(); ++masses_i) {
//					cout << masses_i << "th peaklist mass: " << peaklist[masses_i].first << '\n';
					peaklist[masses_i].first -= modification_mass / static_cast<mass_type>( ion_parser.getCharge() );					
					peaklist[masses_i].first *= static_cast<mass_type>( ion_parser.getCharge() );
//					cout << masses_i << "th peaklist mass: " << peaklist[masses_i].first << '\n';
				}
//				std::transform(modified_masses.begin(), modified_masses.end(), 		// source range
//					modified_masses.begin(), 										// destination					
//					compose_f_gx( bind2nd(plus<mass_type>(), modification_mass / static_cast<mass_type>( ion_parser.getCharge() )),
//								  bind2nd(divides<mass_type>(), static_cast<mass_type>( ion_parser.getCharge() ))));

				// modifies masses with respect to the mass factor				
				if (ion_parser.getMassFactor() > 1) {
					for (peaklist_container_type::size_type masses_i = 0; masses_i < peaklist.size(); ++masses_i) {
//						cout << masses_i << "th peaklist mass: " << peaklist[masses_i].first << '\n';
						peaklist[masses_i].first += masses_i * (ion_parser.getMassFactor() - 1);
						peaklist[masses_i].first /= ion_parser.getMassFactor();
//						cout << masses_i << "th peaklist modified mass: " << peaklist[masses_i].first << '\n';
					}
				}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////// Implements simulation of sub-ppm machine
//				for (peaklist_container_type::size_type masses_i = 0; 
//									masses_i < peaklist.size(); ++masses_i) {
//					peaklist[masses_i].first = 
//							molecule_isotope_distribution[masses_i] + 
//							( peaklist[masses_i].first - 
//							  molecule_isotope_distribution[masses_i] )
//							* precision_ppm / 2;
//				}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
				// finds the minimum size between two vectors to be compared
				peaklist_container_type::size_type size = min(peaklist.size(), molecule_isotope_distribution.size());

				//////////////////////////////////////////////////////////////////////////////
				/////////////////Getting differences & writing output ////////////////////////
				//////////////////////////////////////////////////////////////////////////////
				
				// filters out peaklists those size is less than of a given peak index				
				if (peak_index >= size) {
					continue;
				}

				++n_peaklists;
//				++number_of_peaklists[peaklist.size()];					

				if (fabs(peaklist[0].first - molecule_isotope_distribution.getMass(0)) / molecule_isotope_distribution.getMass(0) * 1000000.0
								< precision_ppm) {
					if (ion_parser.getMassFactor() == 1 && ion_parser.getCharge() == 1) {
						++n_singly_charged;
					} else {
						++n_multiply_charged;
					}
				} 
				else {
					if (ion_parser.getMassFactor() == 1 && ion_parser.getCharge() == 1) {
						++n_singly_charged_not_within_error;
					} else {
						++n_multiply_charged_not_within_error;
					}
//					cout << "mass error " << fabs(peaklist[0].first - 
//							molecule_isotope_distribution.getMass(0)) / 
//							molecule_isotope_distribution.getMass(0) * 1000000.0 
//						 << " > 2 ppm! (theoretical mass = " << molecule_isotope_distribution.getMass(0) <<")\n";
					continue;
				}
				
				///////////////////////////////////////////////////////////////////////////////
				//////////////// Output for score decompositions tool /////////////////////////
//				cout << molecule_sequence << ' ' << ions_string << '\n';
//
//				for (peaklist_container_type::size_type i = 0; i < size; ++i) {
//					cout << peaklist[i].first << ' ' << peaklist[i].second << '\n';
//				}
//				cout << "### " << peaklist_filename << " ###\n";
				///////////////////////////////////////////////////////////////////////////////
					
				cout << molecule_isotope_distribution.getMass(peak_index) << ' ';
				// outputs direct mass difference
			 	cout
//					<< peaklist[peak_index].first << "  "
					// absolute
					<< (peaklist[peak_index].first - molecule_isotope_distribution.getMass(peak_index)) << ' '
					// relative
			 		<< (peaklist[peak_index].first - molecule_isotope_distribution.getMass(peak_index)) / molecule_isotope_distribution.getMass(peak_index) << ' ';
////////	 		<< (peaklist_with_area[peak_index].first - molecule_isotope_distribution.getMass(peak_index)) / molecule_isotope_distribution.getMass(peak_index) << "  ";
				if (peak_index != 0) {				
					// outputs indirect mass difference					
					mass_type indirect_diff = peaklist[peak_index].first - peaklist[0].first - 
												molecule_isotope_distribution.getMass(peak_index) + molecule_isotope_distribution.getMass(0);
//					mass_type indirect_diff_with_area = peaklist_with_area[peak_index].first - peaklist_with_area[0].first - 
//												molecule_isotope_distribution.getMass(peak_index) + molecule_isotope_distribution.getMass(0);
					cout 
						<< indirect_diff << ' '
						<< indirect_diff / molecule_isotope_distribution.getMass(peak_index) << ' ';
//						<< indirect_diff_with_area / molecule_isotope_distribution.getMass(peak_index) << ' ';
				}
				cout << '\n';

				// outputs log10(abundance difference)
				
////				// before calculating difference between theoretical and measured abundances, we have to take into account
////				// the isotope distribution of ions sequence.
////				// first copies the theoretical isotope distribution of the molecule:
////				IsotopeDistribution molecule_with_ions_isotope_distribution(molecule_isotope_distribution);
////				// second folds it with ions distribution
////				molecule_with_ions_isotope_distribution *= ions_distribution;
////				// if size of isotope distribution is bigger than of peaklist
////				// normalizes abundances from the simulated isotope distribution
////				abundances_container theoretical_abundances = molecule_with_ions_isotope_distribution.getAbundances();
////				if (size < molecule_with_ions_isotope_distribution.size()) {
////					// normalizes the isotope distribution abundances with respect to the number of elements in peaklist				
////					abundance_type sum = std::accumulate(theoretical_abundances.begin(), theoretical_abundances.begin() + size, 0.0);
////					if (fabs(sum - 1) > IsotopeDistribution::ABUNDANCES_SUM_ERROR) {
////						abundance_type scale = 1/sum;
////						std::transform(theoretical_abundances.begin(), theoretical_abundances.begin() + size,	// source range
////								theoretical_abundances.begin(), 											// destination
////								std::bind2nd(std::multiplies<abundance_type>(), scale));				// operation (*scale)
////					}
////					
////				}
				
////				cout 
//////					 << theoretical_abundances[peak_index] << "  " << peaklist[peak_index].second << "  "
////					 << peaklist[peak_index].second / theoretical_abundances[peak_index] << ' '
////					 << log10(peaklist[peak_index].second / theoretical_abundances[peak_index]) << "  ";
////////					 << peaklist_with_area[peak_index].second << "  "
////////					 << log10(peaklist_with_area[peak_index].second / theoretical_abundances[peak_index]);
////				cout << '\n';
			} // .. for ion_peaklist_map	
		
		} // .. peaklist_filenames && molecules_sequences loop
		
		cout << "#spectra = " << n_spectra << '\n' 
			 << "#peaklists = " << n_peaklists << '\n' 
			 << "# - singly charged < " << precision_ppm << "ppm = " << n_singly_charged << '\n'
			 << "# - singly charged > " << precision_ppm << "ppm = " << n_singly_charged_not_within_error
			 << "\n# - multiply charged < " << precision_ppm << "ppm = " << n_multiply_charged << '\n'
			 << "# - multiply charged > " << precision_ppm << "ppm = " << n_multiply_charged_not_within_error

			 << '\n'; 

////////////////////////////////////////////////////////////////////////////////////
///////////		Output for number of peaklists with different amounts of peaks /////
////////////////////////////////////////////////////////////////////////////////////
//		for (vector<int>::size_type i = 0; i < number_of_peaklists.size(); ++i) {
//			cout << "+" << i << " = " << number_of_peaklists[i] << '\n';
//		}
	} catch (Exception& e) {
			cout << "Exception: " << e.message() << endl;
	}

	return 0;
}

void initializeCHNOPS(alphabet_t& chnops) {
	typedef IsotopeDistribution distribution_type;
	typedef IsotopeDistribution::peaks_container peaks_container;
	typedef IsotopeDistribution::nominal_mass_type nominal_mass_type;	
	typedef Alphabet::element_type element_type;
	typedef Alphabet::container elements_type;

	distribution_type::SIZE = 10;
	distribution_type::ABUNDANCES_SUM_ERROR = 0.0001;

// Hydrogen
	nominal_mass_type massH = 1;	
	peaks_container peaksH;
	peaksH.push_back(peaks_container::value_type(0.007825, 0.99985));
	peaksH.push_back(peaks_container::value_type(0.014102, 0.00015));

	distribution_type distributionH(peaksH, massH);

// Oxygen		
	nominal_mass_type massO = 16;
	peaks_container peaksO;
	peaksO.push_back(peaks_container::value_type(-0.005085, 0.99762));
	peaksO.push_back(peaks_container::value_type(-0.000868, 0.00038));
	peaksO.push_back(peaks_container::value_type(-0.000839, 0.002));
	
	distribution_type distributionO(peaksO, massO);

// Carbonate	
	nominal_mass_type massC = 12;
	peaks_container peaksC;
	peaksC.push_back(peaks_container::value_type(0.0, 0.9889));
	peaksC.push_back(peaks_container::value_type(0.003355, 0.0111));

	distribution_type distributionC(peaksC, massC);

// Nitrogen
	nominal_mass_type massN = 14;
	peaks_container peaksN;
	peaksN.push_back(peaks_container::value_type(0.003074, 0.99634));
	peaksN.push_back(peaks_container::value_type(0.000109, 0.00366));

	distribution_type distributionN(peaksN, massN);

// Sulfur
	nominal_mass_type massS = 32;
	peaks_container peaksS;
	peaksS.push_back(peaks_container::value_type(-0.027929, 0.9502));
	peaksS.push_back(peaks_container::value_type(-0.028541, 0.0075));
	peaksS.push_back(peaks_container::value_type(-0.032133, 0.0421));
	peaksS.push_back(peaks_container::value_type());
	peaksS.push_back(peaks_container::value_type(-0.032919, 0.0002));

	distribution_type distributionS(peaksS, massS);

// Phosphor
	nominal_mass_type massP = 31;
	peaks_container peaksP;
	peaksP.push_back(peaks_container::value_type(-0.026238, 1.0));

	distribution_type distributionP(peaksP, massP);

// TODO: Clorine, Silicon ... Wolfram  - intensities are not precise!

// Natrium
	nominal_mass_type massNa = 23;
	peaks_container peaksNa;
	peaksNa.push_back(peaks_container::value_type(-0.010231, 1.0));
	distribution_type distributionNa(peaksNa, massNa);

// Clorine
	nominal_mass_type massCl = 35;
	peaks_container peaksCl;
	peaksCl.push_back(peaks_container::value_type(-0.031147, 0.7577));
	peaksCl.push_back(peaks_container::value_type());
	peaksCl.push_back(peaks_container::value_type(-0.034097, 0.2423));		

	distribution_type distributionCl(peaksCl, massCl);

// Bromine
	nominal_mass_type massBr = 79;
	peaks_container peaksBr;
	peaksBr.push_back(peaks_container::value_type(-0.081663, 0.5069));
	peaksBr.push_back(peaks_container::value_type());
	peaksBr.push_back(peaks_container::value_type(-0.083709, 0.4931));		
	
	distribution_type distributionBr(peaksBr, massBr);

// Kalium
	nominal_mass_type massK = 39;
	peaks_container peaksK;
	peaksK.push_back(peaks_container::value_type(-0.036293, 0.9326));
	peaksK.push_back(peaks_container::value_type());
	peaksK.push_back(peaks_container::value_type(-0.038174, 0.0674));		
	
	distribution_type distributionK(peaksK, massK);

	nominal_mass_type massSi = 28;
	peaks_container peaksSi;
	peaksSi.push_back(peaks_container::value_type(-0.023073, 0.9223));
	peaksSi.push_back(peaks_container::value_type(-0.023505, 0.0467));
	peaksSi.push_back(peaks_container::value_type(-0.026230, 0.0310));
	
	distribution_type distributionSi(peaksSi, massSi);
	
// Fluorine
	nominal_mass_type massF =19;
	peaks_container peaksF;
	peaksF.push_back(peaks_container::value_type(-0.001597, 1.0));
	IsotopeDistribution distributionF(peaksF, massF);


//// Molybdenum
//	abundances_container containerMo;
//	containerMo.push_back(0.1484);
//	containerMo.push_back(0.9250);
//	containerMo.push_back(0.1592);
//	containerMo.push_back(0.1668);
//	containerMo.push_back(0.0955);
//	containerMo.push_back(0.2413);
//
//	masses_container massesContainerMo;
//	massesContainerMo.push_back(-0.093189);
//	massesContainerMo.push_back(-0.093187);
//	massesContainerMo.push_back(-0.094912);
//	massesContainerMo.push_back(-0.094158);
//	massesContainerMo.push_back(-0.095320);
//	massesContainerMo.push_back(-0.093978);	
//	
//	nominal_mass_type massMo = 92;
//	IsotopeDistribution distributionMo(containerMo, massesContainerMo, massMo);
//	distributionMo.normalize();
//
//// Wolfram
//	abundances_container containerW;
//	containerW.push_back(0.263);
//	containerW.push_back(0.143);
//	containerW.push_back(0.3067);
//	containerW.push_back(0.286);
//
//	masses_container massesContainerW;
//	massesContainerW.push_back(-0.051796);
//	massesContainerW.push_back(-0.049777);
//	massesContainerW.push_back(-0.049069);
//	massesContainerW.push_back(-0.046581);
//
//	nominal_mass_type massW = 182;
//	IsotopeDistribution distributionW(containerW, massesContainerW, massW);
//	distributionW.normalize();
//
//// Gadolinium
//	abundances_container containerGd;
//	containerGd.push_back(0.0218);
//	containerGd.push_back(0.1480);
//	containerGd.push_back(0.2047);
//	containerGd.push_back(0.1565);
//	containerGd.push_back(0.2484);
//
//	masses_container massesContainerGd;
//	massesContainerGd.push_back(-0.079134);
//	massesContainerGd.push_back(-0.077378);
//	massesContainerGd.push_back(-0.077877);
//	massesContainerGd.push_back(-0.07604);
//	massesContainerGd.push_back(-0.075896);
//	
//	nominal_mass_type massGd = 154;
//	IsotopeDistribution distributionGd(containerGd, massesContainerGd, massGd);
//	distributionGd.normalize();


	// initialization of chemical elements
	element_type C("C", distributionC);
	element_type H("H", distributionH);
	element_type N("N", distributionN);
	element_type O("O", distributionO);
	element_type P("P", distributionP);
	element_type S("S", distributionS);
		
	element_type Na("Na", distributionNa);
	element_type Cl("Cl", distributionCl);
	element_type Br("Br", distributionBr);
	element_type K("K", distributionK);	
	element_type Si("Si", distributionSi);
				
	element_type F("F", distributionF);

//	element_type Mo("Mo", distributionMo);
//	element_type Gd("Gd", distributionGd);
//	element_type W("W", distributionW);
	
	chnops.push_back(C);
	chnops.push_back(H);
	chnops.push_back(N);
	chnops.push_back(O);
	chnops.push_back(P);
	chnops.push_back(S);

	chnops.push_back(Na);
	chnops.push_back(Cl);
	chnops.push_back(Br);
	chnops.push_back(K);
	chnops.push_back(Si);
				
	chnops.push_back(F);		
//	chnops.push_back(Mo);
//	chnops.push_back(Gd);
//	chnops.push_back(W);			
}
