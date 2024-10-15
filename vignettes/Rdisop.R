## ----include = FALSE----------------------------------------------------------
knitr::opts_chunk$set(
  collapse = TRUE,
  comment = "#>"
)

## ----setup--------------------------------------------------------------------
library(Rdisop)

## ----exmpl1-------------------------------------------------------------------
molecule <- getMolecule("C2H5OH")
getFormula(molecule)
getMass(molecule)

## ----exmpl2-------------------------------------------------------------------
essentialElements <- initializeCHNOPSMgKCaFe()
chlorophyll <- getMolecule("C55H72MgN4O5H", z=1, elements=essentialElements)
isotopes <- getIsotope(chlorophyll, seq(1,4))
isotopes

## ----fig1, fig.cap="Isotope pattern for a protonated chlorophyll ion, which could be observed on a high-resolution mass spectrometer in positive mode."----
plot(t(isotopes), type="h", xlab="m/z", ylab="Intensity")

## ----exmpl3-------------------------------------------------------------------
molecules <- decomposeMass(46.042, ppm=20, maxisotopes = 4)
molecules

## ----exmpl4-------------------------------------------------------------------
length(decomposeMass(147.053))

## ----exmpl5-------------------------------------------------------------------
# glutamic acid (C5H9NO4)
masses <- c(147.053, 148.056)
intensities <- c(93, 5.8)

molecules <- decomposeIsotopes(masses, intensities)
data.frame(getFormula(molecules), getScore(molecules), getValid(molecules))

## ----exmpl6-------------------------------------------------------------------
getFormula(subMolecules("C5H10NO4", "H"))

