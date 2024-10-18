#' @name decomposeIsotopes
#' @title Mass Decomposition of Isotope Patterns
#' @aliases decomposeMass
#' @aliases isotopeScore
#'
#' @description  Calculate the elementary compositions from an exact Mass or
#'     Isotope Pattern, obtained e.g. by FTICR or TOF mass spectrometers.
#'
#' @param masses A vector of masses (or m/z values) of an isotope cluster.
#' @param intensities Absolute or relative intensities of the \code{masses} peaks.
#' @param ppm Allowed deviation of hypotheses from given mass.
#' @param mzabs Absolute deviation in Dalton (mzabs and ppm will be added).
#' @param elements List of allowed chemical elements, defaults to CHNOPS.
#' @param filter NYI, will be a selection of DU, DBE and Nitrogen rules.
#' @param z Charge z of m/z peaks for calculation of real mass, keep z=0 for auto-detection.
#' @param maxisotopes Maximum number of isotopes shown in the resulting molecules.
#' @param minElements Molecular formulas, which contain lower and upper boundaries of allowed formula respectively.
#' @param maxElements Molecular formulas, which contain lower and upper boundaries of allowed formula respectively.

#' @details Sum formulas are calculated which explain the given mass or isotope pattern.
#'
#' @return A list of molecules, which contain the sub-lists `formulas` potential 
#'     formulas, `exactmass` exact mass of each hypothesis (not monoisotopic), 
#'     `score` calculated score, `isotopes` a list of isotopes.
#'     
#' @export
#' @import Rcpp
#' @useDynLib Rdisop, .registration = TRUE
#' 
#' @examples
#' # Glutamate: 
#' decomposeIsotopes(c(147.0529, 148.0563), c(100.0, 5.56))
#'
#' @author Steffen Neumann <sneumann@IPB-Halle.DE>
#' @references For a description of the underlying IMS see citation("Rdisop")
#'
decomposeIsotopes <- function(
  masses, intensities, ppm = 2.0, mzabs = 0.0001, elements = NULL, 
  filter = NULL, z = 0, maxisotopes = 10, 
  minElements = "C0", maxElements = "C999999"
) {
  
  # Use limited limited CHNOPS unless stated otherwise
  if (!is.list(elements) || length(elements) == 0) {
    elements <- initializeCHNOPS()
  }

  # If only a single mass is given, intensities are irrelevant
  if (length(masses) == 1) {
    intensities <- 1
  }

  if (length(masses) != length(intensities)) {
    stop("masses and intensities have different lengths!")
  }

  # Calculate (average) mass difference, guess charge and recalculate
  charge <- 1

  # Remember ordering of element names, but ensure list of elements is ordered by mass
  element_order <- sapply(elements, function(x) {
    x$name
  })
  elements <- elements[order(sapply(elements, function(x) {
    x$mass
  }))]

  # Calculate relative Error based on masses[1] and mzabs
  ppm <- ppm + mzabs / masses[1] * 1000000
  
  # Finally ready to make the call...
  .Call("decomposeIsotopes",
    masses, intensities, ppm, elements, element_order, z,
    maxisotopes, minElements, maxElements, PACKAGE = "Rdisop"
  )
}

#' @rdname decomposeIsotopes
#' @param mass A single mass (or m/z value).
#' @export
decomposeMass <- function(
  mass, ppm = 2.0, mzabs = 0.0001, elements = NULL, filter = NULL, z = 0,
  maxisotopes = 10, minElements = "C0", maxElements = "C999999"
) {
  # call the simplified version of decomposeIsotopes
  decomposeIsotopes(masses = c(mass), intensities = c(1), ppm = ppm, mzabs = mzabs,
    elements = elements, filter = filter, z = z, maxisotopes = maxisotopes,
    minElements = minElements, maxElements = maxElements
  )
}

#' @rdname decomposeIsotopes
#' @param molecule An initialized molecule as returned by getMolecule() or the decomposeMass() and decomposeIsotope() functions.
#' @export
isotopeScore <- function(
  molecule, masses, intensities, elements = NULL, filter = NULL, z = 0
) {
  # Obtain the similarity score between two molecules / isotope Patterns
  # Use limited limited CHNOPS unless stated otherwise
  if (!is.list(elements) || length(elements) == 0) {
    elements <- initializeCHNOPS()
  }
  
  # If only a single mass is given, intensities are irrelevant
  if (length(masses) == 1) {
    intensities <- 1
  }
  
  if (length(masses) != length(intensities)) {
    stop("masses and intensities have different lengths!")
  }
  
  # 	molecule <- getMolecule(formula)
  # 	massintensities<-matrix(NA,ncol=length(masses),nrow=2)
  # 	for(i in 1:length(masses))
  {
    # 		massintensities[1,i]<-masses[i];
    # 		massintensities[2,i]<-intensities[i];
  }
  # 	molecule$isotopes <- list(massintensities)
  #         scoreMolecule(molecule,elements,filter,z);
  predictedMass <- molecule$isotopes[[1]][1, ]
  predictedAbundances <- molecule$isotopes[[1]][2, ]
  
  # call the score function
  .Call("calculateScore", predictedMass, predictedAbundances, masses, intensities, PACKAGE = "Rdisop")
  
}
