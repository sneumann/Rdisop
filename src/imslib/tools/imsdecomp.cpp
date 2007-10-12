/**
 * @b imsdecomp is a command line front end to the decomposition classes
 * implemented in imslib.
 *
 * @author Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 * @author Marcel Martin <Marcel.Martin@CeBiTec.Uni-Bielefeld.DE>
 */
/*
TODO (nach Prioritaet sortiert)
- Test Case!!!
- Fuer den Praxiseinsatz waere es vielleicht ganz praktisch, wenn man die
  Massen via stdin eingeben koennte (und nach jeder Zeile die Compomere
  ausgegeben werden). Das ist nicht das gleiche wie "-f /dev/stdin" anzugeben.
*/

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iomanip>
#include <sstream>

#include <ims/base/exception/exception.h>
#include <ims/base/exception/ioexception.h>
#include <ims/base/parser/massestextparser.h>
#include <ims/decomp/realmassdecomposer.h>
#include <ims/decomp/decomputils.h>
#include <ims/utils/print.h>
#include <ims/utils/math.h>
#include <ims/alphabet.h>
#include <ims/weights.h>

#include "decompcommandline.h"

#define PROGRAM_NAME "imsdecomp"
#define PROGRAM_VERSION "1.1"

using namespace ims;
using namespace std;

typedef IntegerMassDecomposer<>::decompositions_type decompositions_t;

template <typename MassType>
void outputDecompositions(const decompositions_t& decompositions,
						  const Alphabet& alphabet, const Weights& weights,
						  MassType mass, unsigned int maxNumber,
						  bool showParentMass, bool showErrorDelta);


void printHeader(const Alphabet& alphabet, const Weights& weights,
                 double precision, double error, string& errorUnits, const vector<string>& modifications, bool showParentMass, bool showErrorDelta, const string& massMode, bool automaticPrecision) {
	cout << setprecision(8) <<
		"# " PROGRAM_NAME " " PROGRAM_VERSION "\n"
		"# Copyright 2007 Informatics for Mass Spectrometry group\n"
		"#                at Bielefeld University\n"
		"#\n"
		"# http://BiBiServ.TechFak.Uni-Bielefeld.DE/decomp/\n"
		"#\n";
	if (automaticPrecision) {
		cout << "# precision: " << setw(5) << weights.getPrecision() << "\n";
	} else {
		cout <<
			"# requested precision   : " << setw(5) << precision << "\n"
			"# gcd-adjusted precision: " << setw(5) << weights.getPrecision() << "\n";
	}
	cout <<
		"# allowed error: " << error << ' ' << errorUnits << "\n"
		"# modifications:";
	if (modifications.empty()) {
		cout << " none\n";
	} else {
		cout << '\n';
		for (vector<string>::const_iterator it = modifications.begin(); 
						it != modifications.end(); ++it) {
			cout
				<< "# " << setw(30) << *it << '\n';
		}
	}
	if (!modifications.empty()) {
		cout << "# type of modifications: " << massMode << '\n'; 
	}
	cout << "# alphabet (character, mass, integer mass):\n";
	for (Weights::size_type i = 0; i < weights.size(); ++i) {
		cout
			<< "#  " << setw(6) << alphabet.getName(i)
			<< setw(10) << weights.getAlphabetMass(i) << "\t"
			<< weights[i] << '\n';
	}
	cout << "#\n";
	if (showParentMass || showErrorDelta) {
		cout << "# Shown in parentheses after each decomposition:\n";
		if (showParentMass) {
			cout << "# - actual mass\n";
		}
		if (showErrorDelta) {
			cout << "# - deviation from actual mass\n";
		}
		cout << "#\n";
	}
}

struct Modification {

	Modification(string short_name, string long_name, 
			double mono_mass, double average_mass): short_name(short_name),
		long_name(long_name), mono_mass(mono_mass), average_mass(average_mass) {}

	Modification(string name, double mass) : short_name(name),
		long_name(name), mono_mass(mass), average_mass(mass) {}

	Modification(string short_name, string long_name, double mass) : short_name(short_name),
		long_name(long_name), mono_mass(mass), average_mass(mass) {}

	Modification(string name, double mono_mass, double average_mass) : short_name(name),
		long_name(name), mono_mass(mono_mass), average_mass(average_mass) {}

	string short_name;
	string long_name;
	double mono_mass;
	double average_mass;
};

typedef map<string, Modification> modifications_t;

void initializeModifications(modifications_t&);

int main(int argc, char **argv) {

	try {

		// parses command line
		DecompCommandLine cmd;
		cmd.parse(argc, argv);

		// initializes alphabet
		Alphabet alphabet;
		alphabet.load(cmd.getAlphabetFileName());

		bool automaticPrecision = false;

		// gets decomposition precision
		double precision = cmd.getPrecision();

		if (precision == 0.0) {
			// 0.0 is the default, which means that no '-p' option was
			// given on the command line. We instead compute the precision
			// automatically.

			// smallest alphabet mass times 4e-5.
			// Note: it has to be 'floored' to avoid the issue with decimal points
			precision = floor(alphabet.getMass(0)) * 4.0e-05;
			automaticPrecision = true;
		}

		// initializes weights
		Weights weights(alphabet.getMasses(), precision);

		// optimize alphabet by dividing by gcd
		weights.divideByGCD();

		// gets masses
		vector<double> masses;
		if (cmd.isMassesNumbersSet()) {
			masses = cmd.getMasses();
		} else if (cmd.isMassesFileSet()) {
			MassesTextParser parser;
			parser.load(cmd.getMassesFile());
			masses = parser.getElements();
		} else {
			cerr << "Neither -m nor -f option provided!" << endl;
			return 1;
		}
		if (masses.empty()) {
			cerr << "No input masses given to decompose!\n";
			return 1;
		}

		modifications_t name2modification;
		initializeModifications(name2modification);
		vector<string> modifications_headers;

		vector<string> modifications = cmd.getModifications();

		string mass_mode = cmd.getMassMode();

		if (!modifications.empty()) {
			double accumulated_modification = 0.0;
			for (vector<string>::const_iterator it = modifications.begin(); it != modifications.end(); ++it) {
				string modification = *it;
				istringstream modification_stream(modification);
				double mass = 0.0;
				if (!(modification_stream >> mass)) {
					modifications_t::iterator pos = name2modification.find(modification);
					if (pos != name2modification.end()) {
						if (mass_mode == "average") {
							mass = pos->second.average_mass;
						} else {
							mass = pos->second.mono_mass;
						}
						modifications_headers.push_back(pos->second.long_name);
					}
				} else {
					ostringstream header; 
					header << "Custom (" << mass << "Da)";
					modifications_headers.push_back(header.str());
				}
				accumulated_modification += mass;
			}
			if (accumulated_modification > 0.0) {
				transform(masses.begin(), masses.end(), masses.begin(), bind2nd(minus<double>(), accumulated_modification));
			}
		}

		double error = cmd.getError();
		
		string errorUnits = cmd.getErrorUnits();
		// gets maximal number of decompositions to be shown
		unsigned int maxNumber = cmd.getMaxNumberDecompositions();

		printHeader(alphabet, weights, precision, error, errorUnits, modifications_headers, cmd.isShowParentMassSet(), cmd.isShowErrorDeltaSet(), mass_mode, automaticPrecision);

		// decomposes real values
		RealMassDecomposer decomposer(weights);

		double absolute_error = error;
		// loops through the given masses
		for (vector<double>::const_iterator it = masses.begin();
											it != masses.end(); ++it) {
			double mass = *it;
			
			if (errorUnits == "ppm") {
				absolute_error = mass * error * 1.0e-06;
			}

			outputDecompositions(decomposer.getDecompositions(mass, absolute_error),
				alphabet, weights, mass, maxNumber,
				cmd.isShowParentMassSet(),
				cmd.isShowErrorDeltaSet());
		} // end of loop through masses
		cout << "# done\n";
	} catch (IOException& ioe) {
		cerr << "IOException: " << ioe.what() << endl;
		return 1;
	} catch (Exception& e) {
		cerr << "Exception: " << e.what() << endl;
		return 1;
	}
	return 0;
}

void initializeModifications(modifications_t& modifications) {
	Modification acetylation("Acetylation", "Acetylation (+42Da)", 42.010565 , 42.0367);
	Modification amidation("Amidation", "Amidation (-1Da)", -0.984016, -0.9848);
	Modification biotinylation("Biotinylation", "Biotinylation (+226Da)", 226.077598, 226.2954);
	Modification carboxylation("Carboxylation", "Carboxylation (+44Da)", 43.989829, 44.0095);
	Modification deamidation("Deamidation", "Deamidation (+1Da)", 0.984016, 0.9848);
	Modification dioxidation("Dioxidation", "Dioxidation (+32Da)", 31.989829, 31.9988);
	Modification formylation("Formylation", "Formylation (+27Da)", 27.994915, 28.0101);
	Modification hydroxylation("Hydroxylation", "Hydroxylation (+16Da)", 15.994915, 15.9994);
	Modification oxydation("Oxydation", "Oxydation (+16Da)", 15.994915, 15.9994);
	Modification methylation("Methylation", "Methylation (+14Da)", 14.015650, 14.0266);
	Modification beta_methylthiolation("Beta-methylthiolation", "Beta-methylthiolation (+46Da)", 45.987721, 46.0916);
	Modification phosphorylation("Phosphorylation", "Phosphorylation (+80Da)", 79.966331, 79.9799);
	Modification o_sulfonation("O-Sulfonation", "O-Sulfonation (+80Da)", 79.956815, 80.0632);

	Modification a_ions("A-ions", "A-ions (-27Da)", -26.98709, -27.002466); // + H - CO
	Modification b_ions("B-ions", "B-ions (-27Da)", 1.007825, 1.007976);	// + H
	Modification c_ions("C-ions", "C-ions (+18Da)", 18.034374, 18.038627);	// + H + NH + H + H
	Modification x_ions("X-ions", "X-ions (+45Da)", 44.997655, 45.017723);	// + OH + CO
	Modification y_ions("Y-ions", "Y-ions (+19Da)", 19.01839, 19.023233);	// + OH + H + H
	Modification z_ions("Z-ions", "Z-ions (+2Da)", 1.991841, 1.992582);	// + OH - NH

	Modification h2o("+H2O", "+H2O (+18Da)", 18.010565, 18.015257);
	Modification minus_h2o("-H2O", "-H2O (-18Da)", -18.010565, -18.015257);

	// mono and average masses of the following modifications are calculated by 
	// calculating the sum formula and substracting one electron
	Modification h("M+H+", "[M+H]+ (+1Da)", 1.007277, 1.007427);
	Modification na("M+Na+", "[M+Na]+ (+23Da)", 22.989769);
	Modification nh4("M+NH4+", "[M+NH4]+ (+18Da)", 18.033825, 18.038078);
	Modification k2("M+2K+", "[M+2K]+ (+78Da)", 77.926865, 78.196212);
	Modification c3h9n("M-C3H9N+", "[M-C3H9N]+ (-59Da)", -59.0729504, -59.1113697);


	modifications.insert(make_pair(acetylation.short_name, acetylation));
	modifications.insert(make_pair(amidation.short_name, amidation));
	modifications.insert(make_pair(biotinylation.short_name, biotinylation));
	modifications.insert(make_pair(carboxylation.short_name, carboxylation));
	modifications.insert(make_pair(deamidation.short_name, deamidation));
	modifications.insert(make_pair(dioxidation.short_name, dioxidation));
	modifications.insert(make_pair(formylation.short_name, formylation));
	modifications.insert(make_pair(hydroxylation.short_name, hydroxylation));
	modifications.insert(make_pair(oxydation.short_name, oxydation));
	modifications.insert(make_pair(methylation.short_name, methylation));
	modifications.insert(make_pair(beta_methylthiolation.short_name, beta_methylthiolation));
	modifications.insert(make_pair(phosphorylation.short_name, phosphorylation));
	modifications.insert(make_pair(o_sulfonation.short_name, o_sulfonation));
	modifications.insert(make_pair(a_ions.short_name, a_ions));
	modifications.insert(make_pair(b_ions.short_name, b_ions));
	modifications.insert(make_pair(c_ions.short_name, c_ions));
	modifications.insert(make_pair(x_ions.short_name, x_ions));
	modifications.insert(make_pair(y_ions.short_name, y_ions));
	modifications.insert(make_pair(z_ions.short_name, z_ions));
	modifications.insert(make_pair(h2o.short_name, h2o));
	modifications.insert(make_pair(minus_h2o.short_name, minus_h2o));
	modifications.insert(make_pair(h.short_name, h));
	modifications.insert(make_pair(na.short_name,na));
	modifications.insert(make_pair(nh4.short_name, nh4));
	modifications.insert(make_pair(k2.short_name, k2));
	modifications.insert(make_pair(c3h9n.short_name, c3h9n));
}

// TODO: create NamedWeights which will contain names, then there will be no
// necessity to carry alphabet around.
template <typename MassType>
void outputDecompositions(const decompositions_t& decompositions,
						  const Alphabet& alphabet, const Weights& weights,
						  MassType mass, unsigned int maxNumber,
						  bool showParentMass, bool showErrorDelta) {

	cout
		<< "# mass " << mass
		<< " has " << decompositions.size()
		<< " decomposition";
	if (decompositions.size() != 1) {
		cout << "s";
	}
	if (maxNumber < decompositions.size()) {
		cout << " (showing " << maxNumber << ")";
	}
	if (!decompositions.empty()) {
		cout << ":";
	}
	cout << endl;

	// output at most maxNumber decompositions
	unsigned int counter = maxNumber;
	for (decompositions_t::const_iterator pos = decompositions.begin();
			pos != decompositions.end() && counter > 0; ++pos, --counter) {
		bool first = true;
		for (Alphabet::size_type i = 0; i < alphabet.size(); ++i) {
			if ((*pos)[i] > 0) {
				if (!first) {
					cout << " ";
				} else {
					first = false;
				}
				cout << alphabet.getName(i) << (*pos)[i];
			}
		}
		if (showParentMass || showErrorDelta) {
			double parentMass = DecompUtils::getParentMass(weights, *pos);
			if (showParentMass) {
				cout << " (" << parentMass;
				if (showErrorDelta) {
					cout << "; " << showpos << parentMass - mass << noshowpos;
				}
				cout << ')';
			} else {
				cout << " (" << showpos << parentMass - mass << noshowpos << ')';
			}
		}
		cout << '\n';
	}
	cout << endl;
}

// amino-acid modifications

// acetylation 		42.010565 	42.0367 	H(2) C(2) O
// Amidation 		-0.984016 	-0.9848 	H N O(-1) 
// Biotinylation 	226.077598 	226.2954 	H(14) C(10) N(2) O(2) S
// Carboxylation 	43.989829 	44.0095 	C O(2) 
// Deamidation 		0.984016 	0.9848 	H(-1) N(-1) O
// Dioxidation 		31.989829 	31.9988 	O(2)
// Formylation 		27.994915 	28.0101 	C O
// Hydroxylation 	15.994915 	15.9994 	O
// Oxidation 		15.994915 	15.9994 	O
// iTRAQ115 		144.099599 	144.1688 	H(12) C(6) 13C N 15N 18O
// Methylation 		14.015650 	14.0266 	H(2) C
// Beta-methylthiolation 45.987721 	46.0916 	H(2) C S
// Phosphorylation 	79.966331 	79.9799 	H O(3) P
// Pyrr 		-17.026549 	-17.0305 	H(-3) N(-1)
// O-Sulfonation 	79.956815 	80.0632 	O(3) S

// ion masses by ms-ms
// A	Σ +H –CO	S-27	quite common
// B	Σ +H		S+1	common
// C	Σ +H +NH +H +H	S+18	rare
// X	Σ +OH +CO	S+45	rare
// Y	Σ +OH +H +H	S+19	very common
// Z	Σ +OH –NH	S+21	very rare

// fticr modifications
// H+ 1.007285
// Na+ 22.98977
// NH4+ 18.034374
// K2+ 77.92742
// -C3H9N -59.073499

// dna modifications
// H20
