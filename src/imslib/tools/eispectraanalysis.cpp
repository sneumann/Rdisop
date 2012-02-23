/**
 * Analyses a spectrum from an EI database file, writes output in DOT-Format.
 * 
 * @author Florian Rasche <m3rafl@minet.uni-jena.de>
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iomanip>

#include <ims/tclap/CmdLine.h>
#include <ims/base/exception/exception.h>
#include <ims/base/exception/ioexception.h>
#include <ims/base/parser/standardmoleculesequenceparser.h>
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

		string getInput() const { return input.getValue(); }
		unsigned int getNumber() const { return number.getValue(); }
		unsigned int getOffset() const { return offset.getValue(); }
		double getCutoff() const { return cutoff.getValue(); }
		double getError() const { return error.getValue(); }		
		string getOutput() const { return output.getValue(); }
		//double getPrecision() const { return precision.getValue(); }
		//unsigned int getNumberOfMassesShown() const { return number_of_masses_shown.getValue(); }		
	private:
		TCLAP::CmdLine cmd;
		mutable TCLAP::ValueArg<string> input; 							// -i <string>
		mutable TCLAP::ValueArg<unsigned int> number;						// -n <integer>
		mutable TCLAP::ValueArg<unsigned int> offset;
		mutable TCLAP::ValueArg<double> cutoff;
		mutable TCLAP::ValueArg<double> error;
		mutable TCLAP::ValueArg<string> output; 						// -o <string>
		//mutable TCLAP::ValueArg<double> precision;						// -p <double>
		//mutable TCLAP::ValueArg<unsigned int> number_of_masses_shown;	// -n <integer>
};


CmdOptions::CmdOptions() :
	cmd(TCLAP::CmdLine("EI spectrum analysis usage:")),
	input(TCLAP::ValueArg<std::string>("i", "input", "Input directory name", true, "", "string")),
	number(TCLAP::ValueArg<unsigned int>("n", "number", "Number of Spectra to be processed", false, 10000, "unsigned integer")),
	offset(TCLAP::ValueArg<unsigned int>("f", "offset", "At which Spectrum shall we start", false, 1, "unsigned integer")),	
	cutoff(TCLAP::ValueArg<double>("c", "cutoff", "What is to be considered as noise?", true, 1000.0, "double")),
	error(TCLAP::ValueArg<double>("e", "error", "Allowed mass error", false, static_cast<double>(10.0), "double")),	
	output(TCLAP::ValueArg<std::string>("o", "output", "Output file name", false, "output.txt", "string")) {
	//number_of_masses_shown(TCLAP::ValueArg<unsigned int>("n", "number", "Number of masses shown in the output", false, 500, "unsigned integer")) {

	// adds arguments to the command line
	cmd.add(input);
	cmd.add(number);
	cmd.add(offset);
	cmd.add(output);
	cmd.add(error);
	cmd.add(cutoff);
	//cmd.add(number_of_masses_shown);
}

/**
 * may throw TCLAP::ArgException
 */
void CmdOptions::parse(int argc, char** argv) {
	cmd.parse(argc, argv);
}

bool isConsistentWith(const ComposedElement::container & child, const AbstractMoleculeSequenceParser::container & parent) {
	for (ComposedElement::container::const_iterator cit = child.begin(); cit != child.end(); ++cit){
		AbstractMoleculeSequenceParser::container::const_iterator parent_it = parent.find(cit->first.getName());
		if (parent_it == parent.end()) return false;
		if (parent_it->second < cit->second) return false;
	}
	return true;
}

string sixdigitnumber(unsigned int number){
  stringstream ss;
  ss << number;
  string result = ss.str();
  result.insert(0, 6-result.length(), '0');
  return result;
}

int main(int argc, char **argv) {

	typedef IntegerMassDecomposer<>::decomposition_type decomposition_type;
	typedef IntegerMassDecomposer<>::value_type integer_value_type;
	
	typedef multimap<string, double, less<string> > output_container;
	// parses command line
	CmdOptions cmd;
	cmd.parse(argc, argv);

	// initializes alphabet
	alphabet_t alphabet;
	initializeCHNOPS(alphabet);

	// initializes precision
	double precision = static_cast<double>(1.0e-05);


	// initializes error
	double error_ppm = cmd.getError();

	// initializes weights
	Weights weights(alphabet.getMasses(), precision);

	// optimize alphabet by dividing by gcd
	weights.divideByGCD();

	// initializes decomposer
	RealMassDecomposer decomposer(weights);

	// initializes order of atoms in which one would
	// like them to appear in the molecule's sequence
	vector<string> elements_order;
	elements_order.push_back("C");
	elements_order.push_back("H");
	elements_order.push_back("N");
	elements_order.push_back("O");
	elements_order.push_back("P");
	elements_order.push_back("S");
	elements_order.push_back("Cl");
	//elements_order.push_back("Br");
	//elements_order.push_back("Si");
	//elements_order.push_back("I");
	

	// Open outfile and first infile
	string directory = cmd.getInput();
	unsigned int offset = cmd.getOffset();
	string currentCompound = "";
	AbstractMoleculeSequenceParser::container elements;
	ifstream infile((directory + '/' + sixdigitnumber(offset) + ".html").c_str());
	ofstream outfile(cmd.getOutput().c_str());
	string line;
	unsigned int spectraDone = 0, maxCompounds = cmd.getNumber();
	unsigned int truePositive = 0, trueNegative = 0, falseNegative = 0, falsePositive = 0, totalPeaks = 0;
	double cutoff = cmd.getCutoff();
	vector<double> peaks, intensities;
	Alphabet::mass_type parentMass = 0.0;
	double p = 0.0, i = 0.0; // store the peak and intensity extracted from stringstream
	istringstream ss;
	double mass = 0.0, intensity = 0.0, error = 0.0;
	decompositions_t decompositions;
	decompositions_t::size_type n_theoretical_formulas = 0;
	ComposedElement candidate_molecule("H",alphabet);
	output_container output;


	while (infile.good() && spectraDone < maxCompounds){ // loop over compounds
		try {
		  while (getline(infile, line) && line.find("CH$NAME") == string::npos) {}
			//Write header only if compound changed
			if (line.substr(line.find("CH$NAME")+9) != currentCompound) {
				currentCompound = line.substr(line.find("CH$NAME")+9);
				outfile << endl << currentCompound << endl; // compound name as short header
				// Read molecule sequence associated with this entry
				parentMass = 0.0;
				// If first char of line is a digit, the peaks are reached, no formula will be found for the compuond!
				while (getline(infile, line) && line.find("CH$FORMULA") == string::npos){}
				elements = AbstractMoleculeSequenceParser::container();
					if (line.find("{")== string::npos) {
						elements = MoleculeSequenceParser(line.substr(12)).getElements();
					} else {
						elements = StandardMoleculeSequenceParser(line.substr(12)).getElements();
					}
					for (StandardMoleculeSequenceParser::container::const_iterator cit=elements.begin(); cit != elements.end(); ++cit){
						parentMass += (cit->second)*alphabet.getElement(cit->first).getMass();
					}
				  outfile << line.substr(3) << endl << " Parent Mass: " << parentMass << endl;
					// Exit if we dont know the formula
					if (parentMass == 0.0) {
					  cout << "formula for spectrum number " << spectraDone+offset << "not found" << endl;
					  return 0;
					}
				// Write name of spectrometer
				while (getline(infile, line) && line.find("AC$INSTRUMENT") == string::npos) {}
				outfile << line.substr(line.find("AC")+3) << endl;
				peaks.push_back(parentMass);
				intensities.push_back(0.0); // intensities must have corresponding entries to peaks				
			}
			
			// Output the number of the current spectrum
			outfile << "Spectrum number: " << spectraDone+offset << endl;
			// Read all peaks
			while (getline(infile, line) && line.find("PK$PEAK") == string::npos){}
			// Read the peaks
			while (getline(infile, line) && line.find("//") == string::npos){
				ss.clear();
				ss.str(line);
				ss >> p >> i;
				peaks.push_back(p);
				intensities.push_back(i);
			}

			// Calculate theoretical formulas for every peak
			for (vector<double>::const_iterator cit = peaks.begin(), iit = intensities.begin(); 
			     cit != peaks.end() && iit != intensities.end(); ++cit, ++iit){
				mass = *cit, intensity = *iit;
				// calculates absolute error
				error = mass * error_ppm * 1.0e-06;
				// gets all possible decompositions for the monoisotopic mass with error allowed
				decompositions = decomposer.getDecompositions(mass, error);
				n_theoretical_formulas = decompositions.size();
			
				output.clear();
				for (decompositions_t::iterator decomps_it = decompositions.begin(); 
					decomps_it != decompositions.end(); ++decomps_it) {
		
					// creates a candidate molecule out of elemental composition and a set of elements
					candidate_molecule = ComposedElement(*decomps_it, alphabet);
					// checks on chemical filter
					if (!isConsistentWith(candidate_molecule.getElements(), elements)) {
						continue;
					} 
					// updates molecule's sequence in a given order of elements(atoms)
					candidate_molecule.updateSequence(&elements_order);
		
					string candidate_molecule_sequence = 
							candidate_molecule.getSequence();
							
					// updates molecule's isotope distribution to calculate its molecule's monoisotopic mass
					candidate_molecule.updateIsotopeDistribution();
					
					// stores the score with the sequence
					output.insert(make_pair(candidate_molecule_sequence, candidate_molecule.getMass()));
				}
			
				outfile
					<< " peak " << setw(10) << mass
					<< " has " << output.size() 
					<< "(" << n_theoretical_formulas << ")"
					<< " decomposition";
				if (output.size() != 1) {
					outfile << "s";
				}
				outfile << " Intensity: " << intensity << endl;
			
				//if (output.size() < 12){
					// output of molecules
					outfile << setiosflags(ios::left);
					// outputs molecule's sequences & masses.
					for (output_container::const_iterator it = output.begin(); it != output.end(); ++it) {
						outfile << "  " << setw(15) << it->first << ' ' << setw(10) << it->second << '\n';
					}
				//}
				//Statistics
				++totalPeaks;
				if (intensity < cutoff){
				  if (output.size() > 0) { ++falseNegative; }
				  else { ++trueNegative; }
				} else {
				  if (output.size() == 0) { ++falsePositive; }
				  else { ++truePositive; }				  
				}
			} //Till here decompositions are calculated
		} catch (IOException& ioe) {
			cerr << "IOException: " << ioe.what() << endl;
			currentCompound = "yyyyyyyyyyyyyyyyyyyyyyyyy"; // Reset compoundName to check on next spectrum. 
		} catch (Exception& e) {
			cerr << "Exception: " << e.what() << endl;
			currentCompound = "yyyyyyyyyyyyyyyyyyyyyyyyy";
		}						
		decompositions.clear();
		output.clear();
		peaks.clear();
		intensities.clear();
		++spectraDone;
		infile.close();
		infile.open((directory+'/'+sixdigitnumber(spectraDone+offset)+".html").c_str());
	} //end while compound-loop
	infile.close();
	outfile.close();
	cout << truePositive << " Peaks were intenser than " << cutoff << " and meaningful" << endl;
	cout << trueNegative << " Peaks were less intense than " << cutoff << " and meaningless" << endl << endl;		
	cout << falsePositive << " Peaks were intenser than " << cutoff << " but meaningless" << endl;
	cout << falseNegative << " Peaks were less intense than " << cutoff << " but meaningful" << endl;
	cout << totalPeaks << " Peaks were analyzed in total." << endl; 
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

	nominal_mass_type massI = 127;
	peaks_container peaksI;
	peaksI.push_back(peaks_container::value_type(-0.1, 1.0));
	
	distribution_type distributionI(peaksI, massI);
	element_type I("I", distributionI);

	chnops.push_back(H);	
	chnops.push_back(C);
	chnops.push_back(N);
	chnops.push_back(O);
	chnops.push_back(P);
	chnops.push_back(S);
	chnops.push_back(Cl);
	//chnops.push_back(Br);
	//chnops.push_back(Si);
	//chnops.push_back(I);	


}
