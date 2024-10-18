#' @name getMolecule
#' @title Calculate mass and isotope information for a molecule given as sum formula
#' @aliases getMass
#' 
#' @description Parse the sum formula and calculate the theoretical exact mass 
#'     and the isotope distribution for an approximate MS resolution of 20,000.
#'
#' @param formula Sum formula.
#' @param elements List of allowed chemical elements, defaults to full periodic system of elements.
#' @param z Charge z of molecule for exact mass calculation.
#' @param maxisotopes Maximum number of isotopes shown for the resulting molecule.
#'
#' @details \code{getMolecule()} will parse the sum formula and calculate the 
#'     exact mass and the isotope distribution.
#'     The exact mass is the mass of the most abundant isotope and is not 
#'     identical with the monoisotopic mass. The latter can be extracted using 
#'     the function `getMono()`. This function can also be supplied with a 
#'     vector of chemical formulas directly (in case that the isotopic 
#'     distribution is of no interest).
#'     Since of version 1-65-3, if a charge is specified, the exact mass of the 
#'     molecule will be reduced or increased by n-times the electron mass 
#'     (depending on the sign). Also, isotopic masses will additionally be 
#'     divided by the charge specified to reflect what would be measured in HR-MS.
#' 
#' @return A list containing the elements `formula` (repeated sum formula), 
#'     `mass` exact mass of molecule, `score` probability, for given molecules a 
#'     dummy value which is always 1.0, `valid` result of neutrogen rule check, 
#'     `isotopes` a list of isotope masses and abundances.
#'     
#' @export
#'
#' @examples
#' # Ethanol
#' getMolecule("C2H6O")
#' 
#' @author Steffen Neumann <sneumann@IPB-Halle.DE>
#' @references For a description of the underlying IMS see citation("Rdisop")
#' 
getMolecule <- function(formula, elements = NULL, z = 0, maxisotopes=10) {
  # Use full PSE unless stated otherwise
  if (!is.list(elements) || length(elements)==0 ) {
    elements <- initializePSE()
  }
  
  # Remember ordering of element names,
  # but ensure list of elements is ordered
  # by mass
  element_order <- sapply(elements, function(x){ x$name })
  elements <- elements[order(sapply(elements, function(x) { x$mass }))]
  
  # Call imslib to parse formula and calculate
  # masses and isotope pattern
  molecule <- .Call("getMolecule",
                    formula, elements, element_order,
                    z, maxisotopes,
                    PACKAGE="Rdisop")
  
  # the charge parameter is not correctly used within C++ imslib to calculate
  # appropriate deviations in exactmass and isotopes info
  # to solve this the result is postprocessed here
  if (z!=0) {
      # JL: I would keep this message to inform previous users of Rdisop regarding
      # the modification but remove it in the future (i.e. in 1 year, so 10/2025).
      message("You specified a charge z different from 0. Please read the details part of the documentation.")
      molecule[["exactmass"]] <- (molecule[["exactmass"]] - z * 0.00054858)/abs(z)
      molecule[["isotopes"]] <- lapply(molecule[["isotopes"]], function(y) { 
          y[1,] <- (y[1,] - z * 0.00054858)/abs(z) 
          y
      }) 
  }
  
  molecule
}

#' @rdname getMolecule
#' @param molecule An initialized molecule as returned by getMolecule() or the decomposeMass() and decomposeIsotope() functions.
#' @export
getMass <- function(molecule) {
  molecule$exactmass
}

#' @rdname getMolecule
#' @export
getMono <- function(molecule) {
    if (is.list(molecule)) {
        # assume that the user provided a list as returned by getMolecule
        molecule <- molecule$formula
    }
    # assume that the user provided a vector of chemical formulas
    sapply(molecule, function(x) {
        ele <- CountChemicalElements(x)
        sum(Rdisop::mono_masses[names(ele)]*ele)
    })
    
}

#' @rdname getMolecule
#' @param index Return the n-th isotope mass/abundance pair(s) of the molecule
#' @export
getIsotope <- function(molecule, index) {
    # [JL, 1.65.4] substituted the getter function to be consistent with the 
    # others and return a list of similar length as meolecule$isotopes 
    # containing a matrix each
    lapply(molecule$isotopes, function(x) {
        x[,index,drop=FALSE]
    })
    #molecule$isotopes[[1]][,index]
}

#' @rdname getMolecule
#' @export
getFormula <- function(molecule) {
  if (is.null(molecule$formula)) {
    lapply(molecule, function(x) {x$formula})
  } else {
    molecule$formula
  }
}

#' @rdname getMolecule
#' @export
getScore <- function(molecule) {
  if (is.null(molecule$score)) {
    lapply(molecule, function(x) {x$score})
  } else {
    molecule$score
  }
}

#' @rdname getMolecule
#' @export
getValid <- function(molecule) {
  if (is.null(molecule$valid)) {
    lapply(molecule, function(x) {x$valid})
  } else {
    molecule$valid
  }
}
