/**
 * Calculates chemically approved sum formulas for a given mass and error.
 * 
 * @author Anton Pervukhin <apervukh@minet.uni-jena.de>
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
#include <ims/alphabet.h>
#include <ims/weights.h>
#include <ims/composedelement.h>
#include <ims/nitrogenrulefilter.h>
#include <ims/utils/stopwatch.h>

using namespace ims;
using namespace std;

typedef Alphabet alphabet_t;
typedef IntegerMassDecomposer<>::decompositions_type decompositions_t;
typedef decompositions_t::value_type decomposition_t;

void initializeCHNOPS(alphabet_t&);

/**
 * Wrapper for command line options. After instantiation,
 * call parse with main's argc, argv (catch TCLAP::ArgException).
 * Then use the public methods to get the values given on the command line.
 */
class CmdOptions {
	public:
		CmdOptions();
		void parse(int argc, char** argv);
						
		double getMass() const { return mass.getValue(); }
		double getError() const { return error.getValue(); }
		double getPrecision() const { return precision.getValue(); }
		unsigned int getNumberOfMassesShown() const { return number_of_masses_shown.getValue(); }		
	private:
		TCLAP::CmdLine cmd;
		mutable TCLAP::ValueArg<double> mass; 							// -m <double>
		mutable TCLAP::ValueArg<double> error;							// -e <double>
		mutable TCLAP::ValueArg<double> precision;						// -p <double>
		mutable TCLAP::ValueArg<unsigned int> number_of_masses_shown;	// -n <integer>
};


CmdOptions::CmdOptions() :
	cmd(TCLAP::CmdLine("Elemental compositions usage:")),
	mass(TCLAP::ValueArg<double>("m", "mass", "Mass for which elemental compositions are to be generated", true, static_cast<double>(100.0), "double")),
	error(TCLAP::ValueArg<double>("e", "error", "Allowed mass error", false, static_cast<double>(2.0), "double")),
	precision(TCLAP::ValueArg<double>("p", "precision", "Precision of decomposition", false, static_cast<double>(1.0e-05), "double")),
	number_of_masses_shown(TCLAP::ValueArg<unsigned int>("n", "number", "Number of masses shown in the output", false, 500, "unsigned integer")) {

	// adds arguments to the command line
	cmd.add(mass);
	cmd.add(error);
	cmd.add(precision);
	cmd.add(number_of_masses_shown);
}

/**
 * may throw TCLAP::ArgException
 */
void CmdOptions::parse(int argc, char** argv) {
	cmd.parse(argc, argv);
}

void printHeader(const alphabet_t& alphabet, const Weights& weights,
                 double precision, double error) {
	cout << setprecision(10) << 
		"# Elemental composition tool \n"
		"#\n"
		"# requested precision   : " << setw(5) << precision << "\n"
		"# gcd-adjusted precision: " << setw(5) << weights.getPrecision() << "\n"
		"# allowed error: " << error << " ppm\n"
		"# alphabet (character, weight, integer weight):\n";
	for (Weights::size_type i = 0; i < weights.size(); ++i) {
		cout
			<< "#  " << setw(6) << alphabet.getName(i)
			<< setw(10) << weights.getAlphabetMass(i) << "\t"
			<< weights[i] << '\n';
	}
	cout << "#\n";
}


int main(int argc, char **argv) {

	typedef IntegerMassDecomposer<>::decomposition_type
												decomposition_type;
	typedef IntegerMassDecomposer<>::value_type integer_value_type;
	
	typedef multimap<string, double, less<string> > output_container;
	try {
		// parses command line
		CmdOptions cmd;
		cmd.parse(argc, argv);

		// initializes alphabet
		alphabet_t alphabet;
		initializeCHNOPS(alphabet);

		// initializes mass
		double mass = cmd.getMass();
		
		// initializes precision
		double precision = cmd.getPrecision();
		if (precision < 0.0 || precision > 1.0) {
			precision = static_cast<double>(1.0e-05);
		}

		// initializes error
		double error_ppm = cmd.getError();
		if (error_ppm < 0.0) {
			error_ppm = static_cast<double>(2.0);
		}

		// calculates absolute error
		double error = mass * error_ppm * 1.0e-06;

		// initializes number of masses shown in the output
		unsigned int n_shown = cmd.getNumberOfMassesShown();
		
		// initializes weights
		Weights weights(alphabet.getMasses(), precision);

		// optimize alphabet by dividing by gcd
		weights.divideByGCD();

		// initializes decomposer
		RealMassDecomposer decomposer(weights);		

		// gets all possible decompositions for the monoisotopic mass with error allowed
		decompositions_t decompositions = 
			decomposer.getDecompositions(mass, error);

		Stopwatch timer;
		timer.start();
		decompositions_t::size_type n_theoretical_formulas = decompositions.size();
		double runtimes = timer.elapsed();

		// initializes order of atoms in which one would
		// like them to appear in the molecule's sequence
		vector<string> elements_order;
		elements_order.push_back("C");
		elements_order.push_back("H");
		elements_order.push_back("N");
		elements_order.push_back("O");
		elements_order.push_back("P");
		elements_order.push_back("S");
	
		output_container output;
		for (decompositions_t::iterator decomps_it = decompositions.begin(); 
			decomps_it != decompositions.end(); ++decomps_it) {

			// creates a candidate molecule out of elemental composition and a set of elements
			ComposedElement candidate_molecule(*decomps_it, alphabet);
			// checks on chemical filter
			if (!NitrogenRuleFilter::isMoleculeValid(candidate_molecule)) {
				continue;
			} 
			// updates molecule's sequence in a given order of elements(atoms)
			candidate_molecule.updateSequence(&elements_order);

			string candidate_molecule_sequence = 
					candidate_molecule.getSequence();
					
			// updates molecule's isotope distribution to calculate its molecule's monoisotopic mass
			candidate_molecule.updateIsotopeDistribution();
			
			double monoisotopicMass = candidate_molecule.getMass();
			
			// stores the score with the sequence
			output.insert(make_pair(candidate_molecule_sequence, monoisotopicMass));

			////////////////////////
			// to be removed
			////////////////////////
			// cout << candidate_molecule_sequence << '\n';
			// cout << candidate_molecule.getIsotopeDistribution() << '\n';
			////////////////////////
		}
	
		printHeader(alphabet, weights, precision, error_ppm);
		cout
			<< "# mass " << setw(10) << mass
			<< " has " << output.size() 
			<< "(" << n_theoretical_formulas << ")"
			<< " decomposition";
		if (decompositions.size() != 1) {
			cout << "s";
		}
		if (n_shown < output.size()) {
			cout << " (showing " << n_shown << ")";
		}
		if (!decompositions.empty()) {
			cout << ":";
		}
		cout << '\n';
	
		cout << setiosflags(ios::left);
		// outputs molecule's sequences & masses.
		for (output_container::const_iterator it = output.begin(); 
					it != output.end() && n_shown > 0; ++it, --n_shown) {
			cout << setw(15) << it->first << ' ' << setw(10) << it->second << '\n';
		}
		cout << "# It takes " << runtimes << " seconds to calculate all decompositions.\n" << endl;

	} catch (IOException& ioe) {
		cerr << "IOException: " << ioe.what() << endl;
		return 1;
	} catch (Exception& e) {
		cerr << "Exception: " << e.what() << endl;
		return 1;
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


}
