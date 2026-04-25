#' @name addMolecules
#' @title Add/subtract sum formulas
#' @aliases subMolecules
#' 
#' @description Simple arithmetic modifications of sum formulas.
#'
#' @param formula1 Sum formula (can be a vector).
#' @param formula2 Sum formula.
#' @param elements List of allowed chemical elements, defaults to full periodic system of elements. See \code{\link{initializeElements}}.
#' @param maxisotopes Maximum number of isotopes shown in the resulting molecules.
#' 
#' @details `addMolecules` adds the second argument to the first. `subMolecules`
#'    subtracts the second argument from the first. This can be useful to revert
#'    e.g. adduct/fragment formation found in ESI mass spectrometry, or to 
#'    mimic simple chemical reactions. No chemical checks are performed.
#'
#' @return The input vector formula1 will be converted into molecule objects similar
#'     to function \code{\link{getMolecule}}. However, the results will be amended 
#'     or reduced by formula2.
#'
#' @examples
#' # Remove the proton-adduct from Ethanol
#' subMolecules("C2H7O", "H")
#' 
#' @export
addMolecules <- function(
  formula1, formula2, elements = NULL, maxisotopes = 10
) {
    # First argument may be vector of formulas, Second is single molecule only!
    if (length(formula2) > 1) { stop("Second formula must be single Molecule") }
    
    # Use the minimal set of the PSE unless stated otherwise
    if (!is.list(elements) || length(elements) == 0) { elements <- .minset_elements(paste0(formula1, formula2)) }
    
    maxisotopes <- .check_maxisotopes(maxisotopes)
  
    ordered <- .orderElementsByMass(elements)
    element_order <- ordered$element_order
    elements <- ordered$elements
    
    # Call imslib to parse formula and calculate masses and isotope pattern
    .Call(
        "addMolecules",
        formula1, formula2, elements, element_order, maxisotopes,
        PACKAGE = "Rdisop"
    )
}

#' @name addMolecules
#' @export
subMolecules <- function(
  formula1, formula2, elements = NULL, maxisotopes = 10
) {
    # First argument may be vector of formulas, second is single molecule only!
    if (length(formula2) > 1) { stop("Second formula must be single Molecule") }
    
    # Use the minimal set of the PSE unless stated otherwise
    if (!is.list(elements) || length(elements) == 0) { elements <- .minset_elements(paste0(formula1, formula2)) }
    
    maxisotopes <- .check_maxisotopes(maxisotopes)
    
    ordered <- .orderElementsByMass(elements)
    element_order <- ordered$element_order
    elements <- ordered$elements
  
    # Call imslib to parse formula and calculate masses and isotope pattern
    .Call(
        "subMolecules",
        formula1, formula2, elements, element_order, maxisotopes,
        PACKAGE = "Rdisop"
    )
}
