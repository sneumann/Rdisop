
#
# Create a set of elements
# as a subset of PSE
#
# Example:
#
# elements <- initializeCHNOPS()
#
initializeElements <- function(names) {
    elements <- initializePSE()
    lapply(names, function (name) {.getElement(name, elements)})
}

#
# Create a set of elements
# with standard CHNOPS
#
# Example:
#
# elements <- initializeCHNOPS()
#
initializeCHNOPS <- function() {
    initializeElements(c("C", "H", "N", "O", "P", "S"))
}

#
# Create a set of elements
# with standard CHNOPS+Ions
#
# Example:
#
# elements <- initializeCHNOPSMgKCaFe()
#
initializeCHNOPSMgKCaFe <- function() {
    initializeElements(c("C", "H", "N", "O", "P", "S", "Mg", "K", "Ca", "Fe"))
}

initializeCHNOPSNaK <- function() {
    initializeElements(c("C", "H", "N", "O", "P", "S", "Na", "K"))
}


#
# Create a set of Elements containing the full PSE
#
# Example:
#
# names <- initializePSE()
# http://www.webelements.com/
#

initializePSE <- function() {
    c <- list(name="C", mass=12, isotope = list(mass=c(0.0,     0.003355), abundance=c(0.9889,  0.0111)))
    h <- list(name="H", mass=1,  isotope = list(mass=c(0.007825,0.014102), abundance=c(0.99985, 0.00015)))
    n <- list(name="N", mass=14, isotope = list(mass=c(0.003074,0.000109), abundance=c(0.99634, 0.00366)))

    o <- list(name="O", mass=16, isotope = list(mass =c(-0.005085,-0.000868,-0.000839),
                                 abundance=c( 0.99762,  0.00038,  0.002)))

    p <- list(name="P", mass=31, isotope = list(mass=c(-0.026238), abundance=c(1.0)))

    s <- list(name="S", mass=32, isotope = list(mass     =c(-0.027929, -0.028541,  -0.032133, 0, -0.032919),
                                 abundance=c( 0.9502,    0.0075,     0.0421,   0,  0.0002)))


    na <- list(name="Na", mass= 23, isotope = list(mass=c(0.0), abundance=c(1.0)))

    k  <- list(name="K",  mass= 39, isotope = list(     mass=c(-0.036293, -0.036001, -0.038175),
                                    abundance=c( 0.932581,  0.0117,    6.7302 )))

    cl <- list(name="Cl", mass= 35, isotope = list(     mass=c( -0.0311, 0.0, -0.0341),
                                    abundance=c(  0.7577, 0.0,  0.2423)))

    br <- list(name="Br", mass= 79, isotope = list(     mass=c( -0.0817, 0.0, -0.0837 ),
                                    abundance=c(  0.505,  0.0,  0.495)))


    si <- list(name="Si", mass= 28, isotope = list(     mass=c( -0.0231, -0.0235, -0.0262  ),
                                    abundance=c(  0.9228,  0.0470,  0.0302  )))

    f  <- list(name="F",  mass= 19, isotope = list(mass=c( -0.0016), abundance=c(1.0)))
    i  <- list(name="I",  mass=127, isotope = list(mass=c( -0.0955), abundance=c(1.0)))

    fe <- list(name="Fe", mass= 54, isotope = list(     mass=c( -0.060387, 0.0, -0.065061, -0.0646042, -0.0667227   ),
                                    abundance=c( 0.05845  , 0.0,  0.91754,   0.02119, 0.00282  )))


    mg <- list(name="Mg", mass=24, isotope = list(     mass=c( -0.0149577, -0.0141626, -0.017406 ),
                                   abundance=c( 78.99,       0.1000 ,    0.1101)))


    ca <- list(name="Ca", mass=40, isotope = list(     mass=c( -0.0374094, -0.0413824, -0.0412338, -0.0445194   ),
                                   abundance=c(  0.96941,    0.00647,    0.00135,    0.02086  )))


    list(c,h,n,o,p,s, na, k, cl, br, f, i, fe, mg, ca)
}



#
# Tests
#

# m <- initializePSE()
# m <- initializeCHNOPS()
# m <- initializeCHNOPSMgKCaFe()
