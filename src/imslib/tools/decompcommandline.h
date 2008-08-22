#ifndef IMS_DECOMPCOMMANDLINE_H
#define IMS_DECOMPCOMMANDLINE_H

#include <string>
#include <vector>
#include <limits>

#include <ims/tclap/CmdLine.h>
#include <ims/base/exception/exception.h>

/**
 * @brief Represents Command Line Interface for the tool Decomp that 
 * decomposes values over a set of given weights.
 * 
 * @c DecompCommandLine uses Templatized Command Line Parser (TCLAP) 
 * library to use @c TCLAP::CmdLine functionality and different types of 
 * arguments. There are main arguments that are required in the command line,
 * such as file name of the alphabet to be used, masses to be decomposed that
 * are set either explicitly or by giving the file name to be uploaded. 
 * There are other important (but optional) parameters such as error and 
 * precision that define the range of values which will be actually decomposed.
 * Rest of parameters define how output will look like.
 *
 * TODO: Implement the case when the data for the arguments is read out 
 * of the config file.
 *
 * @see TCLAP::CmdLine
 */
class DecompCommandLine {
	private:

		/**
		 * Pointer to the command line object that is a store for all arguments.
		 */
		TCLAP::CmdLine *cmd;

		/**
		 * Pointer to the argument that represents the alphabet file name.
		 */
		TCLAP::ValueArg<std::string> *alphabet;

		/**
		 * Pointer to the argument that represents the precision of decomposition calculation.
		 */
		TCLAP::ValueArg<double> *precision;

		/**
		 * Pointer to the argument that represents the error that masses allow to have.
		 */
		TCLAP::ValueArg<double> *error;

		TCLAP::ValueArg<std::string> *errorUnits;

		TCLAP::ValueArg<unsigned int> *maxNumberDecompositions;

		/**
		 * Pointer to the switch argument that is set when the parent mass is to be output.
		 */
		TCLAP::SwitchArg *showParentMassFlag;

		/**
		 * Pointer to the switch argument that is set when the error delta is to be output.
		 */
		TCLAP::SwitchArg *showErrorDeltaFlag;

		/**
		 * Pointer to the multiple argument that represents masses to be decomposed.
		 */
		TCLAP::MultiArg<double> *massNumbers;

		/**
		 * Pointer to the argument that represents the file with masses to be decomposed.
		 */
		TCLAP::ValueArg<std::string> *massFile;

		/**
		* Modifications to be applied to masses before decomposition.
		*/
		TCLAP::MultiArg<std::string> *modifications;

		TCLAP::ValueArg<std::string> *massMode;

		/**
		 * Initializes arguments.
		 */
		void initializeArguments();

		/**
		 * Adds arguments to the command line.
		 *
		 * @param cmd The command line to which the arguments are added.
		 */
		void addArguments(TCLAP::CmdLine& cmd);

	public:
		/**
		 * Default constructor.
		 */
		DecompCommandLine();

		/**
		 * Default destructor.
		 */
		~DecompCommandLine();

		/**
		 * Parses the command line.
		 * @param argc - Number of arguments.
		 * @param argv - Array of arguments.
		 */
		void parse(int argc, char** argv);

		/**
		 * Gets the name of the file with the alphabet.
		 *
		 * @return The name of the file.
		 */
		std::string getAlphabetFileName() { return alphabet->getValue(); }

		/**
		 * Gets the precision of the alphabet.
		 *
		 * @return The precision of the alphabet.
		 */
		double getPrecision() { return precision->getValue(); }

		/**
		 * Gets the mass error tolerance.
		 *
		 * @return The mass error tolerance.
		 */
		double getError() { return error->getValue(); }

		std::string getErrorUnits() { return errorUnits->getValue(); }

		/**
		 * Gets the masses to be decomposed or an empty list if the masses are not set as numbers.
		 *
		 * @return The masses to be decomposed or an empty list.
		 */
		std::vector<double> getMasses();

		/**
		 * Gets the name of the file with the masses.
		 *
		 * @return The name of the file.
		 */
		std::string getMassesFile() { return massFile->getValue(); }

		unsigned int getMaxNumberDecompositions() 
							{ return maxNumberDecompositions->getValue(); }

		std::string getMassMode() { return massMode->getValue(); }

		/**
		 *
		 */
		bool isShowParentMassSet() { return showParentMassFlag->getValue(); }

		bool isShowErrorDeltaSet() { return showErrorDeltaFlag->getValue(); }

		/**
		 * Returns true, if masses are given in the command line as numbers, false - otherwise.
		 *
		 * @return true, if masses are given as numbers.
		 */
		bool isMassesNumbersSet() { return massNumbers->isSet(); }

		/**
		 * Returns true, if masses are given in the command line as file name from which they can be read,
		 * false - otherwise.
		 *
		 * @return true, if masses are given as file name.
		 */
		bool isMassesFileSet() { return massFile->isSet(); }

		/**
		 * Gets the modifications to be applied to change masses before decomposition.
		 *
		 * @return Modifications to be applied to change masses.
		 */
		std::vector<std::string> getModifications() { return modifications->getValue(); }
};


DecompCommandLine::DecompCommandLine() {
	try {
		// initialize arguments
		initializeArguments();
		// create the command line object with the default usage message and delimiter
		cmd = new TCLAP::CmdLine("Mass decomposition usage:", "=");
		// add arguments to the command line
		addArguments(*cmd);
	} catch (TCLAP::ArgException &e) {
		// TODO: create more specified and nested exceptions
		throw ims::Exception("Error while parsing command line: " + e.error() + " for argument " + e.argId());
	}
}


void DecompCommandLine::initializeArguments() {

	// alphabet flags
	alphabet = new TCLAP::ValueArg<std::string>("a", "alphabet", "File with alphabet masses", true, "dna.masses", "filename");
	precision = new TCLAP::ValueArg<double>("p", "precision", "Precision of decomposition", false, static_cast<double>(0.0), "number");

	// masses flags
	massNumbers = new TCLAP::MultiArg<double>("m", "masses", "List of masses to be decomposed", true, "list");
	massFile = new TCLAP::ValueArg<std::string>("f", "file", "File with masses to be decomposed", true, "input.masses", "string");
	error = new TCLAP::ValueArg<double>("e", "error", "Allowed mass error", false, 0.0, "number");

	std::vector<std::string> allowedErrorUnits;
	allowedErrorUnits.push_back("ppm");
	allowedErrorUnits.push_back("Da");
	errorUnits = new TCLAP::ValueArg<std::string>("u", "errorunits", "Mass Error Units", false, "ppm", allowedErrorUnits);

	// output flags
	showParentMassFlag = new TCLAP::SwitchArg("s", "showmass",
		"Show the actual mass of each decomposition", false);
	showErrorDeltaFlag = new TCLAP::SwitchArg("d", "showdeviation",
		"Show deviation from the query mass for each decomposition", false);
	maxNumberDecompositions = new TCLAP::ValueArg<unsigned int>("x", "max",
		"Maximal number of decompositions", false, std::numeric_limits<unsigned int>::max(), "integer");
	modifications = new TCLAP::MultiArg<std::string>("c", "modifications", "List of modifications to be applied to masses", false, "string");

	std::vector<std::string> allowedMassModes;
	allowedMassModes.push_back("mono");
	allowedMassModes.push_back("average");
	massMode = new TCLAP::ValueArg<std::string>("o", "massmode", "Mass Mode", false, "mono", allowedMassModes);
}


void DecompCommandLine::addArguments(TCLAP::CmdLine& commandLine) {
	commandLine.add(alphabet);
	commandLine.add(precision);
	commandLine.add(error);
	commandLine.add(errorUnits);
	commandLine.add(maxNumberDecompositions);	
	commandLine.add(showParentMassFlag);
	commandLine.add(showErrorDeltaFlag);
	commandLine.xorAdd(*massNumbers, *massFile);
	commandLine.add(modifications);
	commandLine.add(massMode);
}


void DecompCommandLine::parse(int argc, char** argv) {
	// TODO: design it better to allow any number of user defined(config) delimiters
	try {
		// firts, try to parse as it was created with the default delimiter.
		cmd->parse(argc, argv);
	} catch (TCLAP::ArgException &e) {
		// If that breaks, then try to reininialize the command line object with another delimiter.
		delete cmd;
		try {
			cmd = new TCLAP::CmdLine("Mass decomposition tool usage:", ":");
			addArguments(*cmd);
			cmd->parse(argc, argv);
		} catch (TCLAP::ArgException &e) {
			// TODO: create more specified and nested exceptions
			throw ims::Exception("Error while parsing command line: " + e.error() + " for argument " + e.argId());
		}
	}
}


std::vector<double> DecompCommandLine::getMasses() {
	std::vector<double> masses;
	if (massNumbers->isSet()) {
		masses = massNumbers->getValue();
	}
	return masses;
}



DecompCommandLine::~DecompCommandLine() {
	delete cmd;
	delete alphabet;
	delete precision;
	delete error;
	delete errorUnits;
	delete maxNumberDecompositions;	
	delete showParentMassFlag;
	delete showErrorDeltaFlag;
	delete massNumbers;
	delete massFile;
	delete modifications;
	delete massMode;
}

#endif // IMS_DECOMPCOMMANDLINE_H
