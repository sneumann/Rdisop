
// {{{ Includes, Typedefs, prototypes

#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include <sstream>
#include <numeric>
#include <string>
#include <cstring>

//
// IMS Stuff
//
#include <ims/alphabet.h>
#include <ims/weights.h>
#include <ims/isotopedistribution.h>
#include <ims/distributionprobabilityscorer.h>
#include <ims/composedelement.h>
#include <ims/utils/math.h>
#include <ims/decomp/realmassdecomposer.h>
#include <ims/decomp/integermassdecomposer.h>
#include <ims/decomp/decomputils.h>

//
// R Stuff
//
#include <Rcpp.h>
extern "C" {
#include <Rdefines.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
}
using namespace Rcpp;
using namespace ims;
using namespace std;

typedef Alphabet alphabet_t;
typedef IsotopeDistribution distribution_t;
typedef IntegerMassDecomposer<>::decompositions_type decompositions_t;

static void loadAlphabet(SEXP l_alphabet, SEXP v_element_order,
                         alphabet_t& alphabet, std::vector<std::string>& elements_order,
                         int maxisotopes);

template <typename score_type>
SEXP rlistScores(std::multimap<score_type, ComposedElement, std::greater<score_type> > scores, int z);

float getDBE(const ComposedElement& molecule, int z) {
  // {{{ 

  return (1 + static_cast<int>(molecule.getElementAbundance("C"))
	  + static_cast<int>(molecule.getElementAbundance("Si"))
	  - 0.5 * (static_cast<int>(molecule.getElementAbundance("H"))
		   +static_cast<int>(molecule.getElementAbundance("F"))
		   +static_cast<int>(molecule.getElementAbundance("Cl"))
		   +static_cast<int>(molecule.getElementAbundance("Br"))
		   +static_cast<int>(molecule.getElementAbundance("I")))
	  + 0.5 * (static_cast<int>(molecule.getElementAbundance("N"))
		   + static_cast<int>(molecule.getElementAbundance("P"))));
}

// }}}

char getParity(const ComposedElement& molecule, int charge=0) {
  // {{{ 

  //  return (int)(getDBE(molecule, z) * 2) % 2 == 0 ? 'e' : 'o'; 

  bool masseven =  static_cast<int>(molecule.getMass()) % 2 == 0 ? true : false;
  bool nitrogeneven = static_cast<int>(molecule.getElementAbundance("N")) % 2 == 0 ? true : false;
  bool chargeeven = abs(charge) % 2 == 0 ? true : false;
  
  return (masseven ^ nitrogeneven ^ chargeeven) == true ? 'e' : 'o' ;
}

// }}}

bool isValidMyNitrogenRule(const ComposedElement& molecule, int z) {
  // Valid iff the parities of nominal mass, nitrogen count, and charge sum
  // to even -- that is the four-row truth table the original wrote out.
  // The (z % 2 == 1) idiom is preserved verbatim so that negative odd z
  // behaves the same as before (z=-1 reports zodd=false on this platform).
  bool zodd = (z % 2 == 1);
  bool massodd = (static_cast<int>(molecule.getNominalMass()) % 2) == 1;
  bool nitrogenodd = (static_cast<int>(molecule.getElementAbundance("N")) % 2) == 1;
  return !(zodd ^ massodd ^ nitrogenodd);
}

bool isWithinElementRange(const ComposedElement& molecule, const ComposedElement& minElements, const ComposedElement& maxElements) {
// {{{

//iterating over Elements present in minElements
  for (ComposedElement::container::const_iterator it = minElements.getElements().begin();
       it != minElements.getElements().end(); ++it) {

    int mincount = static_cast<int>(minElements.getElementAbundance((it->first).getName()));

    int count = static_cast<int>(molecule.getElementAbundance((it->first).getName()));

      if (count < mincount) {
	return false;
      }

  }
  
  //must iterate over Elements present in molecule and ignore cases of Elements not defined in maxElements
  for (ComposedElement::container::const_iterator it = maxElements.getElements().begin(); 
       it != maxElements.getElements().end(); ++it) {
    
    int maxcount = static_cast<int>(maxElements.getElementAbundance((it->first).getName()));
    
    int count = static_cast<int>(molecule.getElementAbundance((it->first).getName()));
    
    // TODO: Fails e.g. for "C2N0" 
    // JL: I fixed this error by changing "maxcount > 0" to "maxcount >= 0"; however this operator is only available for C++ >20 so it might not compile on old systems
    if (maxcount >= 0 && count > maxcount) {
      return false;
    }
  }

  return true;
}
// }}}

//
// Decomposition of Mass / Isotope Pattern
//

RcppExport SEXP decomposeIsotopes(SEXP v_masses, SEXP v_abundances, SEXP s_error, 
				  SEXP l_alphabet, SEXP v_element_order, 
				  SEXP z, SEXP i_maxisotopes,
				  SEXP s_minElements, SEXP s_maxElements) {
// {{{ 

    typedef DistributionProbabilityScorer scorer_type;
    typedef scorer_type::score_type score_type;
    typedef scorer_type::masses_container masses_container;
    typedef scorer_type::abundances_container abundances_container;
    typedef distribution_t::peaks_container peaks_container;
    typedef distribution_t::mass_type mass_type;
    typedef distribution_t::abundance_type abundance_type;
    typedef distribution_t::nominal_mass_type nominal_mass_type;
    typedef multimap<score_type, ComposedElement, greater<score_type> > scores_container;
    typedef vector<pair<ComposedElement, score_type> > nonnormalized_scores_container;
    typedef decompositions_t::value_type decomposition_type;

    SEXP  rl=R_NilValue; // Use this when there is nothing to be returned.
    try {

	NumericVector masses = NumericVector(v_masses);
	NumericVector abundances = NumericVector(v_abundances);
	double error = *REAL(s_error);

	// converts relative (ppm) in absolute error
	error *= masses(0) * 1.0e-06;

	double precision = 1.0e-05;

	int maxisotopes = Rf_asInteger(i_maxisotopes);
	alphabet_t alphabet;
	vector<string> elements_order;
	loadAlphabet(l_alphabet, v_element_order, alphabet, elements_order, maxisotopes);

	Weights weights(alphabet.getMasses(), precision);
	
	// checks if weights could become smaller, by dividing on gcd.
	weights.divideByGCD();
	
	// initializes decomposer
	RealMassDecomposer decomposer(weights);
	
	// normalizes abundances
	abundance_type abundances_sum = 0.0;
	for (peaks_container::size_type i = 0; i < abundances.size(); ++i) {
		abundances_sum += abundances(i);
	}
	for (peaks_container::size_type i = 0; i < abundances.size(); ++i) {
		abundances(i) /= abundances_sum;
	}

	// fills peaklist masses and abundances, 
	// since we cannot use masses and abundances - instances of RcppVector object - directly
	masses_container peaklist_masses;
	abundances_container peaklist_abundances;
	for (masses_container::size_type mi = 0; mi < masses.size() && mi < abundances.size(); ++mi) {
		peaklist_masses.push_back(masses(mi));
		peaklist_abundances.push_back(abundances(mi));
	}

	// initializes distribution probability scorer
	scorer_type scorer(peaklist_masses, peaklist_abundances);
	
	// initializes storage to store sum formulas and their non-normalized scores
	nonnormalized_scores_container nonnormalized_scores;

	// initializes storage for results: sum formulas and their scores
	scores_container scores;

	///////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////  Start identification pipeline /////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	// gets all possible decompositions for the monoisotopic mass with error allowed
	decompositions_t decompositions = 
		decomposer.getDecompositions(masses(0), error);

	score_type accumulated_score = 0.0;	
	// for every decomposition:
	// - chemical filter is applied
	// - isotopic pattern is calculated
	// - isotopic pattern is matched against input spectrum

	// Initialize minimum/maximum element count "molecules"
	ComposedElement minElements(CHAR(Rf_asChar(s_minElements)), alphabet);
	ComposedElement maxElements(CHAR(Rf_asChar(s_maxElements)), alphabet);

	for (decompositions_t::iterator decomps_it = decompositions.begin(); 
		decomps_it != decompositions.end(); ++decomps_it) {

		// creates a candidate molecule out of elemental composition and a set of elements
		ComposedElement candidate_molecule(*decomps_it, alphabet);

		// Check minimum/maximum element counts
		if (!isWithinElementRange(candidate_molecule, minElements, maxElements)) {
			continue;
		} 


		// checks on chemical filter
// 		if (!isValidMyNitrogenRule(candidate_molecule, z)) {
// 			continue;
// 		} 


		// updates molecules isotope distribution (since its not calculated upon creation: 
		// it would be time consuming before applying chemical filter)
		candidate_molecule.updateIsotopeDistribution();
		// updates molecules sequence in a order of elements(atoms) one would like it
		// to appear
		candidate_molecule.updateSequence(&elements_order);

		// gets a theoretical isotope distribution of the candidate molecule
		IsotopeDistribution candidate_molecule_distribution = 
				candidate_molecule.getIsotopeDistribution();
		
		// gets a sequence of the candidate molecule
		string candidate_molecule_sequence = 
				candidate_molecule.getSequence();

		// extracts masses and abundances from isotope distribution of the candidate molecule
		masses_container candidate_masses = candidate_molecule_distribution.getMasses();
		abundances_container candidate_abundances = candidate_molecule_distribution.getAbundances();

		// normalizes candidate abundances if the size of the measured peaklist is less than 
		// the size of theoretical isotope distribution. This is always the case since our
		// theoretical distributions are limited to by default 10 peaks and measured peaklists contain
		// less than 10 peaks

		distribution_t::size_type size = min(peaklist_abundances.size(), candidate_abundances.size());
		
		if (size < candidate_abundances.size()) {
			// normalizes the isotope distribution abundances with respect to the number of elements in peaklist
			abundance_type sum = accumulate(candidate_abundances.begin(), 
							candidate_abundances.begin() + size, 
							0.0);
			if (fabs(sum - 1) > IsotopeDistribution::ABUNDANCES_SUM_ERROR) {
				abundance_type scale = 1/sum;
				transform(candidate_abundances.begin(),			// begin of source range
					candidate_abundances.begin() + size,		// end of source range
					candidate_abundances.begin(), 			// destination
					[scale](abundance_type a) { return a * scale; }); // operation (*scale)
			}
				
		}

		// calculates a score
		score_type score = scorer.score(candidate_masses, candidate_abundances);

		// stores the sequence with non-normalized score
		nonnormalized_scores.push_back(make_pair(candidate_molecule, score));

		// accumulates scores
		accumulated_score += score;

	}

	for (nonnormalized_scores_container::const_iterator it = nonnormalized_scores.begin(); it != nonnormalized_scores.end(); ++it) {
		score_type normalized_score = it->second;
		if (accumulated_score > 0.0) {
			normalized_score /= accumulated_score;
		}
		// stores the sequence with the score
		scores.insert(make_pair(normalized_score, it->first));
	}

	// Now output to R ...
	if (scores.size() >0 ) {
	  rl = rlistScores(scores, Rf_asInteger(z));
	}
    } catch(std::exception& ex) {
      //exceptionMesg = copyMessageToR(ex.what());
      //error_return(exceptionMesg); 
      forward_exception_to_r(ex);
    } catch(...) {
      //exceptionMesg = copyMessageToR("unknown reason");
      //error_return(exceptionMesg); 
      ::Rf_error("%s", "c++ exception (unknown reason)");
    }
        
    return rl;

}

// }}}

RcppExport SEXP calculateScore(SEXP v_predictMasses, SEXP v_predictAbundances, SEXP v_measuredMasses, SEXP v_meausuredAbundances) {
//  {{{
	typedef DistributionProbabilityScorer scorer_type;
    	typedef scorer_type::score_type score_type;
    	typedef scorer_type::masses_container masses_container;
    	typedef scorer_type::abundances_container abundances_container;
    	typedef distribution_t::peaks_container peaks_container;
    	typedef distribution_t::mass_type mass_type;
    	typedef distribution_t::abundance_type abundance_type;



	NumericVector masses = NumericVector(v_predictMasses);
	NumericVector abundances = NumericVector(v_predictAbundances);

	// fills peaklist masses and abundances, 
	// since we cannot use masses and abundances - instances of RcppVector object - directly

	masses_container peaklist_masses;
	abundances_container peaklist_abundances;
	for (masses_container::size_type mi = 0; mi < masses.size() && mi < abundances.size(); ++mi)
	{
		peaklist_masses.push_back(masses(mi));
		peaklist_abundances.push_back(abundances(mi));
	}

	// initializes distribution probability scorer
	scorer_type scorer(peaklist_masses, peaklist_abundances);

	masses = NumericVector(v_measuredMasses);
	abundances = NumericVector(v_meausuredAbundances);
	//cout << "Mess:" << masses<<"\n";
	masses_container mess_masses;
	abundances_container mess_abundances;

	// normalizes abundances
	abundance_type abundances_sum = 0.0;
	for (peaks_container::size_type i = 0; i < abundances.size(); ++i) {
		abundances_sum += abundances(i);
	}
	for (peaks_container::size_type i = 0; i < abundances.size(); ++i) {
		abundances(i) /= abundances_sum;
	}	

	for (masses_container::size_type mi = 0; mi < masses.size() && mi < abundances.size(); ++mi)
	{
		mess_masses.push_back(masses(mi));
		mess_abundances.push_back(abundances(mi));
	}

	score_type score=scorer.score(mess_masses, mess_abundances);
	//List output;
	return(List::create(  _[""]  = score));
}
// }}}


RcppExport SEXP getMolecule(SEXP s_formula, SEXP l_alphabet, 
			    SEXP v_element_order, SEXP z, SEXP i_maxisotopes) {
// {{{ 

  SEXP  rl=R_NilValue; // Use this when there is nothing to be returned.

   if( (s_formula==NULL) || !Rf_isString(s_formula) || Rf_length(s_formula) != 1)
         Rf_error("%s","formula is not a single string");

  typedef DistributionProbabilityScorer scorer_type;
  typedef scorer_type::score_type score_type;
  typedef multimap<score_type, ComposedElement, greater<score_type> > scores_container;

  int maxisotopes = Rf_asInteger(i_maxisotopes);
  alphabet_t alphabet;
  vector<string> elements_order;
  loadAlphabet(l_alphabet, v_element_order, alphabet, elements_order, maxisotopes);

  try {
    double precision = 1.0e-05;
    Weights weights(alphabet.getMasses(), precision);

    // initializes storage for scores
    scores_container scores;
    
    ComposedElement molecule( CHAR(Rf_asChar(s_formula)), alphabet);
    
    molecule.updateSequence(&elements_order);
    molecule.updateIsotopeDistribution();
    
    scores.insert(make_pair(1.0, molecule));
    rl = rlistScores(scores, Rf_asInteger(z));
  } catch(std::exception& ex) {
    forward_exception_to_r(ex);
  } catch(...) {
    ::Rf_error("%s","c++ exception (unknown reason)");
  }

  return rl;
}

// }}}

RcppExport SEXP addMolecules(SEXP s_formula1, SEXP s_formula2, SEXP l_alphabet, 
			     SEXP v_element_order, SEXP i_maxisotopes) {
  // {{{ 

  SEXP  rl=R_NilValue; // Use this when there is nothing to be returned.

   if( (s_formula1==NULL) || s_formula2==NULL 
       || !Rf_isString(s_formula1) || !Rf_isString(s_formula2) 
       || Rf_length(s_formula2) != 1) {
         Rf_error("%s","formula is not a single string");
   }

  typedef DistributionProbabilityScorer scorer_type;
  typedef scorer_type::score_type score_type;
  typedef multimap<score_type, ComposedElement, greater<score_type> > scores_container;

  int maxisotopes = Rf_asInteger(i_maxisotopes);
  alphabet_t alphabet;
  vector<string> elements_order;
  loadAlphabet(l_alphabet, v_element_order, alphabet, elements_order, maxisotopes);

  double precision = 1.0e-05;
  Weights weights(alphabet.getMasses(), precision);

  scores_container scores;

  ComposedElement molecule( CHAR(Rf_asChar(s_formula1)), alphabet);
  ComposedElement molecule2( CHAR(Rf_asChar(s_formula2)), alphabet);

  molecule += molecule2;

  molecule.updateSequence(&elements_order);
  molecule.updateIsotopeDistribution();

  scores.insert(make_pair(1.0, molecule));
  rl = rlistScores(scores, 0);
  return rl;
}

// }}}

RcppExport SEXP subMolecules(SEXP s_formula1, SEXP s_formula2, SEXP l_alphabet, SEXP v_element_order, SEXP i_maxisotopes) {
  // {{{ 

  SEXP  rl=R_NilValue; // Use this when there is nothing to be returned.

   if( (s_formula1==NULL) || s_formula2==NULL 
       || !Rf_isString(s_formula1) || !Rf_isString(s_formula2) 
       || Rf_length(s_formula2) != 1) {
         Rf_error("formula is not a single string");
   }

  typedef DistributionProbabilityScorer scorer_type;
  typedef scorer_type::score_type score_type;
  typedef multimap<score_type, ComposedElement, greater<score_type> > scores_container;

  int maxisotopes = Rf_asInteger(i_maxisotopes);
  alphabet_t alphabet;
  vector<string> elements_order;
  loadAlphabet(l_alphabet, v_element_order, alphabet, elements_order, maxisotopes);

  double precision = 1.0e-05;
  Weights weights(alphabet.getMasses(), precision);

  scores_container scores;

  ComposedElement molecule( CHAR(Rf_asChar(s_formula1)), alphabet);
  ComposedElement molecule2( CHAR(Rf_asChar(s_formula2)), alphabet);

  molecule -= molecule2;

  molecule.updateSequence(&elements_order);
  molecule.updateIsotopeDistribution();

  scores.insert(make_pair(1.0, molecule));
  rl = rlistScores(scores, 0);
  return rl;
}

// }}}

template <typename score_type>
SEXP  rlistScores(multimap<score_type, ComposedElement, greater<score_type> > scores, int z) {
  // {{{ 

    typedef DistributionProbabilityScorer scorer_type;
    typedef multimap<score_type, ComposedElement, greater<score_type> > scores_container;

	// Build result set to be returned as a list to R.
	vector<string> formula(scores.size());
	NumericVector score(scores.size());
	NumericVector exactmass(scores.size());
	IntegerVector charge(scores.size());

	// Chemical rules
	vector<string> parity(scores.size());
	vector<string> valid(scores.size());
	NumericVector DBE(scores.size());

	SEXP isotopes = PROTECT(Rf_allocVector(VECSXP, scores.size()));

	unsigned int i = 0;

	vector<string> colNames(2);
	colNames[0] = "mass";
	colNames[1] = "intensity";

	// outputs molecules & their scores.
	for (typename scores_container::const_iterator it = scores.begin(); 
				it != scores.end(); ++it) {
		score(i) = it->first;
      		formula[i] = it->second.getSequence();
      		exactmass(i) = it->second.getMass();
      		charge(i) = z;

		// Chemical rules 
		parity[i] = getParity(it->second, z);
		valid[i] = isValidMyNitrogenRule(it->second, z) ? "Valid" : "Invalid";

		DBE(i) = getDBE(it->second, z);

		IsotopeDistribution isodist = it->second.getIsotopeDistribution();
		
		IsotopeDistribution::masses_container masses = isodist.getMasses();
		IsotopeDistribution::abundances_container intensities = isodist.getAbundances();

		int ny = masses.size();
		SEXP tmp_isotopes = PROTECT(Rf_allocMatrix(REALSXP, 2, ny));

		for(int j = 0; j < ny; j++) {
		    REAL(tmp_isotopes)[0 + 2*j] = masses[j];
		    REAL(tmp_isotopes)[1 + 2*j] = intensities[j];
		}
		SET_VECTOR_ELT(isotopes, i, tmp_isotopes);

		UNPROTECT(1);

		i++;
	}


	UNPROTECT(1); // SEXP isotopes

	return(List::create(  _["formula"]  = formula,
                       _["score"]  = score,
                       _["exactmass"]  = exactmass,
                       _["charge"]  = charge,
                       _["parity"]  = parity,
                       _["valid"]  = valid,
                       _["DBE"]  = DBE,
                       _["isotopes"]  = isotopes));

	// }}}
}

//
// Build the alphabet from the R-supplied elements list and remember the
// caller's preferred element ordering for formula serialization.
//
static void loadAlphabet(SEXP l_alphabet, SEXP v_element_order,
                         alphabet_t& alphabet, std::vector<std::string>& elements_order,
                         int maxisotopes) {
  typedef distribution_t::peaks_container peaks_container;
  typedef distribution_t::nominal_mass_type nominal_mass_type;
  typedef alphabet_t::element_type element_type;

  if (l_alphabet == NULL || Rf_length(l_alphabet) < 1) {
    Rf_error("%s", "elements list must be non-empty");
  }

  distribution_t::SIZE = maxisotopes;
  distribution_t::ABUNDANCES_SUM_ERROR = 0.0001;

  Rcpp::List elementList(l_alphabet);
  for (R_xlen_t i = 0; i < elementList.size(); ++i) {
    Rcpp::List el = elementList[i];
    std::string symbol = Rcpp::as<std::string>(el["name"]);
    nominal_mass_type nominalmass =
      static_cast<nominal_mass_type>(Rcpp::as<double>(el["mass"]));

    Rcpp::List isotope = el["isotope"];
    Rcpp::NumericVector mass = isotope["mass"];
    Rcpp::NumericVector abundance = isotope["abundance"];
    R_xlen_t numisotopes = mass.size();

    peaks_container peaks;
    peaks.reserve(numisotopes);
    for (R_xlen_t j = 0; j < numisotopes; ++j) {
      peaks.push_back(peaks_container::value_type(mass[j], abundance[j]));
    }
    distribution_t distribution(peaks, nominalmass);

    alphabet.push_back(element_type(symbol, distribution));
  }

  R_xlen_t order_length = Rf_length(v_element_order);
  elements_order.reserve(order_length);
  for (R_xlen_t i = 0; i < order_length; ++i) {
    elements_order.push_back(std::string(CHAR(STRING_ELT(v_element_order, i))));
  }
}

extern "C" {

  void R_init_disop(DllInfo *info)
  {
    /* Register routines, allocate resources.
     * We call most functions with .Call 
     */
    R_CallMethodDef callMethods[]  = {
      {"getMolecule", (void* (*)())&getMolecule, 4},
      {"addMolecules", (void* (*)())&addMolecules, 4},
      {"subMolecules", (void* (*)())&subMolecules, 4},
      {"decomposeIsotopes", (void* (*)())&decomposeIsotopes, 9},
      {"calculateScore", (void* (*)())&calculateScore, 7},
      {NULL, NULL, 0}
    };
    
    R_registerRoutines(info, NULL, callMethods, NULL, NULL);
    
    /* included by jan.lisec@bam.de to account for RCHECK Note
     */
    R_useDynamicSymbols(info, FALSE);
  }
  
  void R_unload_disop(DllInfo *info)
  {
    /* Release resources. */
  }

}
