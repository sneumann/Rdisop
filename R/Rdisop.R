
.getElement <- function(name, elements = NULL) {

    if (is.list(elements) || length(elements)==0 ) {
        elements <- initializePSE()
    }
    rex <- paste ("^",name,"$", sep="")

    elements [[grep (rex, sapply (elements, function(x) {x$name}))]]
}

getMass <- function(molecule) {
    molecule$exactmass
}

getIsotope <- function(molecule, index) {
    molecule$isotope[[1]][,index]
}

getFormula <- function(molecule) {
    if (is.null(molecule$formula)) {
        lapply(molecule, function(x) {x$formula})
    } else {
        molecule$formula
    }
}

getScore <- function(molecule) {
    if (is.null(molecule$score)) {
        lapply(molecule, function(x) {x$score})
    } else {
        molecule$score
    }
}

getValid <- function(molecule) {
    if (is.null(molecule$valid)) {
        lapply(molecule, function(x) {x$valid})
    } else {
        molecule$valid
    }
}

getMolecule <- function(formula, elements = NULL, z = 0) {
    # Use full PSE unless stated otherwise
    if (is.list(elements) || length(elements)==0 ) {
        elements <- initializePSE()
    }

    # Remember ordering of element names,
    # but ensure list of elements is ordered
    # by mass
    element_order <- sapply(elements, function(x){x$name})
    elements <- elements[order(sapply(elements, function(x){x$mass}))]

    # Call imslib to parse formula and calculate
    # masses and isotope pattern
    molecule <- .Call("getMolecule",
                      formula, elements, element_order,
                      z, PACKAGE="Rdisop")

    molecule
}


addMolecules <- function(formula1, formula2,
                         elements = NULL)
{

    # First argument may be vector of formulas,
    # Second is single molecule only!
    if (length(formula2) >1 ) {
        stop("Second formula must be single Molecule")
    }

    # Use full PSE unless stated otherwise
    if (is.list(elements) || length(elements)==0 ) {
        elements <- initializePSE()
    }

    # Remember ordering of element names,
    # but ensure list of elements is ordered
    # by mass
    element_order <- sapply(elements, function(x){x$name})
    elements <- elements[order(sapply(elements, function(x){x$mass}))]

    # Call imslib to parse formula and calculate
    # masses and isotope pattern
    molecule <- .Call("addMolecules",
                      formula1, formula2,
                      elements, element_order,
                      PACKAGE="Rdisop")

    molecule
}

subMolecules <- function(formula1, formula2,
                         elements = NULL)
{

    # First argument may be vector of formulas,
    # Second is single molecule only!
    if (length(formula2) >1 ) {
        stop("Second formula must be single Molecule")
    }

    # Use full PSE unless stated otherwise
    if (is.list(elements) || length(elements)==0 ) {
        elements <- initializePSE()
    }

    # Remember ordering of element names,
    # but ensure list of elements is ordered
    # by mass
    element_order <- sapply(elements, function(x){x$name})
    elements <- elements[order(sapply(elements, function(x){x$mass}))]

    # Call imslib to parse formula and calculate
    # masses and isotope pattern
    molecule <- .Call("subMolecules",
                      formula1, formula2,
                      elements, element_order,
                      PACKAGE="Rdisop")

    molecule
}


decomposeMass <- function(mass, ppm=5.0, elements=NULL, filter=NULL, z=0) {
    decomposeIsotopes(c(mass), c(1), elements=elements, filter=filter, z=z)
}

decomposeIsotopes <- function(masses, intensities, ppm=5.0,
                              elements=NULL, filter=NULL, z=0)
{
    # Use limited limited CHNOPS unless stated otherwise
    if (!is.list(elements) || length(elements)==0 ) {
        elements <- initializeCHNOPS()
    }


    # If only a single mass is given,
    # intensities are irrelevant
    if (length(masses) == 1) {
        intensities <- 1
    }

    if (length(masses) != length(intensities) )  {
        stop("masses and intensities have different lengths!")
    }

    # Calculate (average) mass difference,
    # guess charge and recalculate
    #
    charge <- 1

    # Remember ordering of element names,
    # but ensure list of elements is ordered
    # by mass
    element_order <- sapply(elements, function(x){x$name})
    elements <- elements[order(sapply(elements, function(x){x$mass}))]

    # Finally ready to make the call...
    molecules <- .Call("decomposeIsotopes",
                       masses, intensities, ppm, elements, element_order, z,
                       PACKAGE="Rdisop")

    molecules
}

#
# Obtain the similarity score
# between two molecules / isotope Patterns
#
isotopeScore <- function(molecule, masses, intensities,
                         elements=NULL, filter=NULL, z=0) {

    stop("NYI")
}

