#ifndef __IMS_OPTIONS_H
#define __IMS_OPTIONS_H

#include <vector>
#include <string>
#include <ims/tclap/CmdLine.h>

/**
 * Wrapper for command line options. After instantiation,
 * call parse with main's argc, argv (catch TCLAP::ArgException).
 * Then use the public methods to get the values given on the command line.
 */
class Options {
	public:
		Options();
		void parse(int argc, char** argv);

		unsigned int getMaxNumberDecompositions() const { return maxNumberDecompositions.getValue(); }

		/** Gets the name of the file with the masses. */
		std::string getMassFile() const { return massFile.getValue(); }
		bool hasMassFile() const { return massFile.isSet(); }

		/** Gets the masses to be decomposed or an empty list if the masses are not set as numbers. */
		std::vector<double> getMasses() const;

		std::string getAlphabetFileName() const { return alphabet.getValue(); }

		enum Mode { FINDALL, FINDONE, ISDECOMPOSABLE, GETNUMBER };

		Mode getMode() const { return mode; }

	private:
		TCLAP::CmdLine cmd;
		mutable TCLAP::ValueArg<unsigned int> maxNumberDecompositions; // -x <integer>
		mutable TCLAP::ValueArg<std::string> massFile; // -f <filename>
		mutable TCLAP::MultiArg<double> massNumbers;   // -m <integer>
		mutable TCLAP::ValueArg<std::string> alphabet; // -a <filename>
		mutable TCLAP::SwitchArg argFindAll;
		mutable TCLAP::SwitchArg argFindOne;
		mutable TCLAP::SwitchArg argIsDecomposable;
		mutable TCLAP::SwitchArg argGetNumber;
		std::vector<TCLAP::Arg*> modeargs;
		Mode mode;
};

#endif
