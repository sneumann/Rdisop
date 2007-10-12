/**
 * number_decompositions.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <ims/tclap/CmdLine.h>
#include <ims/base/exception/exception.h>
#include <ims/base/exception/ioexception.h>
#include <ims/utils/math.h>
#include <ims/alphabet.h>
#include <ims/weights.h>
#include <ims/utils/stopwatch.h>
#include <ims/decomp/realmassdecomposer.h>

using namespace ims;
using namespace std;

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
	private:
		TCLAP::CmdLine cmd;
		mutable TCLAP::ValueArg<string> alphabet; 				// -a <filename>		
		mutable TCLAP::ValueArg<double> first_mass; 				// -f <double>
		mutable TCLAP::ValueArg<double> last_mass;				// -l <double>
};


CmdOptions::CmdOptions() :
	cmd(TCLAP::CmdLine("Number of Decompositions usage:")),
		alphabet(TCLAP::ValueArg<string>("a", "alphabet", "File with alphabet masses", true, "", "filename")), 
		first_mass(TCLAP::ValueArg<double>("f", "first", "First mass to decompose", true, static_cast<double>(1000.0), "double")),
		last_mass(TCLAP::ValueArg<double>("l", "last", "Last mass to decompose", false, static_cast<double>(-1.0), "double")) {
			
	// adds arguments to the command line
	cmd.add(alphabet);
	cmd.add(first_mass);
	cmd.add(last_mass);
}

/**
 * may throw TCLAP::ArgException
 */
void CmdOptions::parse(int argc, char** argv) {
	cmd.parse(argc, argv);
}

void printHeader(const Alphabet& alphabet, const Weights& weights,
                 double precision) {
	cout << setprecision(8) <<
		"# Tool to calculate a number of decompositions:\n"
		"#\n"
		"# requested precision   : " << setw(5) << precision << "\n"
		"# gcd-adjusted precision: " << setw(5) << weights.getPrecision() << "\n"
		"# alphabet (character, weight, integer weight):\n";
	for (Weights::size_type i = 0; i < weights.size(); ++i) {
		cout
			<< "#  " << alphabet.getName(i) << "\t"
			<< weights.getAlphabetMass(i) << "\t"
			<< weights[i] << '\n';
	}
	cout << "#\n";
}

int main(int argc, char** argv) {
	// initialize alphabets
	typedef RealMassDecomposer decomposer_type;
	typedef decomposer_type::decompositions_type decompositions_type;
	typedef decomposer_type::number_of_decompositions_type number_of_decompositions_type;
	try {
		// parses command line
		CmdOptions cmd;
		cmd.parse(argc, argv);

		// initializes alphabet
		Alphabet alphabet;
		alphabet.load(cmd.getAlphabetFileName());
		alphabet.sortByValues();

		// initializes precision
		double precision = 0.0001;
		// in case if the alphabet smallest mass is not small 
		// (say, we use the alphabet of amino-acids or nucleotides)
		// the precision is adjusted
		if (alphabet.getMass(0) > static_cast<double>(20.0)) {
			precision = 0.01;
		}

		// initializes first mass
		double first_mass = cmd.getFirstMass();
		if (first_mass < alphabet.getMass(0)) {
			throw Exception("First mass to decompose is too small!");
		}
		// initializes last mass
		double last_mass = cmd.getLastMass();
		// if last mass is not set, than the range [0; first_mass] is assumed
		if (last_mass == static_cast<double>(-1.0)) {
			last_mass = first_mass;
			first_mass = static_cast<double>(0.0);
		} else if (last_mass <= first_mass) {
			throw Exception("Last mass must be bigger than first mass!");
		}

		// initializes weights
		Weights weights(alphabet.getMasses(), precision);

		// optimize alphabet by dividing by gcd
		weights.divideByGCD();
		
		double mass = (last_mass + first_mass) / 2;
		double mass_error = last_mass - mass;

		decomposer_type decomposer(weights);

		printHeader(alphabet, weights, precision);

		Stopwatch timer;
		timer.start();
		number_of_decompositions_type n_decompositions = 
			decomposer.getNumberOfDecompositions(mass, mass_error);
		double runtime = timer.elapsed();
		cout << "# Mass range [" << first_mass << ';' << last_mass << "] contains " 
			<< n_decompositions << " decompositions.\n";
		cout << "# It took " << runtime << " seconds to calculate.\n";

	} catch (IOException& ioe) {
		cerr << "IOException: " << ioe.what() << endl;
		return 1;
	} catch (Exception& e) {
		cerr << "Exception: " << e.what() << endl;
		return 1;
	}
	return 0;

}
