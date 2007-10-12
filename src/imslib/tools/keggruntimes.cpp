/**
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */

#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <iomanip>

#include <ims/utils/math.h>
#include <ims/alphabet.h>
#include <ims/weights.h>
#include <ims/decomp/twomassdecomposer.h>
#include <ims/decomp/twomassdecomposer2.h>
#include <ims/decomp/integermassdecomposer.h>
#include <ims/utils/stopwatch.h>
#include <ims/base/exception/ioexception.h>
#include <ims/base/parser/keggligandcompoundsparser.h>

using namespace std;
using namespace ims;

void initializeCHNOPS(Alphabet&);

int main(int argc, char** argv) {
	typedef double mass_type;
	typedef KeggLigandCompoundsParser parser_type;
	typedef parser_type::sequence_type sequence_type;
	typedef parser_type::container parser_container;
	typedef vector<sequence_type> sequences_type;

	typedef TwoMassDecomposer<> decomposer_type;
	typedef decomposer_type::decompositions_type decompositions_type;
	typedef decomposer_type::value_type integer_mass_type;

	typedef TwoMassDecomposer2<> combined_decomposer_type;

	typedef combined_decomposer_type::decompositions_type combined_decompositions_type;

	
	typedef unsigned long combined_integer_mass_type;
	

	try {

		if (argc == 1) {
			throw IOException("command line lacks a parameter with filename!");
		}

		// parse the input file

		ifstream ifs(argv[1]);
		if (!ifs) {
			string filename = argv[1];
			throw IOException("unable to open input file: " + filename + "!");
		}

		// initializes precision
		mass_type precision = 0.00001;
		if (argc > 2) {
			istringstream precision_string(argv[2]);
			precision_string >> precision;
		}

		// initializes alphabet chnops
		Alphabet chnops;
		initializeCHNOPS(chnops);

		// initializes alphabet mono and average weights
		Weights chnops_monoisotopic_weights(chnops.getMasses(), precision);
		std::cerr << "chnops mono weights: " << std::endl << chnops_monoisotopic_weights;
		Weights chnops_average_weights(chnops.getAverageMasses(), precision);
		std::cerr << "chnops average weights: " << std::endl << chnops_average_weights;

		decomposer_type decomposer(chnops_monoisotopic_weights, chnops_average_weights);

		std::vector<double> a = chnops.getMasses();
		std::vector<double> b = chnops.getAverageMasses();

		combined_decomposer_type combined_decomposer(chnops_monoisotopic_weights, chnops_average_weights);
		Stopwatch stopwatch;

		double timeToDecomposeOneMass, timeToDecomposeTwoMasses, timeToDecomposeMassSpecifically;

		cout << "# mono\taverage\t#decomps\tone\ttwo\ttwo2" << endl;
		parser_type parser;
		const string line_delimits(" \t");
		string line, formula;
		map<string, double> formula_mass_map;
		while (getline(ifs, line)) {
			// searches until mass (we know that mass is the 3d element in the row)
			// moves to the end of the 1st element
			string::size_type start_pos = line.find_first_not_of(line_delimits);

			// skips white lines
			if (start_pos == string::npos) {
				continue;
			}

			start_pos = line.find_first_of(line_delimits);

			// moves to the end of the second element
			start_pos = line.find_first_not_of(line_delimits, start_pos);
			string::size_type end_pos = line.find_first_of(line_delimits, start_pos);

			if (end_pos != string::npos) {
				formula = line.substr(start_pos, end_pos - start_pos);
			} else {
				formula = line.substr(start_pos);
			}
			// checks if this formula was already counted
			if (formula_mass_map.find(formula) != formula_mass_map.end()) {
				continue;
			}

			parser.parse(formula);
			parser_container molecule_elements(parser.getElements());
			double mono_mass = 0.0, average_mass = 0.0;
			for (parser_container::const_iterator it = molecule_elements.begin(); it != molecule_elements.end(); ++it) {
				mono_mass += chnops.getElement(it->first).getMass() * it->second;
				average_mass += chnops.getElement(it->first).getAverageMass() * it->second;
			}
			mono_mass *= parser.getMultiplicator();
			average_mass *= parser.getMultiplicator();
//			cout << formula << ", mono = " << mono_mass << ", average = " << average_mass << '\n';
			formula_mass_map[formula] = mono_mass;

			// measure decomposition of one mass
			stopwatch.start();
			// scales monoisotopic mass
			integer_mass_type integer_mono_mass = static_cast<integer_mass_type>(round(mono_mass / precision));
			// decomposes monoisotopic mass
			decompositions_type decompositions = decomposer.getAllDecompositions(integer_mono_mass);
			timeToDecomposeOneMass = stopwatch.elapsed();


			// TwoMassDecomposer
			stopwatch.start();
			// scales average masse
			integer_mass_type integer_average_mass =
					static_cast<integer_mass_type>(round( average_mass/precision));
			// decomposes monoisotopic and average masses simultaneously
			decompositions_type decompositionsFromTwoMasses =
				decomposer.getAllDecompositions(integer_mono_mass, integer_average_mass);
			timeToDecomposeTwoMasses = stopwatch.elapsed();

			// TwoMassDecomposer2
			stopwatch.start();
			combined_decompositions_type decompositions2 =
				combined_decomposer.getAllDecompositions(integer_mono_mass, integer_average_mass);
			timeToDecomposeMassSpecifically = stopwatch.elapsed();

			assert(decompositionsFromTwoMasses.size() == decompositions2.size());

			cout
				<< mono_mass << '\t'
				<< average_mass << '\t'
				<< decompositions2.size() << '\t'
				<< timeToDecomposeOneMass << '\t'
				<< timeToDecomposeTwoMasses << '\t'
				<< timeToDecomposeMassSpecifically << endl;

// gathers the information for masses density
//			vector<unsigned int>::size_type index = min(static_cast<unsigned int>(mono_mass / one_masses_range), table_size);
//			++masses_distribution_table[index];

// Implementation part where it's checked if the mass given by kegg corresponds to the mass derived from sum formula

//			// moves to the 3d element
//			start_pos = line.find_first_not_of(line_delimits, end_pos);
//			end_pos = line.find_first_of(line_delimits, start_pos);
//			string mass_line;
//			if (end_pos != string::npos) {
//				mass_line = line.substr(start_pos, end_pos - start_pos);
//			} else {
//				mass_line = line.substr(start_pos);
//			}
//
//			double mass;
//			istringstream mass_stream(mass_line);
//			if ((mass_stream >> mass)) {
//				if (fabs(mono_mass - mass) > 1.0) {
//					cout << formula << ", sum formula mass = " << mono_mass << ", input mass = " << mass << '\n';
//				}
//			} else {
//// 				throw invalid_argument("Error while parsing the input. The 3d element in row must be a number!");
//			}

		} // while( getline() )

////////////////////// Implementation part to output masses density///////////////////
//		unsigned int N_compounds = 0;
//		for (vector<unsigned int>::size_type i = 0; i < masses_distribution_table.size()-1; ++i) {
//			cout << i * one_masses_range << '-' << (i+1) * one_masses_range << ": " << masses_distribution_table[i] << '\n';
//			N_compounds += masses_distribution_table[i];
//		}
//		cout << max_mass << "+ : " << masses_distribution_table[masses_distribution_table.size()-1] << '\n';
//		N_compounds += masses_distribution_table[masses_distribution_table.size()-1];
		cout << "#compounds = " << formula_mass_map.size() << '\n';
//
//		for (vector<unsigned int>::size_type i = 0; i < masses_distribution_table.size()-2; ++i) {
//			cout << i * one_masses_range << '-' << (i+1) * one_masses_range << ": "
//				 << static_cast<double>(masses_distribution_table[i]) / N_compounds << '\n';
//		}
//		cout << max_mass << "+ : " << static_cast<double>(masses_distribution_table[masses_distribution_table.size()-2]
//									  + masses_distribution_table[masses_distribution_table.size()-1]) / N_compounds << '\n';

	} catch (Exception& e) {
			cout << "Exception: " << e.message() << endl;
			return 1;
	}

	return 0;
}

void initializeCHNOPS(Alphabet& chnops) {
	typedef IsotopeDistribution distribution_type;
	typedef IsotopeDistribution::peaks_container peaks_container;
	typedef IsotopeDistribution::nominal_mass_type nominal_mass_type;	
	typedef Alphabet::element_type element_type;
	typedef Alphabet::container elements_type;

	distribution_type::SIZE = 10;
	distribution_type::ABUNDANCES_SUM_ERROR = 0.0001;

// Hydrogren
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

// Natrium
//	abundances_container containerNa;
//	containerNa.push_back(1);
//
//	masses_container massesContainerNa;
//	massesContainerNa.push_back(-0.010231);
//
//	nominal_mass_type massNa = 23;
//	IsotopeDistribution distributionNa(containerNa, massesContainerNa, massNa);

// Clorine
//	abundances_container containerCl;
//	containerCl.push_back(0.7577);			// Clorine, Silicon ... Wolfram  - intensities are not precise!
//	containerCl.push_back(0.0);
//	containerCl.push_back(0.2423);
//
//	masses_container massesContainerCl;
//	massesContainerCl.push_back(-0.031147);
//	massesContainerCl.push_back(0.0);
//	massesContainerCl.push_back(-0.034097);
//
//	nominal_mass_type massCl = 35;
//	IsotopeDistribution distributionCl(containerCl, massesContainerCl, massCl);

// Silicon
//	abundances_container containerSi;
//	containerSi.push_back(0.9223);
//	containerSi.push_back(0.0467);
//	containerSi.push_back(0.0310);
//
//	masses_container massesContainerSi;
//	massesContainerSi.push_back(-0.023073);
//	massesContainerSi.push_back(-0.023505);
//	massesContainerSi.push_back(-0.026230);
//
//	nominal_mass_type massSi = 28;
//	IsotopeDistribution distributionSi(containerSi, massesContainerSi, massSi);

// Fluorine
//	abundances_container containerF;
//	containerF.push_back(1);
//
//	masses_container massesContainerF;
//	massesContainerF.push_back(-0.001597);
//
//	nominal_mass_type massF =19;
//	IsotopeDistribution distributionF(containerF, massesContainerF, massF);

// Kalium
//	abundances_container containerK;
//	containerK.push_back(0.9326);
//	containerK.push_back(0.0001);
//	containerK.push_back(0.0673);
//
//	masses_container massesContainerK;
//	massesContainerK.push_back(-0.036293);
//	massesContainerK.push_back(-0.036002);
//	massesContainerK.push_back(-0.038174);
//
//	nominal_mass_type massK = 39;
//	IsotopeDistribution distributionK(containerK, massesContainerK, massK);

// Bromine
//	abundances_container containerBr;
//	containerBr.push_back(0.5069);
//	containerBr.push_back(0.0);
//	containerBr.push_back(0.4931);
//
//	masses_container massesContainerBr;
//	massesContainerBr.push_back(-0.081663);
//	massesContainerBr.push_back(0.0);
//	massesContainerBr.push_back(-0.083709);
//
//	nominal_mass_type massBr = 79;
//	IsotopeDistribution distributionBr(containerBr, massesContainerBr, massBr);

//// Molybdenum
//	abundances_container containerMo;
//	containerMo.push_back(0.1484);
//	containerMo.push_back(0.9250);
//	containerMo.push_back(0.1592);
//	containerMo.push_back(0.1668);
//	containerMo.push_back(0.0955);
//	containerMo.push_back(0.2413);
//
//	masses_container massesContainerMo;
//	massesContainerMo.push_back(-0.093189);
//	massesContainerMo.push_back(-0.093187);
//	massesContainerMo.push_back(-0.094912);
//	massesContainerMo.push_back(-0.094158);
//	massesContainerMo.push_back(-0.095320);
//	massesContainerMo.push_back(-0.093978);
//
//	nominal_mass_type massMo = 92;
//	IsotopeDistribution distributionMo(containerMo, massesContainerMo, massMo);
//	distributionMo.normalize();
//
//// Wolfram
//	abundances_container containerW;
//	containerW.push_back(0.263);
//	containerW.push_back(0.143);
//	containerW.push_back(0.3067);
//	containerW.push_back(0.286);
//
//	masses_container massesContainerW;
//	massesContainerW.push_back(-0.051796);
//	massesContainerW.push_back(-0.049777);
//	massesContainerW.push_back(-0.049069);
//	massesContainerW.push_back(-0.046581);
//
//	nominal_mass_type massW = 182;
//	IsotopeDistribution distributionW(containerW, massesContainerW, massW);
//	distributionW.normalize();
//
//// Gadolinium
//	abundances_container containerGd;
//	containerGd.push_back(0.0218);
//	containerGd.push_back(0.1480);
//	containerGd.push_back(0.2047);
//	containerGd.push_back(0.1565);
//	containerGd.push_back(0.2484);
//
//	masses_container massesContainerGd;
//	massesContainerGd.push_back(-0.079134);
//	massesContainerGd.push_back(-0.077378);
//	massesContainerGd.push_back(-0.077877);
//	massesContainerGd.push_back(-0.07604);
//	massesContainerGd.push_back(-0.075896);
//
//	nominal_mass_type massGd = 154;
//	IsotopeDistribution distributionGd(containerGd, massesContainerGd, massGd);
//	distributionGd.normalize();


	// initialization of chemical elements
	element_type C("C", distributionC);
	element_type H("H", distributionH);
	element_type N("N", distributionN);
	element_type O("O", distributionO);
//	element_type Na("Na", distributionNa);
//	element_type F("F", distributionF);
	element_type P("P", distributionP);
	element_type S("S", distributionS);
//	element_type Si("Si", distributionSi);
//	element_type Cl("Cl", distributionCl);
//	element_type K("K", distributionK);
//	element_type Br("Br", distributionBr);
//	element_type Mo("Mo", distributionMo);
//	element_type Gd("Gd", distributionGd);
//	element_type W("W", distributionW);

	chnops.push_back(C);
	chnops.push_back(H);
	chnops.push_back(N);
	chnops.push_back(O);
	chnops.push_back(P);
	chnops.push_back(S);
//	chnops.push_back(F);
//	chnops.push_back(Na);
//	chnops.push_back(Si);
//	chnops.push_back(Cl);
//	chnops.push_back(K);
//	chnops.push_back(Br);
//	chnops.push_back(Mo);
//	chnops.push_back(Gd);
//	chnops.push_back(W);
}
