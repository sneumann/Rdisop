#' @name addMolecules
#' @title Add/subtract sum formulae
#' @aliases subMolecules
#' 
#' @description Simple arithmetic modifications of sum formulae.
#'
#' @param formula1 Sum formula.
#' @param formula2 Sum formula.
#' @param elements list of allowed chemical elements, defaults to full periodic system of elements.
#' @param maxisotopes maximum number of isotopes shown in the resulting molecules.
#' 
#' @details `addMolecules` adds the second argument to the first. `subMolecules`
#'    subtracts the second argument from the first. This can be useful to revert
#'    e.g. adduct/fragment formation found in ESI mass spectrometry, or to 
#'    mimick simple chemical reactions. No chemical checks are performed.#' 
#'
#' @return The input vector formula1 either amended or reduced by formula2.
#' @export
#'
#' @examples
#' # Remove the proton-Adduct from Ethanol
#' subMolecules("C2H7O", "H")
addMolecules <- function(
  formula1, formula2, elements = NULL, maxisotopes = 10
) {
  # First argument may be vector of formulas, Second is single molecule only!
  if (length(formula2) > 1) {
    stop("Second formula must be single Molecule")
  }
  
  # Use full PSE unless stated otherwise
  if (!is.list(elements) || length(elements) == 0) {
    elements <- initializePSE()
  }
  
  # Remember ordering of element names, but ensure list of elements is ordered by mass
  element_order <- sapply(elements, function(x) {
    x$name
  })
  elements <- elements[order(sapply(elements, function(x) {
    x$mass
  }))]
  
  # Call imslib to parse formula and calculate
  # masses and isotope pattern
  .Call("addMolecules",
    formula1, formula2,
    elements, element_order,
    maxisotopes,
    PACKAGE = "Rdisop"
  )
}

#' @name addMolecules
#' @export
subMolecules <- function(
  formula1, formula2, elements = NULL, maxisotopes = 10
) {
  # First argument may be vector of formulas, second is single molecule only!
  if (length(formula2) > 1) {
    stop("Second formula must be single Molecule")
  }
  
  # Use full PSE unless stated otherwise
  if (!is.list(elements) || length(elements) == 0) {
    elements <- initializePSE()
  }
  
  # Remember ordering of element names, but ensure list of elements is ordered by mass
  element_order <- sapply(elements, function(x) {
    x$name
  })
  elements <- elements[order(sapply(elements, function(x) {
    x$mass
  }))]
  
  # Call imslib to parse formula and calculate masses and isotope pattern
  .Call("subMolecules",
    formula1, formula2,
    elements, element_order,
    maxisotopes,
    PACKAGE = "Rdisop"
  )
}
