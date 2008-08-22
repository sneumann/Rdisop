#include <limits>
#include <stdlib.h>
#include "options.h"

Options::Options() :
	cmd(TCLAP::CmdLine("Mass decomposition usage:")), // TODO used to have "=" as second parameter
	maxNumberDecompositions(
		TCLAP::ValueArg<unsigned int>("x", "max",
		"Maximal number of decompositions (only used if 'find all' is selected", std::numeric_limits<unsigned int>::max(), false, "integer")
	),
	massFile(TCLAP::ValueArg<std::string>("f", "file", "File with masses to be decomposed", true, "input.masses", "string")),
	massNumbers(TCLAP::MultiArg<double>("m", "masses", "List of masses to be decomposed", true, "list")),
	alphabet(TCLAP::ValueArg<std::string>("a", "alphabet", "File with alphabet masses", true, "dna.masses", "filename")),
	argFindAll(TCLAP::SwitchArg("l", "all", "Find all decompositions", true)),
	argFindOne(TCLAP::SwitchArg("1", "one", "Find one decomposition", false)),
	argIsDecomposable(TCLAP::SwitchArg("i", "is", "Check if mass is decomposable", false)),
	argGetNumber(TCLAP::SwitchArg("n", "number", "Get number of decompositions", false)),
	mode(FINDALL)
{
	// add arguments to the command line
	cmd.add(alphabet);
	cmd.add(maxNumberDecompositions);

	cmd.xorAdd(massNumbers, massFile);
	modeargs.push_back(&argFindAll);
	modeargs.push_back(&argFindOne);
	modeargs.push_back(&argIsDecomposable);
	modeargs.push_back(&argGetNumber);
	cmd.xorAdd(modeargs);
}


/**
 * may throw TCLAP::ArgException
 */
void Options::parse(int argc, char** argv) {
	cmd.parse(argc, argv);
	if (argFindAll.isSet()) mode = FINDALL;
	else if (argFindOne.isSet()) mode = FINDONE;
	else if (argIsDecomposable.isSet()) mode = ISDECOMPOSABLE;
	else mode = GETNUMBER;
}


std::vector<double> Options::getMasses() const {
	std::vector<double> masses;
	if (massNumbers.isSet()) {
		masses = massNumbers.getValue();
	}
	return masses;
}
