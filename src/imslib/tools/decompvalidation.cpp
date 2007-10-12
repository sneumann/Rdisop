/**
 * Validates the output and calculates runtimes for decomposition algorithms over real and integer alphabets.
 * 
 * @author Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 * @author Marcel Martin <Marcel.Martin@CeBiTec.Uni-Bielefeld.DE>
 */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iomanip>

#include <ims/tclap/CmdLine.h>
#include <ims/base/exception/exception.h>
#include <ims/base/exception/ioexception.h>
#include <ims/decomp/realmassdecomposer.h>
#include <ims/decomp/integermassdecomposer.h>
#include <ims/decomp/fastintegermassdecomposer.h>
#include <ims/decomp/classicaldpmassdecomposer.h>
#include <ims/decomp/decomputils.h>
#include <ims/utils/stopwatch.h>
#include <ims/alphabet.h>
#include <ims/weights.h>

using namespace ims;
using namespace std;

typedef MassDecomposer<long unsigned int, unsigned int>::decompositions_type decompositions_t;
typedef decompositions_t::value_type decomposition_t;

/**
 * Wrapper for command line options. After instantiation,
 * call parse with main's argc, argv (catch TCLAP::ArgException).
 * Then use the public methods to get the values given on the command line.
 */
class CmdOptions {
	public:
		CmdOptions();
		void parse(int argc, char** argv);
						
		string getAlphabetFileName() const { return alphabet.getValue(); }
		double getFirstMass() const { return first_mass.getValue(); }
		double getLastMass() const { return last_mass.getValue(); }
		unsigned int getNumberOfMasses() const { return number_of_masses.getValue(); }
		double getError() const { return error.getValue(); }
		double getPrecision() const { return precision.getValue(); }
		bool isRuntimesMode() const { return is_runtimes_mode.getValue(); }
	private:
		TCLAP::CmdLine cmd;
		mutable TCLAP::ValueArg<string> alphabet; 				// -a <filename>		
		mutable TCLAP::ValueArg<double> first_mass; 			// -f <double>
		mutable TCLAP::ValueArg<double> last_mass;				// -l <double>
		mutable TCLAP::ValueArg<unsigned int> number_of_masses;	// -n <integer>
		mutable TCLAP::ValueArg<double> error;					// -e <double>
		mutable TCLAP::ValueArg<double> precision;				// -p <double>
		mutable TCLAP::SwitchArg is_runtimes_mode;				// -r 
};


CmdOptions::CmdOptions() :
	cmd(TCLAP::CmdLine("Decomposition validation usage:")),
		alphabet(TCLAP::ValueArg<string>("a", "alphabet", "File with alphabet masses", true, "", "filename")), 
		first_mass(TCLAP::ValueArg<double>("f", "first", "First mass to validate", true, static_cast<double>(1.0), "double")),
		last_mass(TCLAP::ValueArg<double>("l", "last", "Last mass to validate", false, static_cast<double>(1.0), "double")),
		number_of_masses(TCLAP::ValueArg<unsigned int>("n", "number", "Number of masses to validate", false, 1, "unsigned integer")), 
		error(TCLAP::ValueArg<double>("e", "error", "Allowed mass error", false, static_cast<double>(0.0), "double")),
		precision(TCLAP::ValueArg<double>("p", "precision", "Precision of decomposition", false, static_cast<double>(1.0), "double")),
		is_runtimes_mode(TCLAP::SwitchArg("r", "runtimes", "Generate decomposition runtimes", false)) {
			
	// adds arguments to the command line
	cmd.add(alphabet);
	cmd.add(first_mass);
	cmd.add(last_mass);
	cmd.add(number_of_masses);
	cmd.add(error);
	cmd.add(precision);
	cmd.add(is_runtimes_mode);
}

/**
 * may throw TCLAP::ArgException
 */
void CmdOptions::parse(int argc, char** argv) {
	cmd.parse(argc, argv);
}

decompositions_t getAllDecompositionsExhaustively(double mass, double error, const Weights& weights);
void collectDecompositionsRecursively(double mass, double error, const Weights& weights, 
	decomposition_t::value_type index, decomposition_t decomposition, decompositions_t& container);
decompositions_t getAllDecompositionsOverCHNOPS(double mass, double error);

void printHeader(const Alphabet& alphabet, const Weights& weights,
                 double precision, double error, bool is_runtime_mode) {
	cout << setprecision(8) <<
		"# Decomp validation tool \n"
		"#\n"
		"# requested precision   : " << setw(5) << precision << "\n"
		"# gcd-adjusted precision: " << setw(5) << weights.getPrecision() << "\n"
		"# allowed error: " << error << " ppm\n"
		"# alphabet (character, weight, integer weight):\n";
	for (Weights::size_type i = 0; i < weights.size(); ++i) {
		cout
			<< "#  " << alphabet.getName(i) << "\t"
			<< weights.getAlphabetMass(i) << "\t"
			<< weights[i] << '\n';
	}
	cout << "#\n";
	if (is_runtime_mode) {
		if (precision < 1.0) {
			cout << "# runtime1 - our algorithm with residue table\n";
			cout << "# runtime2 - an exhaustive algorithm with not fixed alphabet\n"
				 << "#                        size (using 'for' loop recursively)\n";
			cout << "# runtime3 - an exhaustive algorithm (using 'for' loops)\n" 
				 << "#                  with fixed (hard coded) alphabet size\n";
				 
			cout << "#\n";
		} else {
			cout << "# runtime1 - our algorithm with residue table non-recursive\n";
			cout << "# runtime1 - our algorithm with residue table recursive\n";
			cout << "# runtime2 - classical dp algorithm\n";
		}
		cout << "# mass   #decomps    runtime1    runtime2    runtime3";
//		if (precision < 1.0) {
//			cout << "    runtime3"; 
//		}
		cout << endl;
	}
}


int main(int argc, char **argv) {

	typedef IntegerMassDecomposer<>::decomposition_type
												decomposition_type;
	typedef IntegerMassDecomposer<>::value_type integer_value_type;

	try {
		// parses command line
		CmdOptions cmd;
		cmd.parse(argc, argv);

		// initializes alphabet
		Alphabet alphabet;
		alphabet.load(cmd.getAlphabetFileName());
		alphabet.sortByValues();
		
		// initializes precision
		double precision = cmd.getPrecision();
		if (precision < 0.0 || precision > 1.0) {
			precision = static_cast<double>(1.0);
		}

		precision *= alphabet.getMass(0);

		// initializes error
		double error_ppm = cmd.getError();
		if (error_ppm < 0.0) {
			error_ppm = static_cast<double>(0.0);
		}

		// initializes weights
		Weights weights(alphabet.getMasses(), precision);

		// optimize alphabet by dividing by gcd
		weights.divideByGCD();

		// containers for decompositions
		decompositions_t our_decompositions, other_decompositions, yet_another_decompositions;
		// variables to keep runtimes
		double our_runtimes, other_runtimes, yet_another_runtimes = static_cast<double>(0.0);

		// initializes first and last mass and loop step
		double first_mass = cmd.getFirstMass();
		if (first_mass < alphabet.getMass(0)) {
			first_mass = alphabet.getMass(0);
		}
		double last_mass = cmd.getLastMass();
		double loop_step;
		if (cmd.getNumberOfMasses() > 1) {
			loop_step = (last_mass - first_mass) / (cmd.getNumberOfMasses() - 1);
		} else {
			loop_step = 1;
			last_mass = first_mass;
		}
		
		if (loop_step < precision) {
			loop_step = precision;
		}
		
		printHeader(alphabet, weights, precision, error_ppm, cmd.isRuntimesMode());
		
		Stopwatch timer;
		// loops through the given masses
		for (double mass = first_mass; mass <= last_mass; mass += loop_step) {

			// calculates absolute error
			double error = mass * error_ppm * 1.0e-06;

			// decomposes real values
			if (precision < 1.0) {

				// initializes decomposers which uses approach with residues
				RealMassDecomposer rt_decomposer(weights);

				timer.start();				
				our_decompositions = rt_decomposer.getDecompositions(mass, error);
				our_runtimes = timer.elapsed();
				
				timer.start();
				other_decompositions = getAllDecompositionsExhaustively(mass, error, weights);
				other_runtimes = timer.elapsed();
				
				timer.start();
				yet_another_decompositions = getAllDecompositionsOverCHNOPS(mass, error);	
				yet_another_runtimes = timer.elapsed();
				

			// decomposes integer values
			} else {
		
				IntegerMassDecomposer<> rt_decomposer(weights);
				
				FastIntegerMassDecomposer<> fast_rt_decomposer(weights);

				// initializes decomposer which implements classical dp algorithm
				ClassicalDPMassDecomposer<> dp_decomposer(weights);	
				
				integer_value_type integer_mass = static_cast<integer_value_type>(mass);
				
				timer.start();				
				our_decompositions = fast_rt_decomposer.getAllDecompositions(integer_mass);
				our_runtimes = timer.elapsed();

				timer.start();
				other_decompositions = rt_decomposer.getAllDecompositions(integer_mass);
				other_runtimes = timer.elapsed();

				timer.start();
				yet_another_decompositions = dp_decomposer.getAllDecompositions(integer_mass);
				yet_another_runtimes = timer.elapsed();
				
			}
			
			if (cmd.isRuntimesMode()) {
				cout << mass << "   " 
				    << our_decompositions.size() << "   "  
					<< our_runtimes << "   " 
					<< other_runtimes << "   ";
				if (yet_another_runtimes > 0.0) {
					cout << yet_another_runtimes << '\n';
				} else {
					cout << '\n';
				}
			} else {
			
			// compares containers with decompositions
			bool they_differ = false;
			if (our_decompositions.size() == other_decompositions.size()) {
				for (decompositions_t::const_iterator it1 = our_decompositions.begin(); 
								it1 != our_decompositions.end(); ++it1) {
					decompositions_t::iterator it2 = find(other_decompositions.begin(),
						other_decompositions.end(), *it1);
					if (it2 == other_decompositions.end()) {
						they_differ = true;
						break;
					}
				}				
			} else {
				they_differ = true;
			}
	
			// if containers are different
			if (they_differ) {
				cout << "results differ!\n";
				cout << "our #" << our_decompositions.size() << " decompositions from mass " << mass << '\n';	
				cout << "not our #" << other_decompositions.size() << " decompositions from mass " << mass << '\n';
				cout << "decompositions found in our and not in other algorithm:\n";
				for (decompositions_t::const_iterator it1 = our_decompositions.begin(); 
								it1 != our_decompositions.end(); ++it1) {
					decompositions_t::iterator it2 = find(other_decompositions.begin(),
						other_decompositions.end(), *it1);
					if (it2 == other_decompositions.end()) {
						// outputs decomposition
						bool first = true;
						for (Alphabet::size_type i = 0; i < alphabet.size(); ++i) {
							if ((*it1)[i] > 0) {
								if (!first) {
									cout << " ";
								} else {
									first = false;
								}
								cout << alphabet.getName(i) << (*it1)[i];
							}
						}
						double parentMass = DecompUtils::getParentMass(weights, *it1);
						Weights::weight_type integer_parent_mass = 
								DecompUtils::getIntegerParentMass(weights, &(*it1));
						cout << " (" << parentMass <<  "; " << parentMass - mass << "; " << integer_parent_mass << ")\n";
					}
				}				

				cout << "decompositions found in other and not in our algorithm:\n";
				for (decompositions_t::const_iterator it1 = other_decompositions.begin(); 
								it1 != other_decompositions.end(); ++it1) {
					decompositions_t::iterator it2 = find(our_decompositions.begin(),
						our_decompositions.end(), *it1);
					if (it2 == our_decompositions.end()) {
						// outputs decomposition
						bool first = true;
						for (Alphabet::size_type i = 0; i < alphabet.size(); ++i) {
							if ((*it1)[i] > 0) {
								if (!first) {
									cout << " ";
								} else {
									first = false;
								}
								cout << alphabet.getName(i) << (*it1)[i];
							}
						}
						double parentMass = DecompUtils::getParentMass(weights, *it1);
						Weights::weight_type integer_parent_mass = 
								DecompUtils::getIntegerParentMass(weights, &(*it1));
						
						cout << " (" << parentMass <<  "; " << parentMass - mass << "; " << integer_parent_mass << ")\n";
					}
				}				
				
			// if containers are equal
			} else {
				if (precision < 1.0) {
					cout << '#' << our_decompositions.size() << " decompositions compared ok. mass = " << mass << " +/- " << error << '\n';
//					for (decompositions_t::const_iterator it1 = our_decompositions.begin(); 
//								it1 != our_decompositions.end(); ++it1) {
//						// outputs decomposition
//						bool first = true;
//						for (Alphabet::size_type i = 0; i < alphabet.size(); ++i) {
//							if ((*it1)[i] > 0) {
//								if (!first) {
//									cout << " ";
//								} else {
//									first = false;
//								}
//								cout << alphabet.getName(i) << (*it1)[i];
//							}
//						}
//						double parentMass = DecompUtils::getParentMass(weights, *it1);
//						Weights::weight_type integer_parent_mass = 
//								DecompUtils::getIntegerParentMass(weights, &(*it1));
//						cout << " (" << parentMass <<  "; " << parentMass - mass << "; " << integer_parent_mass << ")\n";
//					}				
					
				} else {
					cout << '#' << our_decompositions.size() << " decompositions compared ok. mass = " << mass << '\n';
				}
			} // comparison of containers

			} // if for modes.
			
		} // end of loop through masses


	} catch (IOException& ioe) {
		cerr << "IOException: " << ioe.what() << endl;
		return 1;
	} catch (Exception& e) {
		cerr << "Exception: " << e.what() << endl;
		return 1;
	}
	return 0;
}

decompositions_t getAllDecompositionsExhaustively(double mass, double error, const Weights& weights) {
	decompositions_t container;
	decomposition_t decomposition(weights.size());
	collectDecompositionsRecursively(mass, error, weights, weights.size()-1, decomposition, container);
	return container;
}

void collectDecompositionsRecursively(double mass, double error, const Weights& weights, 
	decomposition_t::value_type index, decomposition_t decomposition, decompositions_t& container) {

	typedef decomposition_t::value_type decomposition_value_type;
	
	if (mass <= error) {
		container.push_back(decomposition);
		return;
	}

	for (decomposition_value_type n_i = 0; mass >= -error; 
					++n_i, mass -= weights.getAlphabetMass(index)) {
		if (index > 0) {
			decomposition[index] = n_i;
			collectDecompositionsRecursively(mass, error, 
										weights, index-1, decomposition, container);
		} else {
			for (decomposition_value_type n_0 = 0; mass >= -error; 
						++n_0, mass -= weights.getAlphabetMass(0)) {
				if (mass < error) {
					decomposition[0] = n_0;
					container.push_back(decomposition);
					return;
				}
			}
		}
	}
	
}

// hard coded 6 for-loops over chnops
decompositions_t getAllDecompositionsOverCHNOPS(double mass, double error) {

	decompositions_t result;

	typedef decomposition_t::value_type decomposition_value_type;
	
	double mass_H = 1.007825, mass_C = 12.0, mass_N = 14.003074, 
		mass_O = 15.994915, mass_P = 30.973762, mass_S = 31.972071;	

	// starts "6 for-loops" algorithm hard coded over chnops
	double rest_mass_chnop = mass;
	for (decomposition_value_type is = 0; rest_mass_chnop >= -error; ++is, rest_mass_chnop -= mass_S) {
				
		double rest_mass_chno = rest_mass_chnop;
		for (decomposition_value_type ip = 0; rest_mass_chno >= -error; ++ip, rest_mass_chno -= mass_P) {
					
			double rest_mass_chn = rest_mass_chno;
			for (decomposition_value_type io = 0; rest_mass_chn >= -error; ++io, rest_mass_chn -= mass_O) {
							
				double rest_mass_ch = rest_mass_chn;
				for (decomposition_value_type in = 0; rest_mass_ch >= -error; ++in, rest_mass_ch -= mass_N) {
								
					double rest_mass_c = rest_mass_ch;
					for (decomposition_value_type ic = 0; rest_mass_c >= -error; ++ic, rest_mass_c -= mass_C) {
									
						double rest_mass = rest_mass_c;
						for (decomposition_value_type ih = 0; rest_mass >= -error; ++ih, rest_mass -= mass_H) {
							if (rest_mass <= error) {
								decomposition_t decomposition;
								decomposition.push_back(ih);
								decomposition.push_back(ic);
								decomposition.push_back(in);
								decomposition.push_back(io);							
								decomposition.push_back(ip);
								decomposition.push_back(is);
								result.push_back(decomposition);											
							}
										
						} // element H
					} // element C
				} // element N
			} // element O
		} // element p
	} // element S
	
	return result;
}
