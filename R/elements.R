
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
    d <- list(name="D", mass=2,  isotope = list(mass=c(0.014102),	   abundance=c(1))) #Heavy Water
    h <- list(name="H", mass=1,  isotope = list(mass=c(0.007825,0.014102), abundance=c(0.99985, 0.00015)))
    n <- list(name="N", mass=14, isotope = list(mass=c(0.003074,0.000109), abundance=c(0.99634, 0.00366)))

    o <- list(name="O", mass=16, isotope = list(mass=c(-0.00509,-0.000868,-0.000839),
                                 abundance=c(           0.99762, 0.00038,  0.002)))

    p <- list(name="P", mass=31, isotope = list(mass=c(-0.026238), abundance=c(1.0)))

    s <- list(name="S", mass=32, isotope = list(mass     =c(-0.027929, -0.028541,  -0.032133, 0, -0.032919),
                                 abundance=c( 0.9502,    0.0075,     0.0421,   0,  0.0002)))


    na <- list(name="Na", mass= 23, isotope = list(mass=c(-0.01023072), abundance=c(1.0)))

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
                                   abundance=c( 0.7899,       0.1000 ,    0.1101)))


    ca <- list(name="Ca", mass=40, isotope = list(     mass=c( -0.0374094, +0.9586176, +0.9587662, +0.9554806   ),
                                   abundance=c(  0.96941,    0.00647,    0.00135,    0.02086  )))

    cd <- list(name="Cd", mass=106, isotope = list(    mass=c( -0.093539, +0.904176, +1.903005, +1.904182, +1.902757, +1.904400, +1.903357, +2.904755),
				   abundance=c(0.0125, 0.0089, 0.1249, 0.1280, 0.2413, 0.1222, 0.2873, 0.0749)))
    hg <- list(name="Hg", mass=196, isotope = list(    mass=c(-0.034193, +0.966743, +0.968254, +0.968300, +0.970277, +0.970617, +1.973467),
				   abundance=c(0.0015, 0.0997, 0.1687, 0.2310, 0.1318, 0.2986, 0.0687)))

    zn <- list(name="Zn", mass=64, isotope = list(     mass=c(-0.0708552, 0.0, -0.0739653, -0.0728709, -0.0751541, 0.0, -0.074675),
				   abundance=c(0.4863, 0.0, 0.2790, 0.0410, 0.1875, 0.0, 0.0062)))

    cu <- list(name="Cu", mass=63, isotope = list(	mass=c(-0.0704011, 0.0, -0.0722071),
				   abundance=c(0.6917, 0.0, 0.3083)))

    ni <- list(name="Ni", mass=58, isotope = list(	mass=c(-0.0646538, 0.0, -0.0692116, -0.0689421, -0.0716539, 0.0, -0.0720321),
				  abundance=c(0.680769, 0.0, 0.262231, 0.011399, 0.036345, 0.0, 0.009256)))

    mn <- list(name="Mn", mass=55, isotope = list(	mass=c(0.0619529),
				  abundance=c(1.0)))

    mo <- list(name="Mo", mass=91, isotope = list(	mass=c(0.906809, 0.0, 0.9050853, 0.9058411, 0.9046785, 0.9060205, 0.905054073, 0.0, 0.907477),
				abundance=c(0.1484, 0.0, 0.0925, 0.1668, 0.0955, 0.2413, 0.0, 0.0963)))

    list(c,h,d,n,o,p,s, na, k, cl, br, f, i, fe, mg, ca, cd, hg, zn, cu, ni, mn, mo)
}

initializeCharges <- function() {
    positive <- list(name="+", mass=0, isotope = list(mass=c(-0.005485), abundance=c(1)))
    negative <- list(name="-", mass=0, isotope = list(mass=c(+0.005485), abundance=c(1)))
    list(positive, negative)
}

#
# Tests
#

# m <- initializePSE()
# m <- initializeCHNOPS()
# m <- initializeCHNOPSMgKCaFe()
