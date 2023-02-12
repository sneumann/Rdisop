/** @author Tobias Marschall, tmarscha@cebitec.uni-bielefeld.de
  * Based on cvs module fragmenting
  */

#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <cstdlib>

#include <cstring>
#include <config.h>

#include <ims/alphabet.h>
#include <ims/pmffragmenter.h>
#include <ims/massintensitypeak.h>
#include <ims/peaklist.h>
#include <ims/logger.h>
#include <ims/functors/alphabetgetmass.h>
#include <ims/modifier/multimodifier.h>
#include <ims/modifier/sortmodifier.h>
#include <ims/modifier/unificationmodifier.h>
#include "../src/ims/characteralphabet.h"

// TODO: make use of logger

using namespace std;
using namespace ims;

typedef double mass_type;
typedef PMFFragmenter<mass_type,mass_type> fragmenter_type;
typedef fragmenter_type::peak_type peak_type;
typedef fragmenter_type::peaklist_type peaklist_type;
// for letter frequencies
typedef map<char, long> map_type;

// command line arguments
string alphabet_filename = "";
string database_filename = "";
// option -l or -v
ELogLevel loglevel = Messages;
// option -b
string bad_chars = "BZX-*?";
// option  -c
string cleavage_chars = "KR";
// option  -p
string prohibition_chars = "P";
// option -w
bool with_cleavage_char = true;
// option  -o
string output_filename = "/dev/stdout";
// option  -s
string sequence_string_template = "%i\\t%s\\t%l\\t%n\\t%m";
// option  -f
string fragment_string_template = "\\t%m\\t%l";
// option  -S
string statistic_filename = "";

/** Prints usage information to stdout */
void usage() {
	cout << "Usage: imsfrag [options] <alphabet> <sequence_db>" << endl
		<< "  <alphabet>: file containing alphabet" <<  endl
		<< "  <sequence_db>: FASTA file containing database of sequences" << endl
		<< endl
		<< "General options:" << endl
		<< "  -h show this help, then exit" << endl
		<< "  -V print version number, then exit" << endl
		<< "  -l <ll> set log level [1..4] (default=2)" << endl
		<< "  -v be verbose, equivalent to -l 3" << endl
		<< "  -c <cleavage_chars> (default=\"" << cleavage_chars << "\")" << endl
		<< "  -p <prohibition_chars> (default=\"" << prohibition_chars << "\")" << endl
		<< "  -w don't count cleavage character to fragment" << endl
		<< "  -b <bad_chars> sequences containing one of those characters will" << endl
		<< "     be ignored (default=\"" << bad_chars << "\")" << endl
		<< "Output control:" << endl
		<< "  -o <output_filename> redirect output (default=\"" << output_filename << "\")" << endl
		<< "  -s <sequence_string_template> template for a line describing a sequence," << endl
		<< "     set to \"\" to supress output of these lines (default=\"" << sequence_string_template << "\")" << endl
		<< "     Valid wildcards:" << endl
		<< "       %s sequence" << endl
		<< "       %i sequence id" << endl
		<< "       %l length of sequence" << endl
		<< "       %n number of fragments" << endl
		<< "       %m total mass" << endl
		<< "  -f <fragment_string_template> template for a line describing a fragment," << endl
		<< "     set to \"\" to supress output of these lines (default=\"" << fragment_string_template << "\")" << endl
		<< "     Valid wildcards:" << endl
		<< "       %m mass of fragment" << endl
		<< "       %l length of fragment" << endl
		<< "  -S <statistic_filename> write frequency of occurrence of alphabet characters" << endl
		<< "     to specified file (default=\"" << statistic_filename << "\")" << endl;
}


/** Prints version information (from config.h) to stdout */
void version() {
	cout << "imsfrag, imslib version " VERSION << endl;
}


/** Handles parameters given on command-line by modifying global variables. */
bool handleargs(int argc, char **argv) {
	// TODO: maybe use tclap
	extern char *optarg;
	extern int optopt;
	//currently unused: extern int optind;
	const char * options = "hVl:vc:p:b:o:s:f:S:w";
	int c,i;

	if (argc==2) {
		if (strcmp(argv[1],"-h") == 0) {usage(); exit(0);}
		if (strcmp(argv[1],"--help") == 0) {usage(); exit(0);}
		if (strcmp(argv[1],"?") == 0) {usage(); exit(0);}
		if (strcmp(argv[1],"-V") == 0) {version(); exit(0);}
	}
	
	if (argc<3) {
		cerr << "Error: Too few arguments" << endl;
		usage(); 
		exit(-1);
	}
	alphabet_filename = argv[argc-2];
	database_filename = argv[argc-1];
	argc-=2;
	
	while ( (c = getopt(argc, argv, options)) != -1 ) {
		switch (c) {
		case 'V':
			version();
			exit(0);
		case 'v':
			loglevel = Details;
			break;
		case 'l':
			i = atoi(optarg);
			switch (i) {
				case 1:
					loglevel = Silent;
					break;
				case 2:
					loglevel = Messages;
					break;
				case 3:
					loglevel = Details;
					break;
				case 4:
					loglevel = Everything;
					break;
				default:
					cerr << "Invalid log level: \"" << optarg << "\"" << endl;
					exit(-1);
			}
			break;
		case 'b':
			bad_chars=optarg;
			break;
		case 'c':
			cleavage_chars=optarg;
			break;
		case 'p':
			prohibition_chars=optarg;
			break;
		case 'o':
			output_filename=optarg;
			break;
		case 's':
			sequence_string_template=optarg;
			break;
		case 'f':
			fragment_string_template=optarg;
			break;
		case 'S':
			statistic_filename=optarg;
			break;
		case 'w':
			with_cleavage_char=false;
			break;
		case 'h':
			usage();
			exit(0);
		case '?':
			cerr << "Option -" << char(optopt) << " not recognized.\n";
			usage();
			exit(-1);
		}
	}
	
	return true;
}

inline bool handle_sequence(const string& sequence, const string& id, const CharacterAlphabet& alphabet,
	fragmenter_type& fragmenter, map_type& letter_frequencies, ostream& os)
{
	map_type tmp_letter_frequencies;

	// filter out bad characters and count letter frequencies
	// but only of those sequences, which are clean
	for (size_t i=0; i<sequence.length(); ++i) {
		if (bad_chars.find(sequence[i])==string::npos){
			//count it
			(tmp_letter_frequencies[sequence[i]])++;
		} else {
			// contains bad_character --> invalid sequence
			return false;
		}
	}
	// so we have a valid sequence and can store the letter frequencies
	map_type::const_iterator it = tmp_letter_frequencies.begin();
	for ( ; it != tmp_letter_frequencies.end(); ++it) {
		letter_frequencies[it->first]+=it->second;
	}

	// now perform fragmentation
	peaklist_type peaklist;
	
	fragmenter.predictSpectrum(&peaklist, sequence);

	// print output, starting with output for the sequence
	if (sequence_string_template!="") {
		string s = sequence_string_template;
		size_t n;
		while ((n=s.find("%s"))!=string::npos) {
			s.replace(n,2,sequence);
		}
		while ((n=s.find("%i"))!=string::npos) {
			s.replace(n,2,id);
		}
		while ((n=s.find("%l"))!=string::npos) {
			ostringstream oss;
			oss << sequence.size();
			s.replace(n,2,oss.str());
		}
		while ((n=s.find("%n"))!=string::npos) {
			ostringstream oss;
			oss << peaklist.size();
			s.replace(n,2,oss.str());
		}
		while ((n=s.find("%m"))!=string::npos) {
			Alphabet::mass_type m = 0.0;
			for (size_t i=0; i<sequence.size(); ++i) {
				m+=alphabet.getMass(sequence[i]);
			}
			ostringstream oss;
			oss << m;
			s.replace(n,2,oss.str());
		}
		// support tabs ...
		while ((n=s.find("\\t"))!=string::npos) {
			s.replace(n,2,"\t");
		}
		// ... and newlines
		while ((n=s.find("\\n"))!=string::npos) {
			s.replace(n,2,"\n");
		}
		// and finally write line to stream
		os << s << endl;
	}
	// now write output for every fragment
	if (fragment_string_template!="") {
		peaklist_type::iterator it_peaklist = peaklist.begin();
		for ( ; it_peaklist!=peaklist.end(); ++it_peaklist) {
			string s = fragment_string_template;
			size_t n;
			while ((n=s.find("%m"))!=string::npos) {
				ostringstream oss;
				oss << it_peaklist->getMass();
				s.replace(n,2,oss.str());
			}
			while ((n=s.find("%l"))!=string::npos) {
				ostringstream oss;
				// and it seems like the second one of this pair is the length
				oss << (it_peaklist->getLength());
				s.replace(n,2,oss.str());
			}
			// support tabs ...
			while ((n=s.find("\\t"))!=string::npos) {
				s.replace(n,2,"\t");
			}
			// ... and newlines
			while ((n=s.find("\\n"))!=string::npos) {
				s.replace(n,2,"\n");
			}
			// and finally write line to stream
			os << s << endl;
		}
	}

	return true;
}


int main(int argc, char** argv)
{
	// prints more information if the program was terminated due to an
	// exception.
	// see http://groups.yahoo.com/group/open-source-programmer/message/91
	#ifdef __GNUC__
	std::set_terminate (__gnu_cxx::__verbose_terminate_handler);
	#endif
	// parse command line arguments
	handleargs(argc, argv);

	//load an alphabet from the mass_file
	Alphabet alphabet;
	try {
		alphabet.load(alphabet_filename);
	}
	catch (IOException) {
		cerr << "Error loading alphabet \""<< alphabet_filename << "\"" << endl;
		exit(1);
	}

	cout << "Alphabet succesfully loaded:" << endl;
	cout << alphabet << endl;
	
	// copy over masses into a alphabet with fast access by character
	CharacterAlphabet fast_alphabet(alphabet);

	//now build some kind of fragmentizer
	cout << "Start building Fragmenter..." << endl;
	unique_ptr<Modifier<peaklist_type> > sort_modifier(new SortModifier<peaklist_type>);
	unique_ptr<Modifier<peaklist_type> > unification_modifier(new UnificationModifier<peaklist_type>);
	unique_ptr<MultiModifier<peaklist_type> > multi_modifier(new MultiModifier<peaklist_type>);
	multi_modifier->addModifier(sort_modifier);
	multi_modifier->addModifier(unification_modifier);
	fragmenter_type fragmenter(alphabet, cleavage_chars, prohibition_chars, with_cleavage_char);
	fragmenter.setModifier(unique_ptr<Modifier<peaklist_type> >(multi_modifier));
	cout << "...done\n" << endl;


	//now that we hopefully have some fragmentizer, we should be able to start parsing the fasta file
	//first open the stream
	ifstream fasta_stream(database_filename.c_str());
	if (!fasta_stream) {
		cerr << "Error: file \"" << database_filename << "\" could not be opened" << endl;
		exit(1);
	}

	//some vars
	//this maxlength thingie determines, how long such a sequence should be. 0 means unlimited
	const unsigned int maxlength = 0;
	string line;
	string currentString("");
	string currentID("");

	//open file for output
	ofstream ofs(output_filename.c_str());
	if (!ofs) {
		cerr << "Error: Could not open output file \""<< output_filename << "\"" << endl;
		exit(1);
	}

	//init the statistics vars
	long discarded_sequences = 0;
	long kept_sequences = 0;
	map_type letter_frequencies;

	//begin the looping
	while (getline(fasta_stream, line)) {
		if (line[0] == ' ') {
			continue; //FIXME this is not fasta file format
		}
		if(line[0] == '>') {          // check if end of sequence (rather beginng of new)
            // close current sequence (if there is one)
			if((maxlength==0 || currentString.length()<=maxlength) && currentString.length()>0) {
				//here comes the sequence handling
				//FIXME we need string toUpper, but we dont have it
				if (handle_sequence (currentString, currentID, fast_alphabet, fragmenter, letter_frequencies, ofs))
					++kept_sequences;
				else
					++discarded_sequences;
				currentString ="";
			}
            // begin new id sequence
			int k=1;
			while(isspace(line[k++]));
			currentID="";
			for(unsigned int i=k-1;i<line.length() && !(isspace(line[i]));i++) {
				currentID += line[i];
			}
		}
		else {
            // read sequence line
			for(unsigned int i=0;i<line.length();i++) {
				if(!isspace(line[i])) {
					currentString += line[i];
				}
			}
		}
	}
    // handle last sequence
	if((maxlength==0 || currentString.length()<=maxlength) && currentString.length()>0) {
		//here we have to do some handling for the last sequence
		//FIXME we need string toUpper, but we dont have it
		if (handle_sequence (currentString, currentID, fast_alphabet, fragmenter, letter_frequencies, ofs))
			++kept_sequences;
		else
			++discarded_sequences;

	}
	
	//logging
	cout << "\nAll sequences processed\n" << endl;

	//close streams
	fasta_stream.close();
	ofs.close();

	if (statistic_filename!="") {
	//do some statistics for the letter frequencies and sequence counts
		ofstream statistics(statistic_filename.c_str());
		if (!statistics) {
			cerr << "Error opening output file for statistics \""<< statistic_filename << "\"" << endl;
			exit(1);
		}
		cout << "Writing statistics to \"" << statistic_filename << "\"" << endl;
		statistics << "# Kept " << kept_sequences << ", discarded " << discarded_sequences << ", from a total of "
				<< kept_sequences + discarded_sequences << " sequences." << endl;
		statistics << "# Letter frequencies in valid sequences:" << endl;
		map_type::const_iterator it = letter_frequencies.begin();
		for ( ; it != letter_frequencies.end(); ++it) {
			statistics << it->first << "\t" << it->second << endl;
		}
		statistics.close();
	}

}
