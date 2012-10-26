
.getElement <- function(name, elements = NULL) {

    if (!is.list(elements) || length(elements)==0 ) {
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

getMolecule <- function(formula, elements = NULL, z = 0, maxisotopes=10) {
    # Use full PSE unless stated otherwise
    if (!is.list(elements) || length(elements)==0 ) {
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
                      z, maxisotopes,
                      PACKAGE="Rdisop")

    molecule
}


addMolecules <- function(formula1, formula2,
                         elements = NULL, maxisotopes=10)
{

    # First argument may be vector of formulas,
    # Second is single molecule only!
    if (length(formula2) >1 ) {
        stop("Second formula must be single Molecule")
    }

    # Use full PSE unless stated otherwise
    if (!is.list(elements) || length(elements)==0 ) {
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
                      maxisotopes,
                      PACKAGE="Rdisop")

    molecule
}

subMolecules <- function(formula1, formula2,
                         elements = NULL, maxisotopes=10)
{

    # First argument may be vector of formulas,
    # Second is single molecule only!
    if (length(formula2) >1 ) {
        stop("Second formula must be single Molecule")
    }

    # Use full PSE unless stated otherwise
    if (!is.list(elements) || length(elements)==0 ) {
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
                      maxisotopes,
                      PACKAGE="Rdisop")

    molecule
}


decomposeMass <- function(mass, ppm=2.0, mzabs=0.0001,
                          elements=NULL, filter=NULL, z=0, maxisotopes=10,
                          minElements="C0", maxElements="C999999") {
    decomposeIsotopes(c(mass), c(1), ppm=ppm, mzabs=mzabs,
                      elements=elements, filter=filter, z=z, maxisotopes=maxisotopes,
                      minElements=minElements, maxElements=maxElements)
}

decomposeIsotopes <- function(masses, intensities, ppm=2.0, mzabs=0.0001,
                              elements=NULL, filter=NULL, z=0, maxisotopes=10,
                              minElements="C0", maxElements="C999999")
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

    ##
    ## Calculate relative Error based on masses[1] and mzabs
    ##

    ppm <- ppm + mzabs/masses[1]*1000000
    # Finally ready to make the call...
    molecules <- .Call("decomposeIsotopes",
                       masses, intensities, ppm, elements, element_order, z,
                       maxisotopes,
                       minElements, maxElements,
                       PACKAGE="Rdisop")

    molecules
}

#
# Obtain the similarity score
# between two molecules / isotope Patterns
#
isotopeScore <- function(molecule, masses, intensities,
                         elements=NULL, filter=NULL, z=0) {

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

# 	molecule <- getMolecule(formula)
# 	massintensities<-matrix(NA,ncol=length(masses),nrow=2)
# 	for(i in 1:length(masses))
	{
# 		massintensities[1,i]<-masses[i];
# 		massintensities[2,i]<-intensities[i];
	}
# 	molecule$isotopes <- list(massintensities)
#         scoreMolecule(molecule,elements,filter,z);
	predictedMass<-molecule$isotopes[[1]][1,];
	predictedAbundances<-molecule$isotopes[[1]][2,];
	score <- .Call("calculateScore",predictedMass,predictedAbundances,masses,intensities,PACKAGE="Rdisop")

	score

# 	molecule
}
