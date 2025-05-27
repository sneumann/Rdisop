#' @name decomposeIsotopes
#' @title Mass Decomposition of Isotope Patterns
#' @aliases decomposeMass
#' @aliases isotopeScore
#'
#' @description  Calculate the elementary compositions from an exact mass or
#'     isotope pattern, obtained e.g. by FTICR or TOF mass spectrometers.
#'
#' @param masses A vector of masses (or m/z values) of an isotope cluster.
#' @param intensities Absolute or relative intensities of the \code{masses} peaks.
#' @param ppm Allowed deviation of hypotheses from given mass.
#' @param mzabs Absolute deviation in Dalton (mzabs and ppm will be added).
#' @param elements List of allowed chemical elements, defaults to CHNOPS. 
#'     See \code{\link{initializeElements}}. Can also be specified similar to
#'     parameter `minElements` like a formula. 'CHNOPS' would define the elements
#'     C, H, N, O, P and S. 'NA' would specify the element N and return a warning
#'     regarding 'A' because this element is undefined in the PSE.
#' @param filter NYI, will be a selection of DU, DBE and Nitrogen rules.
#' @param z Charge z of m/z peaks for calculation of real mass, keep z=0 for auto-detection.
#' @param maxisotopes Maximum number of isotopes shown in the resulting molecules.
#' @param minElements Molecular formula, defining lower boundaries of allowed elements.
#' @param maxElements Molecular formula, defining upper boundaries of allowed elements.

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
#' # query some measurement values from a Glutamate peak which will return two
#' # suggested/potential sum formulas
#' m <- c(147.0529, 148.0563, 149.0612)
#' i <- c(0.91, 0.06, 0.01)
#' mol <- decomposeIsotopes(m, i, maxisotopes = 3)
#' getFormula(mol)
#' 
#' # Rdisop returns the scores (how well does the exact data match the measured 
#' # data) in a normalized fashion, but you can calculate the raw scores
#' getScore(mol)
#' isotopeScore(mol, m, i)
#' 
#' # using a 5 mDa window, the number of potential candidates is increased to 26
#' getFormula(decomposeIsotopes(m, i, mzabs = 0.005))
#' 
#' # elemental ranges can be specified to affect the result
#' # use maxElements to exclude all of the above suggestions containing more 
#' # than one S and/or one P.
#' getFormula(decomposeIsotopes(m, i, mzabs = 0.005, maxElements = "S1P1"))
#' 
#'
#' @author Steffen Neumann <sneumann@IPB-Halle.DE>
#' @references For a description of the underlying IMS see citation("Rdisop")
#'
decomposeIsotopes <- function(
    masses, intensities, ppm = 2.0, mzabs = 0.0001, elements = NULL, 
    filter = NULL, z = 0, maxisotopes = 10, 
    minElements = "C0", maxElements = "C999999"
) {
  
    # Use CHNOPS unless stated otherwise
    elements <- .check_elements(x = elements, default = initializeCHNOPS())

    # If only a single mass is given, intensities are irrelevant
    if (length(masses) == 1) { intensities <- 1 }
    
    if (length(masses) != length(intensities)) { stop("masses and intensities have different lengths!") }
    
    # Calculate (average) mass difference, guess charge and recalculate
    # ToDo: we could calculate the absolute number of charges for typical CHNOPS compounds using
    # z <- which.min(median(diff(masses))-1/1:5)
    # however, we can not determine positive or negative ionization this way

    .check_maxisotopes(maxisotopes)
    
    # Remember ordering of element names (for Formula output), but ensure list of elements is ordered by mass
    element_order <- sapply(elements, function(x) { x$name })
    elements <- elements[order(sapply(elements, function(x) { x$mass }))]
    
    # Calculate relative Error based on masses[1] and mzabs
    ppm <- ppm + mzabs / masses[1] * 1000000
  
    minElements <- .check_limElements(minElements, elements, default = 0)
    maxElements <- .check_limElements(maxElements, elements, default = 999)
    
    # Finally ready to make the call...
    # 20241106: de-couple 'intensities' from the calling environment using c(intensities) to solve issue #21
    .Call(
        "decomposeIsotopes",
        masses, c(intensities), ppm, elements, element_order, 
        z, maxisotopes, minElements, maxElements, 
        PACKAGE = "Rdisop"
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
#' @param molecule An initialized molecule as returned by getMolecule() or the decomposeMass() and decomposeIsotopes() functions.
#' @export
isotopeScore <- function(
    molecule, masses, intensities
) {
    # Obtain the similarity score between two molecules / isotope Patterns
    # If only a single mass is given, intensities are irrelevant
    if (length(masses) == 1) {
        intensities <- 1
    }
    
    if (length(masses) != length(intensities)) {
        stop("masses and intensities have different lengths!")
    }
  
    scores <- sapply(stats::setNames(molecule$isotopes, molecule$formula), function(x) {
        predictedMass <- x[1,]
        predictedAbundances <- x[2,]
        # call the score function
        .Call("calculateScore", predictedMass, predictedAbundances, masses, intensities, PACKAGE = "Rdisop")
    })
    
    return(unlist(scores))

}
