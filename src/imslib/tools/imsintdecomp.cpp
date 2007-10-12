/**
 * @b imsintdecomp is a command line front end to the integere
 * decomposition classes implemented in imslib.
 *
 * @author Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 * @author Marcel Martin <Marcel.Martin@CeBiTec.Uni-Bielefeld.DE>
 */
/*
for TODO, see imsdecomp.cpp
*/

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cassert>
#include <string>

#include <ims/base/exception/exception.h>
#include <ims/base/exception/ioexception.h>
#include <ims/base/parser/massestextparser.h>
#include <ims/decomp/integermassdecomposer.h>
#include <ims/decomp/classicaldpmassdecomposer.h>
#include <ims/decomp/decomputils.h>
#include <ims/utils/print.h>
#include <ims/alphabet.h>
#include <ims/weights.h>

#include "options.h"

#define PROGRAM_NAME "imsintdecomp"
#define PROGRAM_VERSION "1.0"

using namespace ims;
using namespace std;

typedef IntegerMassDecomposer<>::decompositions_type decompositions_t;
typedef IntegerMassDecomposer<>::decomposition_type decomposition_t;
typedef IntegerMassDecomposer<>::decomposition_value_type decomposition_value_t;
typedef IntegerMassDecomposer<>::value_type value_type;


void printHeader(const Alphabet& alphabet, const Weights& weights, Options::Mode mode) {
	cout <<
		"# " PROGRAM_NAME " " PROGRAM_VERSION "\n"
		"# Copyright 2006 Informatics for Mass Spectrometry group\n"
		"#                at Bielefeld University\n"
		"#\n"
		"# http://BiBiServ.TechFak.Uni-Bielefeld.DE/decomp/\n"
		"#\n"
		"# alphabet (character, weight):\n";
	for (Weights::size_type i = 0; i < weights.size(); ++i) {
		cout
			<< "#\t" << alphabet.getName(i) << "\t"
			<< weights[i] << '\n';
	}
	cout << "#\n# Problem to solve: ";
	switch (mode) {
		case Options::GETNUMBER:
			cout << "Get number of decompositions";
			break;
		case Options::FINDALL:
			cout << "Find all decompositions";
			break;
		case Options::FINDONE:
			cout << "Find one decomposition";
			break;
		case Options::ISDECOMPOSABLE:
			cout << "Is decomposable";
			break;
	}
	cout << "\n#\n";
}


/** Prints one decomposition to stdout */
void printDecomposition(const Alphabet& alphabet, const decomposition_t& decomposition)
{
	assert(alphabet.size() == decomposition.size());
	bool first = true;
	for (Alphabet::size_type i = 0; i < alphabet.size(); ++i) {
		if (decomposition[i] > 0) {
			if (!first) {
				cout << " ";
			} else {
				first = false;
			}
			cout << alphabet.getName(i) << decomposition[i];
		}
	}
}


// TODO: creates NamedWeights which will contain names, then there will be no
// necessity to carry alphabet around.
/** Prints out all decompositions to stdout, with some header
 * information in between */
template <typename MassType>
void outputDecompositions(
		const decompositions_t& decompositions,
		const Alphabet& alphabet, MassType mass, unsigned int maxNumber)
{
	cout
		<< "# mass " << mass
		<< " has " << decompositions.size()
	<< " decomposition";
	if (decompositions.size() != 1) {
		cout << "s";
	}
	if (decompositions.size() > maxNumber) {
		cout << " (showing " << maxNumber << ")";
	}
	cout << '\n';

	// output at most maxNumber decompositions
	unsigned int counter = maxNumber;
	for (decompositions_t::const_iterator pos = decompositions.begin();
			pos != decompositions.end() && counter > 0; ++pos, --counter) {
		printDecomposition(alphabet, *pos);
		cout << '\n';
	}
}


int main(int argc, char **argv) {
	Options options;
	try {
		options.parse(argc, argv);
	} catch (TCLAP::ArgException &e) {
		throw ims::Exception("Error while parsing command line: " + e.error() + " for argument " + e.argId());
	}
	Alphabet alphabet;
	try {
		alphabet.load(options.getAlphabetFileName());
	} catch (IOException& ioe) {
		cerr << "can not read alphabet: " << ioe.what() << endl;
		return 1;
	}
	Weights weights(alphabet.getMasses(), 1 /*precision*/);

	// optimize alphabet by dividing by gcd
	weights.divideByGCD();

	// get masses from -m options and/or from a file
	vector<double> masses = options.getMasses();
	if (options.hasMassFile()) {
		MassesTextParser parser;
		try {
			parser.load(options.getMassFile());
		} catch (IOException& ioe) {
			cerr << "can not read masses file: " << ioe.what() << endl;
			return 1;
		}
		vector<double> filemasses = parser.getElements();
		// append to masses
		copy(filemasses.begin(), filemasses.end(), back_inserter(masses));
	}
	if (masses.empty()) {
		cerr << "No input masses given to decompose (use -m or -f option)!\n";
		return 1;
	}

	// maximal number of decompositions to show
	unsigned int maxNumber = options.getMaxNumberDecompositions();

	printHeader(alphabet, weights, options.getMode());

	if (options.getMode() == Options::GETNUMBER) {
		ClassicalDPMassDecomposer<> decomposer(weights);

		// loop through masses
		for (vector<double>::const_iterator it = masses.begin(); it != masses.end(); ++it) {
			value_type mass = static_cast<value_type>(*it);

			decomposition_value_t number = decomposer.getNumberOfDecompositions(mass);
			cout << "# mass " << mass << " has " << number << " decompositions" << endl;
		}
	} else { // if any other tasks defined
		// use decomposer with residues
		IntegerMassDecomposer<> decomposer(weights);

		if (options.getMode() == Options::FINDALL) {
			// loop through masses
			for (vector<double>::const_iterator it = masses.begin(); it != masses.end(); ++it) {
				value_type mass = static_cast<value_type>(*it);
				outputDecompositions(decomposer.getAllDecompositions(mass), alphabet, mass, maxNumber);
				cout << '\n';
			}
		}
		else if (options.getMode() == Options::FINDONE) {
			// loop through masses
			for (vector<double>::const_iterator it = masses.begin(); it != masses.end(); ++it) {
				value_type mass = static_cast<value_type>(*it);
				decomposition_t decomposition = decomposer.getDecomposition(mass);
				if (decomposition.size() == 0) {
					cout << "# mass " << mass << " has 0 decompositions\n\n";
				} else {
					cout << "# mass " << mass << " has at least this decomposition:\n";
					printDecomposition(alphabet, decomposition);
					cout << "\n\n";
				}
			}
		}
		else if (options.getMode() == Options::ISDECOMPOSABLE) {
			// loop through masses
			for (vector<double>::const_iterator it = masses.begin(); it != masses.end(); ++it) {
				value_type mass = static_cast<value_type>(*it);
				if (decomposer.exist(mass)) {
					cout << "# mass " << mass << " has at least one decomposition\n";
				} else {
					cout << "# mass " << mass << " has no decompositions\n";
				}
			}
		}
	}
	cout << "# done\n";

	return 0;
}

