#' @name initializeElements
#' @title Initialize (a subset of) elements of the periodic system of elements (PSE)
#' @aliases initializeCHNOPS
#' @aliases initializeCHNOPSMgKCaFe
#' @aliases initializeCHNOPSNaK
#' @aliases initializePSE
#' @aliases initializeCharges
#' @aliases .getElement
#' 
#' @description Initialize the information about name, mass and isotopes. To 
#'     reduce the number of decomposition hypotheses, subsets of elements can be 
#'     created.
#'     
#' @param method Use isotope mass and abundance data from either "NIST"
#'     (default) or "IUPAC".
#'
#' @param names Vector of element names within PSE.
#'
#' @details These functions return full, pre-defined or user-defined 
#'     (sub-) lists of elements.
#' 
#' @return A list with the elements `name` repeated sum formula, `mass` nominal 
#'     mass of molecule, `isotope` a list of isotopes. The function `initializeCharges` 
#'     is special, since it allows to parse charges as shown in examples.
#'     
#' @export
#'
#' @examples
#' initializeCHNOPS()
#' getMolecule("H3O+", elements=c(initializeCHNOPS(),initializeCharges()))
#' 
#' @author Steffen Neumann <sneumann@IPB-Halle.DE>
#' @references For a description of the underlying IMS see citation("Rdisop").
#'     Isotope patterns were obtained through wikipedia.org
initializeElements <- function(names, method=c("NIST","IUPAC")) {
    method <- match.arg(method)
    elements <- initializePSE(method = method)
    lapply(names, function (name) {.getElement(name, elements)})
}

#' @rdname initializeElements
#' @param name Chemical element name (abbr.).
#' @param elements Character vector of chemical element names.
#' @export
.getElement <- function(name, elements = NULL) {
  if (!is.list(elements) || length(elements) == 0) {
    elements <- initializePSE()
  }
  rex <- paste("^", name, "$", sep = "")
  
  elements[[grep(rex, sapply(elements, function(x) {
    x$name
  }))]]
}

#' @rdname initializeElements
#' @export
initializeCHNOPS <- function() {
  initializeElements(c("C", "H", "N", "O", "P", "S"))
}

#' @rdname initializeElements
#' @export
initializeCHNOPSMgKCaFe <- function() {
    initializeElements(c("C", "H", "N", "O", "P", "S", "Mg", "K", "Ca", "Fe"))
}

#' @rdname initializeElements
#' @export
initializeCHNOPSNaK <- function() {
    initializeElements(c("C", "H", "N", "O", "P", "S", "Na", "K"))
}

#' @rdname initializeElements
#' @export
initializePSE <- function(method=c("NIST","IUPAC")) {
    method <- match.arg(method)
    if (method=="IUPAC")
        return(.initializePSE_IUPAC())
    else if (method=="NIST")
        return(.initializePSE_NIST())
    else
        stop("Unknown table requested")
}

#' @noRd
#' @keywords internal
.initializePSE_IUPAC <- function() {	
    D <- list(name="D", mass=2,  isotope = list(mass=c(0.014102), abundance=c(1))) #Heavy Water
    Ac <- list(name= 'Ac', mass=227, isotope=list(mass=c(0.02775),abundance=c(1)))
    Ag <- list(name= 'Ag', mass=107, isotope=list(mass=c(-0.094908, 0, -0.095243),abundance=c(0.51839, 0, 0.48161)))
    Al <- list(name= 'Al', mass=27, isotope=list(mass=c(-0.0184614),abundance=c(1)))
    Am <- list(name= 'Am', mass=243, isotope=list(mass=c(0.0614),abundance=c(1)))
    Ar <- list(name= 'Ar', mass=36, isotope=list(mass=c(-0.03245448, 0, -0.0372675, 0, -0.0376163),abundance=c(0.003365, 0, 0.000632, 0, 0.996003)))
    As <- list(name= 'As', mass=75, isotope=list(mass=c(-0.0784058),abundance=c(1)))
    At <- list(name= 'At', mass=210, isotope=list(mass=c(-0.012874),abundance=c(1)))
    Au <- list(name= 'Au', mass=197, isotope=list(mass=c(-0.033457),abundance=c(1)))
    B <- list(name= 'B', mass=10, isotope=list(mass=c(0.0129369, 0.0093054),abundance=c(0.199, 0.801)))
    Ba <- list(name= 'Ba', mass=130, isotope=list(mass=c(-0.093718, 0, -0.094958, 0, -0.095514, -0.094335, -0.095447, -0.094188, -0.094768),abundance=c(0.00106, 0, 0.00101, 0, 0.02417, 0.06592, 0.07854, 0.1123, 0.717)))
    Be <- list(name= 'Be', mass=9, isotope=list(mass=c(0.0121822),abundance=c(1)))
    Bi <- list(name= 'Bi', mass=209, isotope=list(mass=c(-0.019626),abundance=c(1)))
    Bk <- list(name= 'Bk', mass=247, isotope=list(mass=c(0.0702),abundance=c(1)))
    Br <- list(name= 'Br', mass=79, isotope=list(mass=c(-0.0816639, 0, -0.083711),abundance=c(0.5069, 0, 0.4931)))
    C <- list(name= 'C', mass=12, isotope=list(mass=c(0, 0.003354826, 0.003241982),abundance=c(0.989, 0.011, 0)))
    Ca <- list(name= 'Ca', mass=40, isotope=list(mass=c(-0.0374094, 0, -0.0413824, -0.0412338, -0.0445194, 0, -0.046311, 0, -0.047467),abundance=c(0.96941, 0, 0.00647, 0.00135, 0.02086, 0, 4e-05, 0, 0.00187)))
    Cd <- list(name= 'Cd', mass=106, isotope=list(mass=c(-0.093539, 0, -0.095824, 0, -0.096995, -0.095818, -0.097242, -0.0956, -0.096643, 0, -0.095246),abundance=c(0.0125, 0, 0.0089, 0, 0.1249, 0.128, 0.2413, 0.1222, 0.2873, 0, 0.0749)))
    Ce <- list(name= 'Ce', mass=136, isotope=list(mass=c(-0.09286, 0, -0.094015, 0, -0.094567, 0, -0.090759),abundance=c(0.0019, 0, 0.0025, 0, 0.8848, 0, 0.1108)))
    Cf <- list(name= 'Cf', mass=251, isotope=list(mass=c(0.08),abundance=c(1)))
    Cl <- list(name= 'Cl', mass=35, isotope=list(mass=c(-0.03114728, 0, -0.03409738),abundance=c(0.7577, 0, 0.2423)))
    Cm <- list(name= 'Cm', mass=247, isotope=list(mass=c(0.07),abundance=c(1)))
    Co <- list(name= 'Co', mass=59, isotope=list(mass=c(-0.0668024),abundance=c(1)))
    Cr <- list(name= 'Cr', mass=50, isotope=list(mass=c(-0.0539536, 0, -0.0594902, -0.0593487, -0.0611175),abundance=c(0.04345, 0, 0.83789, 0.09501, 0.02365)))
    Cs <- list(name= 'Cs', mass=133, isotope=list(mass=c(-0.094571),abundance=c(1)))
    Cu <- list(name= 'Cu', mass=63, isotope=list(mass=c(-0.0704011, 0, -0.0722071),abundance=c(0.6917, 0, 0.3083)))
    Dy <- list(name= 'Dy', mass=156, isotope=list(mass=c(-0.075723, 0, -0.075597, 0, -0.074807, -0.07307, -0.073205, -0.071272, -0.070829),abundance=c(6e-04, 0, 0.001, 0, 0.0234, 0.189, 0.255, 0.249, 0.282)))
    Er <- list(name= 'Er', mass=162, isotope=list(mass=c(-0.071225, 0, -0.070802, 0, -0.06971, -0.067954, -0.067632, 0, -0.064539),abundance=c(0.0014, 0, 0.0161, 0, 0.336, 0.2295, 0.268, 0, 0.149)))
    Es <- list(name= 'Es', mass=252, isotope=list(mass=c(0.08),abundance=c(1)))
    Eu <- list(name= 'Eu', mass=151, isotope=list(mass=c(-0.080153, 0, -0.078775),abundance=c(0.478, 0, 0.522)))
    F <- list(name= 'F', mass=19, isotope=list(mass=c(-0.00159678),abundance=c(1)))
    Fe <- list(name= 'Fe', mass=54, isotope=list(mass=c(-0.0603873, 0, -0.0650607, -0.0646042, -0.0667227),abundance=c(0.058, 0, 0.9172, 0.022, 0.0028)))
    Fm <- list(name= 'Fm', mass=257, isotope=list(mass=c(0.1),abundance=c(1)))
    Fr <- list(name= 'Fr', mass=223, isotope=list(mass=c(0.019733),abundance=c(1)))
    Ga <- list(name= 'Ga', mass=69, isotope=list(mass=c(-0.07442, 0, -0.0752995),abundance=c(0.60108, 0, 0.39892)))
    Gd <- list(name= 'Gd', mass=152, isotope=list(mass=c(-0.080214, 0, -0.079139, -0.077382, -0.077882, -0.076044, -0.075981, 0, -0.072951),abundance=c(0.002, 0, 0.0218, 0.148, 0.2047, 0.1565, 0.2484, 0, 0.2186)))
    Ge <- list(name= 'Ge', mass=70, isotope=list(mass=c(-0.0757503, 0, -0.0779211, -0.0765374, -0.0788226, 0, -0.0785984),abundance=c(0.2123, 0, 0.2766, 0.0773, 0.3594, 0, 0.0744)))
    H <- list(name= 'H', mass=1, isotope=list(mass=c(0.007825035, 0.01410179, 0.01604927),abundance=c(0.99985, 0.00015, 0)))
    He <- list(name= 'He', mass=3, isotope=list(mass=c(0.01602931, 0.00260324),abundance=c(1.37e-06, 0.9999986)))
    Hf <- list(name= 'Hf', mass=174, isotope=list(mass=c(-0.059956, 0, -0.058594, -0.056783, -0.056304, -0.0541878, -0.0534543),abundance=c(0.00162, 0, 0.05206, 0.18606, 0.27297, 0.13629, 0.351)))
    Hg <- list(name= 'Hg', mass=196, isotope=list(mass=c(-0.034193, 0, -0.033257, -0.031746, -0.0317, -0.029723, -0.029383, 0, -0.026533),abundance=c(0.0015, 0, 0.0997, 0.1687, 0.231, 0.1318, 0.2986, 0, 0.0687)))
    Ho <- list(name= 'Ho', mass=165, isotope=list(mass=c(-0.069681),abundance=c(1)))
    I <- list(name= 'I', mass=127, isotope=list(mass=c(-0.095527),abundance=c(1)))
    In <- list(name= 'In', mass=113, isotope=list(mass=c(-0.095939, 0, -0.09612),abundance=c(0.043, 0, 0.957)))
    Ir <- list(name= 'Ir', mass=191, isotope=list(mass=c(-0.039416, 0, -0.037083),abundance=c(0.373, 0, 0.627)))
    K <- list(name= 'K', mass=39, isotope=list(mass=c(-0.0362926, -0.0360008, -0.0381746),abundance=c(0.932581, 0.000117, 0.067302)))
    Kr <- list(name= 'Kr', mass=78, isotope=list(mass=c(-0.079604, 0, -0.08362, 0, -0.086518, -0.085865, -0.088493, 0, -0.089384),abundance=c(0.0035, 0, 0.0225, 0, 0.116, 0.115, 0.57, 0, 0.173)))
    La <- list(name= 'La', mass=138, isotope=list(mass=c(-0.092895, -0.093653),abundance=c(0.000902, 0.999098)))
    Li <- list(name= 'Li', mass=6, isotope=list(mass=c(0.0151214, 0.016003),abundance=c(0.075, 0.925)))
    Lr <- list(name= 'Lr', mass=260, isotope=list(mass=c(0),abundance=c(1)))
    Lu <- list(name= 'Lu', mass=175, isotope=list(mass=c(-0.05923, -0.057321),abundance=c(0.9741, 0.0259)))
    Md <- list(name= 'Md', mass=258, isotope=list(mass=c(0.1),abundance=c(1)))
    Mg <- list(name= 'Mg', mass=24, isotope=list(mass=c(-0.0149577, -0.0141626, -0.0174063),abundance=c(0.7899, 0.1, 0.1101)))
    Mn <- list(name= 'Mn', mass=55, isotope=list(mass=c(-0.0619529),abundance=c(1)))
    Mo <- list(name= 'Mo', mass=92, isotope=list(mass=c(-0.093192, 0, -0.0949147, -0.0941589, -0.0953215, -0.0939795, -0.0945927, 0, -0.092523),abundance=c(0.1484, 0, 0.0925, 0.1592, 0.1668, 0.0955, 0.2413, 0, 0.0963)))
    N <- list(name= 'N', mass=14, isotope=list(mass=c(0.003074002, 0.00010897),abundance=c(0.99634, 0.00366)))
    Na <- list(name= 'Na', mass=23, isotope=list(mass=c(-0.0102323),abundance=c(1)))
    Nb <- list(name= 'Nb', mass=93, isotope=list(mass=c(-0.0936228),abundance=c(1)))
    Nd <- list(name= 'Nd', mass=142, isotope=list(mass=c(-0.092281, -0.09019, -0.089917, -0.08743, -0.086887, 0, -0.083111, 0, -0.079113),abundance=c(0.2713, 0.1218, 0.238, 0.083, 0.1719, 0, 0.0576, 0, 0.0564)))
    Ne <- list(name= 'Ne', mass=20, isotope=list(mass=c(-0.0075644, -0.0075644, -0.0086169),abundance=c(0.9048, 0.0027, 0.0925)))
    Ni <- list(name= 'Ni', mass=58, isotope=list(mass=c(-0.0646538, 0, -0.0692116, -0.0689421, -0.0716539, 0, -0.0720321),abundance=c(0.68077, 0, 0.26223, 0.0114, 0.03634, 0, 0.00926)))
    No <- list(name= 'No', mass=259, isotope=list(mass=c(0),abundance=c(1)))
    Np <- list(name= 'Np', mass=237, isotope=list(mass=c(0.0481678),abundance=c(1)))
    O <- list(name= 'O', mass=16, isotope=list(mass=c(-0.00508537, -0.0008688, -0.0008397),abundance=c(0.99762, 0.00038, 0.002)))
    Os <- list(name= 'Os', mass=184, isotope=list(mass=c(-0.047552, 0, -0.04617, -0.044259, -0.04414, -0.041863, -0.041564, 0, -0.038533),abundance=c(2e-04, 0, 0.0158, 0.016, 0.133, 0.161, 0.264, 0, 0.41)))
    P <- list(name= 'P', mass=31, isotope=list(mass=c(-0.026238),abundance=c(1)))
    Pa <- list(name= 'Pa', mass=231, isotope=list(mass=c(0.03588),abundance=c(1)))
    Pb <- list(name= 'Pb', mass=204, isotope=list(mass=c(-0.02698, 0, -0.02556, -0.024128, -0.023373),abundance=c(0.014, 0, 0.241, 0.221, 0.524)))
    Pd <- list(name= 'Pd', mass=102, isotope=list(mass=c(-0.094366, 0, -0.095971, -0.094921, -0.096522, 0, -0.096105, 0, -0.094833),abundance=c(0.0102, 0, 0.1114, 0.2233, 0.2733, 0, 0.2646, 0, 0.1172)))
    Pm <- list(name= 'Pm', mass=145, isotope=list(mass=c(-0.087257),abundance=c(1)))
    Po <- list(name= 'Po', mass=209, isotope=list(mass=c(-0.017596),abundance=c(1)))
    Pr <- list(name= 'Pr', mass=141, isotope=list(mass=c(-0.092353),abundance=c(1)))
    Pt <- list(name= 'Pt', mass=190, isotope=list(mass=c(-0.040083, 0, -0.038981, 0, -0.037345, -0.035234, -0.035074, 0, -0.032131),abundance=c(1e-04, 0, 0.0079, 0, 0.329, 0.338, 0.253, 0, 0.072)))
    Pu <- list(name= 'Pu', mass=244, isotope=list(mass=c(0.06),abundance=c(1)))
    Ra <- list(name= 'Ra', mass=226, isotope=list(mass=c(0.025402),abundance=c(1)))
    Rb <- list(name= 'Rb', mass=85, isotope=list(mass=c(-0.088206, 0, -0.090813),abundance=c(0.72165, 0, 0.27835)))
    Re <- list(name= 'Re', mass=185, isotope=list(mass=c(-0.047049, 0, -0.044256),abundance=c(0.374, 0, 0.626)))
    Rh <- list(name= 'Rh', mass=103, isotope=list(mass=c(-0.0945),abundance=c(1)))
    Rn <- list(name= 'Rn', mass=222, isotope=list(mass=c(0.01757),abundance=c(1)))
    Ru <- list(name= 'Ru', mass=96, isotope=list(mass=c(-0.092401, 0, -0.094713, -0.0940611, -0.0957808, -0.0944181, -0.0956515, 0, -0.094576),abundance=c(0.0552, 0, 0.0188, 0.127, 0.126, 0.17, 0.316, 0, 0.187)))
    S <- list(name= 'S', mass=32, isotope=list(mass=c(-0.0279293, -0.02854146, -0.03213335, 0, -0.03291938),abundance=c(0.9502, 0.0075, 0.0421, 0, 2e-04)))
    Sb <- list(name= 'Sb', mass=121, isotope=list(mass=c(-0.0961788, 0, -0.095784),abundance=c(0.5736, 0, 0.4264)))
    Sc <- list(name= 'Sc', mass=45, isotope=list(mass=c(-0.04409),abundance=c(1)))
    Se <- list(name= 'Se', mass=74, isotope=list(mass=c(-0.0775254, 0, -0.080788, -0.0800875, -0.0826924, 0, -0.0834804, 0, -0.0833022),abundance=c(0.0089, 0, 0.0936, 0.0763, 0.2378, 0, 0.4961, 0, 0.0873)))
    Si <- list(name= 'Si', mass=28, isotope=list(mass=c(-0.0230729, -0.0235051, -0.0262293),abundance=c(0.9223, 0.0467, 0.031)))
    Sm <- list(name= 'Sm', mass=144, isotope=list(mass=c(-0.088002, 0, 0, -0.085105, -0.085181, -0.082819, -0.082727, 0, -0.080271, 0, -0.077794),abundance=c(0.031, 0, 0, 0.15, 0.113, 0.138, 0.074, 0, 0.267, 0, 0.227)))
    Sn <- list(name= 'Sn', mass=112, isotope=list(mass=c(-0.095174, 0, -0.097216, -0.096652, -0.098253, -0.097044, -0.098391, -0.09669, -0.0978009, 0, -0.0965596, 0, -0.0947257),abundance=c(0.0097, 0, 0.0065, 0.0034, 0.1453, 0.0768, 0.2423, 0.0859, 0.3259, 0, 0.0463, 0, 0.0579)))
    Sr <- list(name= 'Sr', mass=84, isotope=list(mass=c(-0.08657, 0, -0.0907328, -0.0911159, -0.0943812),abundance=c(0.0056, 0, 0.0986, 0.07, 0.8258)))
    Ta <- list(name= 'Ta', mass=180, isotope=list(mass=c(-0.052538, -0.052008),abundance=c(0.00012, 0.99988)))
    Tb <- list(name= 'Tb', mass=159, isotope=list(mass=c(-0.074658),abundance=c(1)))
    Tc <- list(name= 'Tc', mass=98, isotope=list(mass=c(-0.0945927),abundance=c(1)))
    Te <- list(name= 'Te', mass=120, isotope=list(mass=c(-0.095952, 0, -0.096946, -0.095729, -0.097177, -0.095567, -0.096686, 0, -0.095537, 0, -0.093771),abundance=c(0.00096, 0, 0.02603, 0.00908, 0.04816, 0.07139, 0.1895, 0, 0.3169, 0, 0.338)))
    Th <- list(name= 'Th', mass=232, isotope=list(mass=c(0.038054),abundance=c(1)))
    Ti <- list(name= 'Ti', mass=46, isotope=list(mass=c(-0.0473706, -0.048236, -0.0520527, -0.0521289, -0.0552079),abundance=c(0.08, 0.073, 0.738, 0.055, 0.054)))
    Tl <- list(name= 'Tl', mass=203, isotope=list(mass=c(-0.02768, 0, -0.025599),abundance=c(0.29524, 0, 0.70476)))
    Tm <- list(name= 'Tm', mass=169, isotope=list(mass=c(-0.065788),abundance=c(1)))
    U <- list(name= 'U', mass=234, isotope=list(mass=c(0.0409468, 0.0439242, 0, 0, 0.0507847),abundance=c(5.5e-05, 0.0072, 0, 0, 0.992745)))
    V <- list(name= 'V', mass=50, isotope=list(mass=c(-0.0528391, -0.0560383),abundance=c(0.0025, 0.9975)))
    W <- list(name= 'W', mass=180, isotope=list(mass=c(-0.053299, 0, -0.051798, -0.04978, -0.049072, 0, -0.045643),abundance=c(0.0013, 0, 0.263, 0.143, 0.3067, 0, 0.286)))
    Xe <- list(name= 'Xe', mass=124, isotope=list(mass=c(-0.0941058, 0, -0.095719, 0, -0.0964688, -0.0952199, -0.0964906, -0.094928, -0.095856, 0, -0.094605, 0, -0.092786),abundance=c(0.001, 0, 9e-04, 0, 0.0191, 0.264, 0.041, 0.212, 0.269, 0, 0.104, 0, 0.089)))
    Y <- list(name= 'Y', mass=89, isotope=list(mass=c(-0.094151),abundance=c(1)))
    Yb <- list(name= 'Yb', mass=168, isotope=list(mass=c(-0.066106, 0, -0.065241, -0.063677, -0.063622, -0.061792, -0.061141, 0, -0.057436),abundance=c(0.0013, 0, 0.0305, 0.143, 0.219, 0.1612, 0.318, 0, 0.127)))
    Zn <- list(name= 'Zn', mass=64, isotope=list(mass=c(-0.0708552, 0, -0.0739653, -0.0728709, -0.0751541, 0, -0.074675),abundance=c(0.486, 0, 0.279, 0.041, 0.188, 0, 0.006)))
    Zr <- list(name= 'Zr', mass=90, isotope=list(mass=c(-0.0952974, -0.0943561, -0.0949614, 0, -0.0936852, 0, -0.091725),abundance=c(0.5145, 0.1122, 0.1715, 0, 0.1738, 0, 0.028)))
	
    list(D, Ac, Ag, Al, Am, Ar, As, At, Au, B, Ba, Be, Bi, Bk, Br, C, Ca, Cd, Ce, Cf, Cl, Cm, Co, Cr, Cs, Cu, Dy, Er, Es, Eu, F, Fe, Fm, Fr, Ga, Gd, Ge, H, He, Hf, Hg, Ho, I, In, Ir, K, Kr, La, Li, Lr, Lu, Md, Mg, Mn, Mo, N, Na, Nb, Nd, Ne, Ni, No, Np, O, Os, P, Pa, Pb, Pd, Pm, Po, Pr, Pt, Pu, Ra, Rb, Re, Rh, Rn, Ru, S, Sb, Sc, Se, Si, Sm, Sn, Sr, Ta, Tb, Tc, Te, Th, Ti, Tl, Tm, U, V, W, Xe, Y, Yb, Zn, Zr)
}

#' @noRd
#' @keywords internal
.initializePSE_NIST <- function() {
    D <- list(name="D", mass=2,  isotope = list(mass=c(0.01410178), abundance=c(1))) #Heavy Water
    Ac <- list(name='Ac', mass=227, isotope=list(mass=c(0.02775230), abundance=c(1.00000000)))
    Ag <- list(name='Ag', mass=107, isotope=list(mass=c(-0.09490840, 0.00000000, -0.09524470), abundance=c(0.51839000, 0.00000000, 0.48161000)))
    Al <- list(name='Al', mass=27, isotope=list(mass=c(-0.01846147), abundance=c(1.00000000)))
    Am <- list(name='Am', mass=243, isotope=list(mass=c(0.06138130), abundance=c(1.00000000)))
    Ar <- list(name='Ar', mass=36, isotope=list(mass=c(-0.03245490, 0.00000000, -0.03726789, 0.00000000, -0.03761688), abundance=c(0.00333600, 0.00000000, 0.00062900, 0.00000000, 0.99603500)))
    As <- list(name='As', mass=75, isotope=list(mass=c(-0.07840543), abundance=c(1.00000000)))
    At <- list(name='At', mass=210, isotope=list(mass=c(-0.01285210), abundance=c(1.00000000)))
    Au <- list(name='Au', mass=197, isotope=list(mass=c(-0.03343121), abundance=c(1.00000000)))
    B <- list(name='B', mass=10, isotope=list(mass=c(0.01293695, 0.00930536), abundance=c(0.19900000, 0.80100000)))
    Ba <- list(name='Ba', mass=130, isotope=list(mass=c(-0.09367930, 0.00000000, -0.09493890, 0.00000000, -0.09549182, -0.09431162, -0.09542427, -0.09417286, -0.09475300), abundance=c(0.00106000, 0.00000000, 0.00101000, 0.00000000, 0.02417000, 0.06592000, 0.07854000, 0.11232000, 0.71698000)))
    Be <- list(name='Be', mass=9, isotope=list(mass=c(0.01218307), abundance=c(1.00000000)))
    Bi <- list(name='Bi', mass=209, isotope=list(mass=c(-0.01960090), abundance=c(1.00000000)))
    Bk <- list(name='Bk', mass=247, isotope=list(mass=c(0.07030730), abundance=c(1.00000000)))
    Br <- list(name='Br', mass=79, isotope=list(mass=c(-0.08166240, 0.00000000, -0.08371030), abundance=c(0.50690000, 0.00000000, 0.49310000)))
    C <- list(name='C', mass=12, isotope=list(mass=c(0.00000000, 0.00335484, 0.00324199), abundance=c(0.98930000, 0.01070000, 0.00000000)))
    Ca <- list(name='Ca', mass=40, isotope=list(mass=c(-0.03740914, 0.00000000, -0.04138217, -0.04123356, -0.04451844, 0.00000000, -0.04631100, 0.00000000, -0.04747724), abundance=c(0.96941000, 0.00000000, 0.00647000, 0.00135000, 0.02086000, 0.00000000, 0.00004000, 0.00000000, 0.00187000)))
    Cd <- list(name='Cd', mass=106, isotope=list(mass=c(-0.09354010, 0.00000000, -0.09581660, 0.00000000, -0.09699339, -0.09581713, -0.09723713, -0.09559187, -0.09663491, 0.00000000, -0.09523685), abundance=c(0.01250000, 0.00000000, 0.00890000, 0.00000000, 0.12490000, 0.12800000, 0.24130000, 0.12220000, 0.28730000, 0.00000000, 0.07490000)))
    Ce <- list(name='Ce', mass=136, isotope=list(mass=c(-0.09287079, 0.00000000, -0.09400900, 0.00000000, -0.09455690, 0.00000000, -0.09074960), abundance=c(0.00185000, 0.00000000, 0.00251000, 0.00000000, 0.88450000, 0.00000000, 0.11114000)))
    Cf <- list(name='Cf', mass=251, isotope=list(mass=c(0.07958860), abundance=c(1.00000000)))
    Cl <- list(name='Cl', mass=35, isotope=list(mass=c(-0.03114732, 0.00000000, -0.03409740), abundance=c(0.75760000, 0.00000000, 0.24240000)))
    Cm <- list(name='Cm', mass=247, isotope=list(mass=c(0.07035410), abundance=c(1.00000000)))
    Co <- list(name='Co', mass=59, isotope=list(mass=c(-0.06680571), abundance=c(1.00000000)))
    Cr <- list(name='Cr', mass=50, isotope=list(mass=c(-0.05395817, 0.00000000, -0.05949377, -0.05935185, -0.06112084), abundance=c(0.04345000, 0.00000000, 0.83789000, 0.09501000, 0.02365000)))
    Cs <- list(name='Cs', mass=133, isotope=list(mass=c(-0.09454804), abundance=c(1.00000000)))
    Cu <- list(name='Cu', mass=63, isotope=list(mass=c(-0.07040228, 0.00000000, -0.07221030), abundance=c(0.69150000, 0.00000000, 0.30850000)))
    Dy <- list(name='Dy', mass=156, isotope=list(mass=c(-0.07571530, 0.00000000, -0.07558410, 0.00000000, -0.07479540, -0.07305950, -0.07319440, -0.07126170, -0.07081810), abundance=c(0.00056000, 0.00000000, 0.00095000, 0.00000000, 0.02329000, 0.18889000, 0.25475000, 0.24896000, 0.28260000)))
    Er <- list(name='Er', mass=162, isotope=list(mass=c(-0.07121160, 0.00000000, -0.07079120, 0.00000000, -0.06970050, -0.06794540, -0.06762330, 0.00000000, -0.06452980), abundance=c(0.00139000, 0.00000000, 0.01601000, 0.00000000, 0.33503000, 0.22869000, 0.26978000, 0.00000000, 0.14910000)))
    Es <- list(name='Es', mass=252, isotope=list(mass=c(0.08298000), abundance=c(1.00000000)))
    Eu <- list(name='Eu', mass=151, isotope=list(mass=c(-0.08014220, 0.00000000, -0.07876200), abundance=c(0.47810000, 0.00000000, 0.52190000)))
    F <- list(name='F', mass=19, isotope=list(mass=c(-0.00159684), abundance=c(1.00000000)))
    Fe <- list(name='Fe', mass=54, isotope=list(mass=c(-0.06039101, 0.00000000, -0.06506367, -0.06460716, -0.06672557), abundance=c(0.05845000, 0.00000000, 0.91754000, 0.02119000, 0.00282000)))
    Fm <- list(name='Fm', mass=257, isotope=list(mass=c(0.09510610), abundance=c(1.00000000)))
    Fr <- list(name='Fr', mass=223, isotope=list(mass=c(0.01973600), abundance=c(1.00000000)))
    Ga <- list(name='Ga', mass=69, isotope=list(mass=c(-0.07442650, 0.00000000, -0.07529742), abundance=c(0.60108000, 0.00000000, 0.39892000)))
    Gd <- list(name='Gd', mass=152, isotope=list(mass=c(-0.08020050, 0.00000000, -0.07912590, -0.07736950, -0.07786880, -0.07603140, -0.07588770, 0.00000000, -0.07293760), abundance=c(0.00200000, 0.00000000, 0.02180000, 0.14800000, 0.20470000, 0.15650000, 0.24840000, 0.00000000, 0.21860000)))
    Ge <- list(name='Ge', mass=70, isotope=list(mass=c(-0.07575125, 0.00000000, -0.07792417, -0.07654104, -0.07882224, 0.00000000, -0.07859727), abundance=c(0.20570000, 0.00000000, 0.27450000, 0.07750000, 0.36500000, 0.00000000, 0.07730000)))
    H <- list(name='H', mass=1, isotope=list(mass=c(0.00782503, 0.01410178, 0.01604928), abundance=c(0.99988500, 0.00011500, 0.00000000)))
    He <- list(name='He', mass=3, isotope=list(mass=c(0.01602932, 0.00260325), abundance=c(0.00000134, 0.99999866)))
    Hf <- list(name='Hf', mass=174, isotope=list(mass=c(-0.05995390, 0.00000000, -0.05859240, -0.05677230, -0.05629420, -0.05417680, -0.05344300), abundance=c(0.00160000, 0.00000000, 0.05260000, 0.18600000, 0.27280000, 0.13620000, 0.35080000)))
    Hg <- list(name='Hg', mass=196, isotope=list(mass=c(-0.03416740, 0.00000000, -0.03323140, -0.03171936, -0.03167341, -0.02969716, -0.02935660, 0.00000000, -0.02650602), abundance=c(0.00150000, 0.00000000, 0.09970000, 0.16870000, 0.23100000, 0.13180000, 0.29860000, 0.00000000, 0.06870000)))
    Ho <- list(name='Ho', mass=165, isotope=list(mass=c(-0.06967120), abundance=c(1.00000000)))
    I <- list(name='I', mass=127, isotope=list(mass=c(-0.09552810), abundance=c(1.00000000)))
    In <- list(name='In', mass=113, isotope=list(mass=c(-0.09593816, 0.00000000, -0.09612122), abundance=c(0.04290000, 0.00000000, 0.95710000)))
    Ir <- list(name='Ir', mass=191, isotope=list(mass=c(-0.03941070, 0.00000000, -0.03707840), abundance=c(0.37300000, 0.00000000, 0.62700000)))
    K <- list(name='K', mass=39, isotope=list(mass=c(-0.03629351, -0.03600183, -0.03817474), abundance=c(0.93258100, 0.00011700, 0.06730200)))
    Kr <- list(name='Kr', mass=78, isotope=list(mass=c(-0.07963506, 0.00000000, -0.08362192, 0.00000000, -0.08651727, -0.08587284, -0.08850227, 0.00000000, -0.08938937), abundance=c(0.00355000, 0.00000000, 0.02286000, 0.00000000, 0.11593000, 0.11500000, 0.56987000, 0.00000000, 0.17279000)))
    La <- list(name='La', mass=138, isotope=list(mass=c(-0.09288510, -0.09364370), abundance=c(0.00088810, 0.99911190)))
    Li <- list(name='Li', mass=6, isotope=list(mass=c(0.01512289, 0.01600344), abundance=c(0.07590000, 0.92410000)))
    Lr <- list(name='Lr', mass=262, isotope=list(mass=c(0.10961000), abundance=c(1.00000000)))
    Lu <- list(name='Lu', mass=175, isotope=list(mass=c(-0.05922480, -0.05731030), abundance=c(0.97401000, 0.02599000)))
    Md <- list(name='Md', mass=258, isotope=list(mass=c(0.09843150), abundance=c(1.00000000)))
    Mg <- list(name='Mg', mass=24, isotope=list(mass=c(-0.01495830, -0.01416302, -0.01740703), abundance=c(0.78990000, 0.10000000, 0.11010000)))
    Mn <- list(name='Mn', mass=55, isotope=list(mass=c(-0.06195609), abundance=c(1.00000000)))
    Mo <- list(name='Mo', mass=92, isotope=list(mass=c(-0.09319204, 0.00000000, -0.09491510, -0.09416123, -0.09532388, -0.09398188, -0.09459518, 0.00000000, -0.09252820), abundance=c(0.14530000, 0.00000000, 0.09150000, 0.15840000, 0.16670000, 0.09600000, 0.24390000, 0.00000000, 0.09820000)))
    N <- list(name='N', mass=14, isotope=list(mass=c(0.00307400, 0.00010890), abundance=c(0.99636000, 0.00364000)))
    Na <- list(name='Na', mass=23, isotope=list(mass=c(-0.01023072), abundance=c(1.00000000)))
    Nb <- list(name='Nb', mass=93, isotope=list(mass=c(-0.09362700), abundance=c(1.00000000)))
    Nd <- list(name='Nd', mass=142, isotope=list(mass=c(-0.09227100, -0.09018000, -0.08990700, -0.08742070, -0.08687740, 0.00000000, -0.08310070, 0.00000000, -0.07909780), abundance=c(0.27152000, 0.12174000, 0.23798000, 0.08293000, 0.17189000, 0.00000000, 0.05756000, 0.00000000, 0.05638000)))
    Ne <- list(name='Ne', mass=20, isotope=list(mass=c(-0.00755982, -0.00615331, -0.00861489), abundance=c(0.90480000, 0.00270000, 0.09250000)))
    Ni <- list(name='Ni', mass=58, isotope=list(mass=c(-0.06465759, 0.00000000, -0.06921412, -0.06894443, -0.07165463, 0.00000000, -0.07203318), abundance=c(0.68077000, 0.00000000, 0.26223100, 0.01139900, 0.03634600, 0.00000000, 0.00925500)))
    No <- list(name='No', mass=259, isotope=list(mass=c(0.10103000), abundance=c(1.00000000)))
    Np <- list(name='Np', mass=237, isotope=list(mass=c(0.04817360), abundance=c(1.00000000)))
    O <- list(name='O', mass=16, isotope=list(mass=c(-0.00508538, -0.00086824, -0.00084039), abundance=c(0.99757000, 0.00038000, 0.00205000)))
    Os <- list(name='Os', mass=184, isotope=list(mass=c(-0.04751150, 0.00000000, -0.04616500, -0.04425260, -0.04416480, -0.04185580, -0.04155630, 0.00000000, -0.03852300), abundance=c(0.00020000, 0.00000000, 0.01590000, 0.01960000, 0.13240000, 0.16150000, 0.26260000, 0.00000000, 0.40780000)))
    P <- list(name='P', mass=31, isotope=list(mass=c(-0.02623800), abundance=c(1.00000000)))
    Pa <- list(name='Pa', mass=231, isotope=list(mass=c(0.03588420), abundance=c(1.00000000)))
    Pb <- list(name='Pb', mass=204, isotope=list(mass=c(-0.02695600, 0.00000000, -0.02553430, -0.02410270, -0.02334750), abundance=c(0.01400000, 0.00000000, 0.24100000, 0.22100000, 0.52400000)))
    Pd <- list(name='Pd', mass=102, isotope=list(mass=c(-0.09439780, 0.00000000, -0.09596950, -0.09492040, -0.09651960, 0.00000000, -0.09610840, 0.00000000, -0.09482780), abundance=c(0.01020000, 0.00000000, 0.11140000, 0.22330000, 0.27330000, 0.00000000, 0.26460000, 0.00000000, 0.11720000)))
    Pm <- list(name='Pm', mass=145, isotope=list(mass=c(-0.08724410), abundance=c(1.00000000)))
    Po <- list(name='Po', mass=209, isotope=list(mass=c(-0.01756920), abundance=c(1.00000000)))
    Pr <- list(name='Pr', mass=141, isotope=list(mass=c(-0.09234240), abundance=c(1.00000000)))
    Pt <- list(name='Pt', mass=190, isotope=list(mass=c(-0.04007030, 0.00000000, -0.03896130, 0.00000000, -0.03731910, -0.03520830, -0.03504791, 0.00000000, -0.03210510), abundance=c(0.00012000, 0.00000000, 0.00782000, 0.00000000, 0.32860000, 0.33780000, 0.25210000, 0.00000000, 0.07356000)))
    Pu <- list(name='Pu', mass=244, isotope=list(mass=c(0.06420530), abundance=c(1.00000000)))
    Ra <- list(name='Ra', mass=226, isotope=list(mass=c(0.02541030), abundance=c(1.00000000)))
    Rb <- list(name='Rb', mass=85, isotope=list(mass=c(-0.08821026, 0.00000000, -0.09081947), abundance=c(0.72170000, 0.00000000, 0.27830000)))
    Re <- list(name='Re', mass=185, isotope=list(mass=c(-0.04704550, 0.00000000, -0.04424990), abundance=c(0.37400000, 0.00000000, 0.62600000)))
    Rh <- list(name='Rh', mass=103, isotope=list(mass=c(-0.09450200), abundance=c(1.00000000)))
    Rn <- list(name='Rn', mass=222, isotope=list(mass=c(0.01757820), abundance=c(1.00000000)))
    Ru <- list(name='Ru', mass=96, isotope=list(mass=c(-0.09240975, 0.00000000, -0.09471320, -0.09406590, -0.09578570, -0.09442310, -0.09565590, 0.00000000, -0.09457250), abundance=c(0.05540000, 0.00000000, 0.01870000, 0.12760000, 0.12600000, 0.17060000, 0.31550000, 0.00000000, 0.18620000)))
    S <- list(name='S', mass=32, isotope=list(mass=c(-0.02792883, -0.02854109, -0.03213300, 0.00000000, -0.03291929), abundance=c(0.94990000, 0.00750000, 0.04250000, 0.00000000, 0.00010000)))
    Sb <- list(name='Sb', mass=121, isotope=list(mass=c(-0.09618800, 0.00000000, -0.09578680), abundance=c(0.57210000, 0.00000000, 0.42790000)))
    Sc <- list(name='Sc', mass=45, isotope=list(mass=c(-0.04409172), abundance=c(1.00000000)))
    Se <- list(name='Se', mass=74, isotope=list(mass=c(-0.07752407, 0.00000000, -0.08078630, -0.08008585, -0.08269072, 0.00000000, -0.08347820, 0.00000000, -0.08330050), abundance=c(0.00890000, 0.00000000, 0.09370000, 0.07630000, 0.23770000, 0.00000000, 0.49610000, 0.00000000, 0.08730000)))
    Si <- list(name='Si', mass=28, isotope=list(mass=c(-0.02307347, -0.02350534, -0.02622986), abundance=c(0.92223000, 0.04685000, 0.03092000)))
    Sm <- list(name='Sm', mass=144, isotope=list(mass=c(-0.08799350, 0.00000000, 0.00000000, -0.08509560, -0.08517080, -0.08280790, -0.08271710, 0.00000000, -0.08026030, 0.00000000, -0.07778310), abundance=c(0.03070000, 0.00000000, 0.00000000, 0.14990000, 0.11240000, 0.13820000, 0.07380000, 0.00000000, 0.26750000, 0.00000000, 0.22750000)))
    Sn <- list(name='Sn', mass=112, isotope=list(mass=c(-0.09517613, 0.00000000, -0.09721730, -0.09665530, -0.09825720, -0.09704602, -0.09839343, -0.09668883, -0.09779837, 0.00000000, -0.09655620, 0.00000000, -0.09472340), abundance=c(0.00970000, 0.00000000, 0.00660000, 0.00340000, 0.14540000, 0.07680000, 0.24220000, 0.08590000, 0.32580000, 0.00000000, 0.04630000, 0.00000000, 0.05790000)))
    Sr <- list(name='Sr', mass=84, isotope=list(mass=c(-0.08658090, 0.00000000, -0.09073940, -0.09112250, -0.09438750), abundance=c(0.00560000, 0.00000000, 0.09860000, 0.07000000, 0.82580000)))
    Ta <- list(name='Ta', mass=180, isotope=list(mass=c(-0.05253520, -0.05200420), abundance=c(0.00012010, 0.99987990)))
    Tb <- list(name='Tb', mass=159, isotope=list(mass=c(-0.07464530), abundance=c(1.00000000)))
    Tc <- list(name='Tc', mass=98, isotope=list(mass=c(-0.09278760), abundance=c(1.00000000)))
    Te <- list(name='Te', mass=120, isotope=list(mass=c(-0.09594070, 0.00000000, -0.09695650, -0.09573020, -0.09718290, -0.09557010, -0.09668910, 0.00000000, -0.09553872, 0.00000000, -0.09377725), abundance=c(0.00090000, 0.00000000, 0.02550000, 0.00890000, 0.04740000, 0.07070000, 0.18840000, 0.00000000, 0.31740000, 0.00000000, 0.34080000)))
    Th <- list(name='Th', mass=232, isotope=list(mass=c(0.03805580), abundance=c(1.00000000)))
    Ti <- list(name='Ti', mass=46, isotope=list(mass=c(-0.04737228, -0.04824121, -0.05205802, -0.05213432, -0.05521311), abundance=c(0.08250000, 0.07440000, 0.73720000, 0.05410000, 0.05180000)))
    Tl <- list(name='Tl', mass=203, isotope=list(mass=c(-0.02765540, 0.00000000, -0.02557220), abundance=c(0.29520000, 0.00000000, 0.70480000)))
    Tm <- list(name='Tm', mass=169, isotope=list(mass=c(-0.06578210), abundance=c(1.00000000)))
    U <- list(name='U', mass=234, isotope=list(mass=c(0.04095230, 0.04393010, 0.00000000, 0.00000000, 0.05078840), abundance=c(0.00005400, 0.00720400, 0.00000000, 0.00000000, 0.99274200)))
    V <- list(name='V', mass=50, isotope=list(mass=c(-0.05284399, -0.05604296), abundance=c(0.00250000, 0.99750000)))
    W <- list(name='W', mass=180, isotope=list(mass=c(-0.05328920, 0.00000000, -0.05179606, -0.04977725, -0.04906908, 0.00000000, -0.04563720), abundance=c(0.00120000, 0.00000000, 0.26500000, 0.14310000, 0.30640000, 0.00000000, 0.28430000)))
    Xe <- list(name='Xe', mass=124, isotope=list(mass=c(-0.09410800, 0.00000000, -0.09570170, 0.00000000, -0.09646900, -0.09521914, -0.09649065, -0.09491594, -0.09584491, 0.00000000, -0.09460534, 0.00000000, -0.09278552), abundance=c(0.00095200, 0.00000000, 0.00089000, 0.00000000, 0.01910200, 0.26400600, 0.04071000, 0.21232400, 0.26908600, 0.00000000, 0.10435700, 0.00000000, 0.08857300)))
    Y <- list(name='Y', mass=89, isotope=list(mass=c(-0.09415970), abundance=c(1.00000000)))
    Yb <- list(name='Yb', mass=168, isotope=list(mass=c(-0.06611040, 0.00000000, -0.06523360, -0.06366980, -0.06361410, -0.06178490, -0.06113360, 0.00000000, -0.05742360), abundance=c(0.00123000, 0.00000000, 0.02982000, 0.14090000, 0.21680000, 0.16103000, 0.32026000, 0.00000000, 0.12996000)))
    Zn <- list(name='Zn', mass=64, isotope=list(mass=c(-0.07085799, 0.00000000, -0.07396619, -0.07287225, -0.07515545, 0.00000000, -0.07468080), abundance=c(0.49170000, 0.00000000, 0.27730000, 0.04040000, 0.18450000, 0.00000000, 0.00610000)))
    Zr <- list(name='Zr', mass=90, isotope=list(mass=c(-0.09530230, -0.09436040, -0.09496530, 0.00000000, -0.09368920, 0.00000000, -0.09172860), abundance=c(0.51450000, 0.11220000, 0.17150000, 0.00000000, 0.17380000, 0.00000000, 0.02800000)))
	
    list(D, Ac, Ag, Al, Am, Ar, As, At, Au, B, Ba, Be, Bi, Bk, Br, C, Ca, Cd, Ce, Cf, Cl, Cm, Co, Cr, Cs, Cu, Dy, Er, Es, Eu, F, Fe, Fm, Fr, Ga, Gd, Ge, H, He, Hf, Hg, Ho, I, In, Ir, K, Kr, La, Li, Lr, Lu, Md, Mg, Mn, Mo, N, Na, Nb, Nd, Ne, Ni, No, Np, O, Os, P, Pa, Pb, Pd, Pm, Po, Pr, Pt, Pu, Ra, Rb, Re, Rh, Rn, Ru, S, Sb, Sc, Se, Si, Sm, Sn, Sr, Ta, Tb, Tc, Te, Th, Ti, Tl, Tm, U, V, W, Xe, Y, Yb, Zn, Zr)
}


#' @rdname initializeElements
#' @export
initializeCharges <- function() {
    positive <- list(name="+", mass=0, isotope = list(mass=c(-0.00054858), abundance=c(1)))
    negative <- list(name="-", mass=0, isotope = list(mass=c(+0.00054858), abundance=c(1)))
    list(positive, negative)
}
