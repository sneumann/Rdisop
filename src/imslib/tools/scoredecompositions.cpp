/**
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */

#include <functional>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <ims/utils/math.h>

#include <ims/alphabet.h>
#include <ims/element.h>
#include <ims/composedelement.h>
#include <ims/nitrogenrulefilter.h>
#include <ims/distributionprobabilityscorer.h>
#include <ims/weights.h>
#include <ims/decomp/integermassdecomposer.h>
#include <ims/decomp/decomputils.h>
#include <ims/utils/math.h>
#include <ims/utils/stopwatch.h>

#include <ims/base/parser/moleculesequenceparser.h>
#include <ims/base/exception/ioexception.h>

using namespace ims;
using namespace std;

typedef Alphabet alphabet_t;

///////////////////////////////////////////////////////////////////////////////////////
///// structs and functions to evaluate different scoring schemes /////////////////////
///////////////////////////////////////////////////////////////////////////////////////
struct ScoreSchemeElement {
	vector<unsigned int> score_onoff_bits;
	double n_rank1;
	double n_rank2_10;
	double n_rank11_100;
	unsigned int n_peaks_to_score;
};

bool rank1SortCriterion(const ScoreSchemeElement& e1, const ScoreSchemeElement& e2) {
	return e1.n_rank1 > e2.n_rank1;
}

bool top10SortCriterion(const ScoreSchemeElement& e1, const ScoreSchemeElement& e2) {
	return e1.n_rank1 + e1.n_rank2_10 > 
			e2.n_rank1+e2.n_rank2_10;
}

bool top100SortCriterion(const ScoreSchemeElement& e1, const ScoreSchemeElement& e2) {
	return e1.n_rank1 + e1.n_rank2_10 + e1.n_rank11_100 
			> e2.n_rank1 + e2.n_rank2_10 + e2.n_rank11_100;
}

void outputScoreSchemeStatistics(const ScoreSchemeElement&);

//////////////////////////////////////////////////////////////////////////////////////
///////// struct to collect algorithm evaluation per a sample molecule ///////////////
//////////////////////////////////////////////////////////////////////////////////////
struct DataAnalysisElement {
	double mass;
	string sample_molecule;
	string ions;
	unsigned int N_before_false_positive_check;
	unsigned int overall_number_of_candidates;
	unsigned int number_of_candidates_after_filter;
	multimap<double, string, greater<double> > top_scores;
	// next two fields are for table showing the correllation between rank and masses or abundances delta (error)
	vector<double> masses_delta;
	vector<double> abundances_delta;
	vector< pair<double,double> > measured_distribution;
	vector< pair<double,double> > theoretical_distribution;
	vector<double> scores;
};

//////////////////////////////////////////////////////////////////////////////////////
///////// struct to collect idenitification rates (ranks) for a set of spectra ///////
//////////// (here used for a set of spectra in a certain mass range) ////////////////
//////////////////////////////////////////////////////////////////////////////////////

struct RankTableElement {
	unsigned int N_spectra;
	vector<unsigned int> ranks;
	unsigned int N_integer;
	unsigned int N_real;
	unsigned int N_chemical_filter_ok;
	double runtime;
};

void initializeCHNOPS(alphabet_t&);
void initializeIonsAlphabet(alphabet_t&);

int main(int argc, char** argv) {

//////////////////////////////////////////
//// First part: initialize alphabets ////
//////////////////////////////////////////

	typedef double mass_type;
	typedef IsotopeDistribution distribution_type;
	typedef distribution_type::peaks_container peaks_container;
	typedef distribution_type::mass_type mass_type;
	typedef distribution_type::abundance_type abundance_type;
	typedef distribution_type::masses_container masses_container;
	typedef distribution_type::abundances_container abundances_container;	
	typedef distribution_type::nominal_mass_type nominal_mass_type;
	typedef alphabet_t::element_type element_type;
	typedef element_type::name_type name_type;
	typedef name_type::size_type name_size_type;
	typedef alphabet_t::container elements_type;

	typedef Weights::alphabet_mass_type alphabet_mass_type;
	typedef Weights::alphabet_masses_type alphabet_masses_type;
	typedef Weights::weight_type weight_type;
	typedef Weights::weights_type weights_type;

	typedef IntegerMassDecomposer<> decomposer_type;
	typedef decomposer_type::decompositions_type decompositions_type;
	typedef decompositions_type::value_type decomposition_type;
	typedef pair<alphabet_mass_type, alphabet_mass_type> alphabet_masses_pair_type;
	typedef vector<pair<name_type, pair<name_type, distribution_type> > > molecule_peaklist_pairs_type;

	typedef MoleculeSequenceParser::container parser_container;
 	typedef DistributionProbabilityScorer distribution_scorer_type;
	typedef distribution_scorer_type::score_type score_type;
 	typedef multimap<score_type, name_type, greater<score_type> > scores_type;
  	typedef multimap<unsigned int, unsigned int> molecules_ranking_type;

	typedef vector<DataAnalysisElement> data_analisys_container;
	typedef vector<RankTableElement> rank_table_type;

	if (argc == 1) {
		throw IOException("command line lacks a parameter: filename with masses!");
	}

	/////////////////////////////////////////////////////////////////////////////////
	///////////////////////// parses the input file /////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////

	ifstream ifs(argv[1]);
	if (!ifs) {
		string filename = argv[1];
		throw IOException("unable to open file with masses: " + filename + "!");
	}

	// variables to handle parsing of the whole peaklist file
	const string delimits(" \t"), comments("#");
	name_type line, molecule, ions;
	mass_type mass;
	abundance_type abundance;
	// container to store mass and molecule
	molecule_peaklist_pairs_type molecule_peaklist_pairs;
	peaks_container peaks;
	while (getline(ifs, line)) {
		name_size_type start_pos = line.find_first_not_of(delimits);
		if (start_pos == name_type::npos) {
			continue; // skips white lines
		}
		// finds comments and stores a newly collected peaklist with its corresponding molecule and sequence of ions
		if (comments.find(line[start_pos]) != name_type::npos) {
			if (peaks.size() != 0 && molecule.size() != 0) {
				nominal_mass_type masses_nominal_mass = static_cast<nominal_mass_type>(peaks[0].mass);
//				cout << "nominal mass = " << masses_nominal_mass << '\n';
				for (peaks_container::size_type mi = 0; mi < peaks.size(); ++mi) {
//					cout << "masses[" << mi << "] before = " << masses[mi];
					peaks[mi].mass -= masses_nominal_mass + mi;
//					cout << ", masses[" << mi << "] after = " << masses[mi] << '\n';
				}
				molecule_peaklist_pairs.push_back(make_pair(molecule, make_pair(ions, distribution_type(peaks, masses_nominal_mass))));
				peaks.clear();
			}
			continue; // skips comments
		}
		// extracts molecule
		if (isalpha(line[start_pos])) {
			name_size_type end_pos = line.find_first_of(delimits, start_pos);
			if (end_pos != name_type::npos) {
				molecule = line.substr(start_pos, end_pos - start_pos);
				// extracts sequence of ions
				start_pos = line.find_first_not_of(delimits, end_pos);
				if (start_pos != name_type::npos) {
					end_pos = line.find_first_of(delimits, start_pos);
					if (end_pos != name_type::npos) {
						ions = line.substr(start_pos, end_pos - start_pos);
					} else {
						ions = line.substr(start_pos);
					}
				}
			} else {
				molecule = line.substr(start_pos);
			}
		// extracts and collects peaklist, but not stores it yet
		} else if (isdigit(line[start_pos])) {
			istringstream input(line);
			input >> mass >> abundance;
			peaks.push_back(peaks_container::value_type(mass, abundance));
		}
	}

	// initializes alphabet with elements H(hydrogen), C(carbon), N, O, P, S
	alphabet_t chnops;
	initializeCHNOPS(chnops);
	
	alphabet_t ions_alphabet(chnops);
	initializeIonsAlphabet(ions_alphabet);

	// initializes order of elements in the molecule
	vector<string> elements_order;
	elements_order.push_back("C");
	elements_order.push_back("H");
	elements_order.push_back("N");
	elements_order.push_back("O");
	elements_order.push_back("P");
	elements_order.push_back("S");
	elements_order.push_back("Na");
	elements_order.push_back("K");
	elements_order.push_back("Si");
	elements_order.push_back("Cl");	
	
	// initializes blowup and error
	alphabet_mass_type blowup = 100000;
	alphabet_mass_type mass_precision_ppm = 2.0;
	if (argc > 2) {
		istringstream mass_precision_ppm_string(argv[2]);
		mass_precision_ppm_string >> mass_precision_ppm;
	}

	molecules_ranking_type::key_type scores_cutoff = 10;
	if (argc > 3) {
		istringstream scores_cutoff_string(argv[3]);
		scores_cutoff_string >> scores_cutoff;
	}

	//extracts the index
//	vector<double>::size_type index = 0;
//	if (argc > 4) {
//		istringstream index_string(argv[4]);
//		if ((index_string >> index)) {
//		}
//	}

//	//initializes the number of scores
//	unsigned int n_scores = 7;
//	if (argc > 4) {
//		istringstream n_scores_string(argv[4]);
//		if ((n_scores_string >> n_scores)) {
//		}
//	}
//	int n_combinations_of_scores = static_cast<int>(pow(2.0, static_cast<double>(n_scores)));
//	
//
//	unsigned int n_peaks_to_score = 4;
//	if (argc > 5) {
//		istringstream n_peaks_to_score_string(argv[4]);
//		if ((n_peaks_to_score_string >> n_peaks_to_score)) {
//		}
//	}

	// initializes weights
	Weights chnops_monoisotopic_weights(chnops.getMasses(), 1 / blowup);

	// initializes the decomposer
	decomposer_type decomposer(chnops_monoisotopic_weights);

	// initializes variables for working with false/true positives.
	alphabet_masses_pair_type minMaxWeightsRoundingtErrors = DecompUtils::getMinMaxWeightsRoundingErrors(chnops_monoisotopic_weights);

	//////////// debug ////////////
//	for (molecule_peaklist_pairs_type::const_iterator it = molecule_peaklist_pairs.begin(); it != molecule_peaklist_pairs.end(); ++it) {
//		cout << it->first << ' ' << it->second.first << '\n' << it->second.second;
//	}
	///////////////////////////////
	
	
	
//////////////////////////////////////////////////////////////////////
///////// loop through all possible combinations of scoring schemes //
//////////////////////////////////////////////////////////////////////
////	vector<ScoreSchemeElement> score_schemes_statistics;
////	while (--n_combinations_of_scores >= 0) {
////	
////		// initializes score scheme
////		vector<unsigned int> score_scheme;
////		int rest = n_combinations_of_scores;
////		while (rest > 0) {
////			score_scheme.push_back(rest % 2);
////			rest >>= 1;
////		}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

	// initializes molecule ranking results
	molecules_ranking_type ranking_results;

	// initializes container where analysis will be stored
	data_analisys_container analysis_elements;
	data_analisys_container::size_type analysis_element_index = 0;

	///////////////////////////////////////////////////////////
	/////////////// table "ranks per mass class" //////////////
	///////////////////////////////////////////////////////////

	unsigned int max_mass = 1400, one_mass_class_range = 100;
	unsigned int rank_table_size = max_mass / one_mass_class_range;
	if (rank_table_size == 0) {
		rank_table_size = 1;
	}
	rank_table_type	rank_table(rank_table_size);
	for (rank_table_type::size_type ri = 0; ri < rank_table.size(); ++ri) {
		rank_table[ri].N_spectra = 0;
		rank_table[ri].ranks.reserve(scores_cutoff + 1);
		rank_table[ri].ranks.resize(scores_cutoff + 1);
		rank_table[ri].runtime = 0.0;
		rank_table[ri].N_integer = 0;
		rank_table[ri].N_real = 0;
		rank_table[ri].N_chemical_filter_ok = 0;
	}

	Stopwatch timer;
	cout << setprecision(10);

	// main loop
	for (molecule_peaklist_pairs_type::const_iterator it = molecule_peaklist_pairs.begin(); it != molecule_peaklist_pairs.end(); ++it) {

		// initializes data analysis element
		DataAnalysisElement analysis_element;

		/////////////////////////////////////////////////////////////////////////////////
		///////////////// initializes distribution of sample molecule ///////////////////
		/////////////////////////////////////////////////////////////////////////////////
		ComposedElement sample_molecule(it->first, chnops);
		sample_molecule.updateSequence(&elements_order);		
		string sample_molecule_sequence = sample_molecule.getSequence();

		analysis_element.sample_molecule = sample_molecule_sequence;

		ComposedElement ions(it->second.first, ions_alphabet);
		ions.updateSequence(&elements_order);
		ions.updateIsotopeDistribution();
		IsotopeDistribution ions_distribution = ions.getIsotopeDistribution();
		
		analysis_element.ions = ions.getSequence();
				
		// initializes peaklist
		distribution_type peaklist = it->second.second;
		analysis_element.mass = peaklist.getMass(0);

		/////////////////////////////////////////////
		/////////////// ranking table  //////////////
		/////////////////////////////////////////////
		if (analysis_element.mass > max_mass) {
			continue;
		}
		// defines the mass class in rank table
		rank_table_type::size_type mass_class_index =
			static_cast<rank_table_type::size_type>(analysis_element.mass) / one_mass_class_range;
		if (mass_class_index >= rank_table.size()) {
			mass_class_index = rank_table.size() - 1;
		}
		// increases number of spectra in the corresponding mass class
		++rank_table[mass_class_index].N_spectra;
		timer.start();
		//////////////////////////////////////////////////////////////////////////////////
		///////////// prepares generation of candidates molecules (decompositions) ///////
		//////////////////////////////////////////////////////////////////////////////////

		// prepares an extended range of masses which should be checked
		// (instead of simple +-error) due to the rounding error

		alphabet_mass_type absolute_mass_error = analysis_element.mass * mass_precision_ppm * 0.000001;

		weight_type start_integer_mass = static_cast<weight_type>(ceil(
			(1 + minMaxWeightsRoundingtErrors.first) * (analysis_element.mass - absolute_mass_error) * blowup));
		weight_type	end_integer_mass = static_cast<weight_type>(floor(
			(1 + minMaxWeightsRoundingtErrors.second) * (analysis_element.mass + absolute_mass_error) * blowup));

		//////////////////////////////////////////////////////////////////////////////////
		///////////// generates candidates molecules (decompositions) ////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		decompositions_type all_decompositions_from_range;
//		unsigned int number_of_failed_decompositions = 0;
		for (weight_type integer_mass = start_integer_mass; integer_mass < end_integer_mass; ++integer_mass) {

			decompositions_type decompositions = decomposer.getAllDecompositions(integer_mass);
			for (decompositions_type::iterator pos = decompositions.begin(); pos != decompositions.end(); ++pos) {
				alphabet_mass_type parent_mass = DecompUtils::getParentMass(chnops_monoisotopic_weights, *pos);
				// creates a temporary variable to substitute fabs function (which was the cause of troubles on solaris os)
				if (fabs(parent_mass - analysis_element.mass) <= absolute_mass_error) {
					all_decompositions_from_range.push_back(*pos);
				}
//				else {
//					++number_of_failed_decompositions;
//				}
			}
		} // loop for masses (range of masses allowed by error)

		//////////////////////////////////////////////////////////////////////////////////
		/////////////////////// initializes distribution scorer //////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		distribution_scorer_type scorer(peaklist.getMasses(), peaklist.getAbundances());
		scorer.setMassPrecision(mass_precision_ppm);

		scores_type scores;
		//////////////////////////////////////////////////////////////////////////////////

		for (decompositions_type::const_iterator decomps_it = all_decompositions_from_range.begin();
												 decomps_it != all_decompositions_from_range.end(); ++decomps_it) {

			ComposedElement candidate_molecule(*decomps_it, chnops);
			if (!NitrogenRuleFilter::isMoleculeValid(candidate_molecule)) {
				continue;
			} 
			candidate_molecule.updateIsotopeDistribution();
			candidate_molecule.updateSequence(&elements_order);

			IsotopeDistribution candidate_molecule_distribution = 
					candidate_molecule.getIsotopeDistribution();
			string candidate_molecule_sequence = 
					candidate_molecule.getSequence();
			
			// extracts masses of isotope distribution of candidate molecule
			distribution_scorer_type::masses_container candidate_masses = candidate_molecule_distribution.getMasses();
			//////////////////////////////			
			////////// debug /////////////
//			if (analysis_element.sample_molecule == candidate_molecule_sequence) {
//				cout << "masses & abundances before folding with distribution of ions\n";
//				for (distribution_scorer_type::masses_container::size_type i = 0; i < candidate_molecule_distribution.size(); ++i) {
//					cout << "m[" << i << "] = " << candidate_molecule_distribution.getMass(i)
//						 << ", ab[" << i << "] = " << candidate_molecule_distribution.getAbundance(i) << '\n';
//				}
//			}
			//////////////////////////////
			//////////////////////////////
			
			// modifies isotope distribution of candidate molecule with respect to the sequence of ions
			// since in a predicted peaklist ions are taking part in building an isotope distribution			
			candidate_molecule_distribution *= ions_distribution;
			// if the size of distribution of candidate molecule is bigger than the size of measured peaklist
			// then the size of distribution of candidate molecule is to be changed and abundances are to be normalized
			abundances_container candidate_abundances = candidate_molecule_distribution.getAbundances();
//			distribution_type::size_type size = min(peaklist.size(), candidate_molecule_distribution.size());			
//			if (size < candidate_molecule_distribution.size()) {
//			// normalizes the isotope distribution abundances with respect to the number of elements in peaklist				
//				abundance_type sum = accumulate(candidate_abundances.begin(), 
//												candidate_abundances.begin() + size, 
//												0.0);
//				if (fabs(sum - 1) > IsotopeDistribution::ABUNDANCES_SUM_ERROR) {
//					abundance_type scale = 1/sum;
//					transform(candidate_abundances.begin(),					// begin of source range
//							candidate_abundances.begin() + size,			// end of source range
//							candidate_abundances.begin(), 					// destination
//							bind2nd(multiplies<abundance_type>(), scale));	// operation (*scale)
//				}
//					
//			}
												 
			vector<score_type> erfc_scores;			
			if (analysis_element.sample_molecule == candidate_molecule_sequence) {
				distribution_type::size_type size = min(peaklist.size(), candidate_molecule_distribution.size());
				for (distribution_type::size_type i = 0; i < size; ++i) {
					analysis_element.measured_distribution.push_back(make_pair<double, double>(peaklist.getMass(i), peaklist.getAbundance(i)));
					analysis_element.theoretical_distribution.push_back(make_pair<double, double>(candidate_masses[i], candidate_abundances[i]));
				}
				erfc_scores = scorer.scores(candidate_masses, candidate_abundances);				
				analysis_element.scores = erfc_scores;				
			} else {
				erfc_scores = scorer.scores(candidate_masses, candidate_abundances);
			}
			
			score_type score = (erfc_scores.size() > 0) ? erfc_scores[0]: 1.0;
			for (vector<score_type>::size_type si = 1; si < erfc_scores.size(); ++si) {
				////////////////////////////////////
				// here comes score scheme filter //
//				if (si-1 < score_scheme.size()) {
//					if (score_scheme[si-1] == 1) {
				////////////////////////////////////
				
						score *= erfc_scores[si];
						
				////////////////////////////////////						
				/// continues score scheme filter //
				////////////////////////////////////
//					}
//				}
				/// ends score scheme filter ///////
				////////////////////////////////////
			}						
			scores.insert(make_pair(score, candidate_molecule_sequence));
		}

 		double runtime_per_one_mass = timer.elapsed();
 		rank_table[mass_class_index].runtime += runtime_per_one_mass;

 		molecules_ranking_type::key_type rank = 0, out_of_top_rank = scores_cutoff+1;
 		bool isSampleMoleculeInTop = false;
 		for (scores_type::const_iterator it = scores.begin(); it != scores.end() && ++rank <= scores_cutoff; ++it) {
			if (sample_molecule_sequence == it->second) {
				ranking_results.insert(make_pair(rank, analysis_element_index));
				++rank_table[mass_class_index].ranks[rank-1]; // (rank - 1) - since counting in vector starts from 0 and rank starts from 1
				isSampleMoleculeInTop = true;
				break;
			}
		}
 		if (!isSampleMoleculeInTop) {
			ranking_results.insert(make_pair(out_of_top_rank, analysis_element_index));
			++rank_table[mass_class_index].ranks[out_of_top_rank-1];
 		}

		// copies top scores to analysis element
		scores_type::const_iterator sit = scores.begin();
		molecules_ranking_type::key_type counter = 0;
		for (; sit != scores.end() && ++counter <= scores_cutoff; ++sit) {
	 		analysis_element.top_scores.insert(*sit);
		}

//		analysis_element.N_before_false_positive_check = all_decompositions_from_range.size() + number_of_failed_decompositions;
//
//		analysis_element.overall_number_of_candidates = all_decompositions_from_range.size();
//
 		analysis_element.number_of_candidates_after_filter = scores.size();
 		// adds filled out element to the store
		analysis_elements.push_back(analysis_element);
		++analysis_element_index;

		// fills out the ranking table
//		rank_table[mass_class_index].N_integer += analysis_element.N_before_false_positive_check;
//		rank_table[mass_class_index].N_real += analysis_element.overall_number_of_candidates;
		rank_table[mass_class_index].N_chemical_filter_ok += analysis_element.number_of_candidates_after_filter;
	} // main loop (iteration per one mass)

////////////////// output table: rank <-> masses and abundances delta ////////////
////		cout	 << "# rank  molecule  mass_delta   abundance_delta\n";
//////////////////////////////////////////////////////////////////////////////////

////////////////// output ranking tables per single molecule /////////////////////

////	for (molecules_ranking_type::const_iterator it = ranking_results.begin(); it != ranking_results.end();	++it) {
////		DataAnalysisElement element = analysis_elements[it->second];
////		// output
//////		cout << "# rank           " << it->first;
//////		if (it->first == scores_cutoff + 1) {
//////			cout << '+';
//////		}
//////		cout << "\n# molecule     " << element.sample_molecule
//////			 << "\n# mass         " << element.mass
//////			 << "\n# ions 		  " << element.ions
////////			 << "\n# N_before_false_positives " << element.N_before_false_positive_check
////////			 << "\n# N_candidates             " << element.overall_number_of_candidates
//////			 << "\n# N_after_filter           " << element.number_of_candidates_after_filter //<< '\n';
//////			 << "\n# \n# score  candidate\n";
//////		unsigned int cutoff = 0;
//////		for (scores_type::const_iterator sit = element.top_scores.begin(); sit != element.top_scores.end() && ++cutoff <= 10; ++sit) {
//////			cout << sit->first << ' ' << sit->second << '\n';
//////		}
//////		cout << "#\n";
/////////// output table: rank <-> masses and abundances delta ///////////
////////		if (index < element.masses_delta.size()) {
////////			cout << it->first << ' ' << element.sample_molecule << ' '
////////				 << element.masses_delta[index] << ' ' << element.abundances_delta[index] << '\n';
////////		}
////////// output table: rank <-> theoretical vs measured distributions ///////////
////		ostringstream filename_stream;
////		filename_stream << "distr_theor_vs_measur_" << element.sample_molecule << "_" << element.ions << ".dat";
////		string filename_string = filename_stream.str();
////		ofstream out_file(filename_string.c_str(), ios::out);
////		out_file << setprecision(10) << "#i   m_measur    ab_measur    m_theor    ab_theor    m_score    ab_score  rank  molecule  ions\n";		
////		double overall_score = 1.0;
////		for (distribution_type::size_type ii = 0; ii < element.measured_distribution.size(); ++ii) {
////			out_file << ii << ' ' << element.measured_distribution[ii].first << ' ' 
////				<< element.measured_distribution[ii].second << ' '
////				<< element.theoretical_distribution[ii].first << ' ' 
////				<< element.theoretical_distribution[ii].second << ' ';
////			
////			// output scores in the following format: mass_score[i] abund_score[i]
////			// if there is no score for peak ii, output 1.0
////			if (ii < element.scores.size() / 2) {
////				out_file << element.scores[ii] << ' ' << element.scores[ii + element.scores.size()/2] << ' ';
////				overall_score *= element.scores[ii] * element.scores[ii + element.scores.size()/2];
////			} else {
////				out_file << "   1.0      1.0    ";
////			}
////			out_file << it->first << "   " << element.sample_molecule << "   " << element.ions << '\n';	
////		}
////		out_file << "# overall score = " << overall_score << '\n';
////		out_file.close();
////		// creates gnuplot script
////		ostringstream gnuplotfilename_stream;
////		gnuplotfilename_stream << "plot_distr_theor_vs_measur_" << element.sample_molecule << "_" << element.ions << ".gnu";
////		string gnuplotfilename_string = gnuplotfilename_stream.str();
////		ofstream gnuplot_file(gnuplotfilename_string.c_str(), ios::out);
////		// creates gnuplot title as following: molecule, ions, rank
////		gnuplot_file << "set title \"molecule " << element.sample_molecule << ", ions " << element.ions << ", rank " << it->first << "\"\n";
////		// creates multiplot with measured and theor. distributions on the bigger plot and erfc scores on the smaller plot
////		gnuplot_file << "set terminal postscript enhanced\nset multiplot\nset size 1,1\nset origin 0,0\nset xlabel \"mass\"\nset ylabel \"abundance\"\n";
////		gnuplot_file << "set key left bottom\n";
////		gnuplot_file << "plot '"<< filename_string << "' u 2:3 title 'measured' w linespoints, '' u 4:5 title 'theoretical' w linespoints\n";
////		gnuplot_file << "unset title\nset origin 0.5, 0.5\nset size 0.48, 0.42\nset xlabel \"peak\"\nset ylabel \"erfc score\"\nset xrange [-0.2:4.2]\n";
////		gnuplot_file << "set xtics 0,1,4\nset yrange [0:1.1]\nset ytics 0,0.1,1\nset key right top\n";
////		gnuplot_file << "plot '"<< filename_string << "' u 1:6 title \"mass\" w linespoints, ''  u 1:7 title \"abund\" w linespoints\n";
////		gnuplot_file << "unset multiplot\n";
////		gnuplot_file.close();
////	}

	///////////////////// initializes score schemes statistics ////////////////////
////	ScoreSchemeElement score_scheme_stat_element;
////	score_scheme_stat_element.score_onoff_bits = score_scheme;
////	score_scheme_stat_element.n_peaks_to_score = n_peaks_to_score;

///////////////////////////////////////////////////////////
/////// outputs the scoring scheme ////////////////////////
///////////////////////////////////////////////////////////
////	cout << "# scoring scheme\n";
////	cout << "# m0  +\n";
////	for (vector<unsigned int>::size_type ssi = 0; ssi < n_scores; ++ssi) {
////		if (ssi+1 < n_peaks_to_score) {
////			cout << "# m" << (ssi+1);
////		} else {
////			cout << "# ab" << ssi + 1 - n_peaks_to_score;
////		}
////		if (ssi < score_scheme.size() && score_scheme[ssi] == 1) {
////			cout << " +";
////		} else {
////			cout << " -";
////		}
////		cout << '\n';
////	}
	
///////////////////////////////////////////////////////////
/////// output table "rank per mass class" ////////////////
///////////////////////////////////////////////////////////
	unsigned int n_all_spectra = 0, n_rank1 = 0, n_rank2_10 = 0;
 	cout << "#  masses  N_spectra  rank1  rank2  rank3-5  rank6-10  rank11+  #candidates(av)  runtime_pro_spec(sec)\n";
//	unsigned int n_rank11_100 = 0;
//	cout << "# class  N_spectra  rank1  rank2  rank3-5  rank6-10  rank-50  rank-100 rank100+ #(av) runtime(sec)\n";
//////// 	cout << "#  mass class  N_spectra  rank1  rank2-10  rank11+\n"; 	
//////////////////	cout << "#  mass class  N_spectra  N_integer  N_real  N_chemical_ok  runtime(sec)\n";
	for (rank_table_type::size_type ri = 0; ri < rank_table.size(); ++ri) {
//		int N_ranks_in_range_11_50 = 0, N_ranks_in_range_51_100 = 0;
//		int start = 10, end = (scores_cutoff < 50) ? scores_cutoff: 50;
//		for (int rank_index = start; rank_index < end; ++rank_index) {
//			N_ranks_in_range_11_50 += rank_table[ri].ranks[rank_index];
//		}
//		start = 50, end = (scores_cutoff < 100) ? scores_cutoff: 100;
//		for (int rank_index = start; rank_index < end; ++rank_index) {
//			N_ranks_in_range_51_100 += rank_table[ri].ranks[rank_index];
//		}
//
//		// this cout is for calculating ranks above 10 until 100
//		cout 
//			 << (ri + 1) * one_mass_class_range
//			 << "         " << rank_table[ri].N_spectra
// 			 << "         " << rank_table[ri].ranks[0]
// 			 << "      " << rank_table[ri].ranks[1]
// 			 << "      " << rank_table[ri].ranks[2] + rank_table[ri].ranks[3] + rank_table[ri].ranks[4]
// 			 << "        " << rank_table[ri].ranks[5] + rank_table[ri].ranks[6] + rank_table[ri].ranks[7] + rank_table[ri].ranks[8] + rank_table[ri].ranks[9]
// 			 << "        " << N_ranks_in_range_11_50
// 			 << "        " << N_ranks_in_range_51_100
// 			 << "        " << rank_table[ri].ranks[100]
//			 << "        " << ((rank_table[ri].N_spectra != 0) ? static_cast<double>(rank_table[ri].N_chemical_filter_ok) / rank_table[ri].N_spectra: 0.0)
				
		cout 
			 << setw(9) << (ri + 1) * one_mass_class_range
			 << setw(11) << rank_table[ri].N_spectra
 			 << setw(7) << rank_table[ri].ranks[0]
 			 << setw(7) << rank_table[ri].ranks[1]
 			 << setw(9) << rank_table[ri].ranks[2] + rank_table[ri].ranks[3] + rank_table[ri].ranks[4]
 			 << setw(10) << rank_table[ri].ranks[5] + rank_table[ri].ranks[6] + rank_table[ri].ranks[7] + rank_table[ri].ranks[8] + rank_table[ri].ranks[9]
//////// 			 << "        " << rank_table[ri].ranks[5] + rank_table[ri].ranks[6] + rank_table[ri].ranks[7]
//////// 			 			  						  + rank_table[ri].ranks[8] + rank_table[ri].ranks[9]
//////// 			 			  	  + rank_table[ri].ranks[1] + rank_table[ri].ranks[2] + rank_table[ri].ranks[3] + rank_table[ri].ranks[4]
 			 << setw(9) << rank_table[ri].ranks[10]
			 << setw(17) << ((rank_table[ri].N_spectra != 0) ? static_cast<double>(rank_table[ri].N_chemical_filter_ok) / rank_table[ri].N_spectra: 0.0)

////////////			 << "        " << rank_table[ri].N_integer
////////////			 << "        " << rank_table[ri].N_real
////////////			 << "        " << rank_table[ri].N_chemical_filter_ok

			 << setw(20) << setprecision(6) << ((rank_table[ri].N_spectra != 0) ? rank_table[ri].runtime / rank_table[ri].N_spectra: 0.0) 
			 << '\n';
			 
			/////////////////////////// collects score schemes statistics //////////////////////
			n_all_spectra += rank_table[ri].N_spectra;
			n_rank1 += rank_table[ri].ranks[0];
			n_rank2_10 += rank_table[ri].ranks[1] + rank_table[ri].ranks[2] + rank_table[ri].ranks[3] + 
							rank_table[ri].ranks[4] + rank_table[ri].ranks[5] + rank_table[ri].ranks[6] + 
							rank_table[ri].ranks[7] + rank_table[ri].ranks[8] + rank_table[ri].ranks[9];
//			n_rank11_100 += N_ranks_in_range_11_50 + N_ranks_in_range_51_100;
			////////////////////////////////////////////////////////////////////////////////////
	}
//	cout << setprecision(6) 
//		 << "# %rank1 = " << static_cast<double>(n_rank1) / n_all_spectra
//		 << "\n# %top10 = " <<  static_cast<double>(n_rank1 + n_rank2_10) / n_all_spectra
//		 << "\n# %top100 = " <<  static_cast<double>(n_rank1 + n_rank2_10 + n_rank11_100) / n_all_spectra 
//		 << '\n';			
	
//	score_scheme_stat_element.n_rank1 = static_cast<double>(n_rank1) / n_all_spectra;
//	score_scheme_stat_element.n_rank2_10 = static_cast<double>(n_rank2_10) / n_all_spectra;
//	score_scheme_stat_element.n_rank11_100 = static_cast<double>(n_rank11_100) / n_all_spectra;	
//	score_schemes_statistics.push_back(score_scheme_stat_element);
		
//	} // end of loop with variable n_combinations_of_scores
//	
//	sort(score_schemes_statistics.begin(), score_schemes_statistics.end(), rank1SortCriterion);
//	cout << "# scoring schemes sorted by #rank1 hits \n";
//	for_each(score_schemes_statistics.begin(), 
//			((score_schemes_statistics.size() < 100) ? score_schemes_statistics.end(): score_schemes_statistics.begin() + 100), 
//			outputScoreSchemeStatistics);
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

	element_type H("H", distributionH);
	element_type C("C", distributionC);
	element_type N("N", distributionN);
	element_type O("O", distributionO);
	element_type P("P", distributionP);
	element_type S("S", distributionS);

// Clorine
	nominal_mass_type massCl = 35;
	peaks_container peaksCl;
	peaksCl.push_back(peaks_container::value_type(-0.031147, 0.7577));
	peaksCl.push_back(peaks_container::value_type());
	peaksCl.push_back(peaks_container::value_type(-0.034097, 0.2423));		

	distribution_type distributionCl(peaksCl, massCl);
	element_type Cl("Cl", distributionCl);

// Bromine
	nominal_mass_type massBr = 79;
	peaks_container peaksBr;
	peaksBr.push_back(peaks_container::value_type(-0.081663, 0.5069));
	peaksBr.push_back(peaks_container::value_type());
	peaksBr.push_back(peaks_container::value_type(-0.083709, 0.4931));		
	
	distribution_type distributionBr(peaksBr, massBr);
	element_type Br("Br", distributionBr);	

	nominal_mass_type massSi = 28;
	peaks_container peaksSi;
	peaksSi.push_back(peaks_container::value_type(-0.023073, 0.9223));
	peaksSi.push_back(peaks_container::value_type(-0.023505, 0.0467));
	peaksSi.push_back(peaks_container::value_type(-0.026230, 0.0310));
	
	distribution_type distributionSi(peaksSi, massSi);
	element_type Si("Si", distributionSi);
	
	chnops.push_back(H);
	chnops.push_back(C);
	chnops.push_back(N);
	chnops.push_back(O);
	chnops.push_back(P);
	chnops.push_back(S);

//	chnops.push_back(Br);
//	chnops.push_back(Cl);
//	chnops.push_back(Si);	
}

void initializeIonsAlphabet(alphabet_t& ions_alphabet) {

	typedef IsotopeDistribution distribution_type;
	typedef IsotopeDistribution::peaks_container peaks_container;
	typedef IsotopeDistribution::nominal_mass_type nominal_mass_type;
	typedef Alphabet::element_type element_type;
	typedef Alphabet::container elements_type;

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

// Silicium
	nominal_mass_type massSi = 28;
	peaks_container peaksSi;
	peaksSi.push_back(peaks_container::value_type(-0.023073, 0.9223));
	peaksSi.push_back(peaks_container::value_type(-0.023505, 0.0467));
	peaksSi.push_back(peaks_container::value_type(-0.026230, 0.0310));
	
	distribution_type distributionSi(peaksSi, massSi);

	element_type Na("Na", distributionNa);
	element_type Cl("Cl", distributionCl);
	element_type Br("Br", distributionBr);
	element_type K("K", distributionK);	
	element_type Si("Si", distributionSi);

	ions_alphabet.push_back(Na);
	ions_alphabet.push_back(Cl);
	ions_alphabet.push_back(Br);
	ions_alphabet.push_back(K);
	ions_alphabet.push_back(Si);
	
}

void outputScoreSchemeStatistics(const ScoreSchemeElement& element) {
	cout << "# score scheme = [m0";
	for (vector<unsigned int>::size_type bi = 0; bi < element.score_onoff_bits.size(); ++bi) {
		if (bi + 1 < element.n_peaks_to_score) {
			if (element.score_onoff_bits[bi] == 1) {
				cout << " m" << bi+1;
			}
		} else {
			if (element.score_onoff_bits[bi] == 1) {				
				cout << " ab" << bi + 1 - element.n_peaks_to_score;
			}
		}
	}
	cout << setprecision(6) 
		 << "], #rank1 = " << element.n_rank1 
		 << ", #top10 = " <<  element.n_rank1 + element.n_rank2_10 
		 << ", #top100 = " <<  element.n_rank1 + element.n_rank2_10 + element.n_rank11_100 
		 << '\n';			
}
