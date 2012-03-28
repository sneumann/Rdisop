\name{getMolecule}
\alias{getMolecule}
\alias{getMass}
\alias{getFormula}
\alias{getIsotope}
\alias{getScore}
\alias{getValid}

\title{Calculate mass and isotope information for a molecule given as sum formula}
\description{
  Parse the sum formula and calculate the theoretical exact mass and
  the isotope distribution.
}
\usage{
getMolecule(formula, elements = NULL, z = 0, maxisotopes = 10)
getMass(molecule)
getFormula(molecule)
getIsotope(molecule, index)
getScore(molecule)
getValid(molecule)
}
\arguments{
  \item{formula}{Sum formula}
  \item{elements}{list of allowed chemical elements, defaults to full
    periodic system of elements}   
  \item{z}{charge z of molecule for exact mass calculation}
  \item{maxisotopes}{maximum number of isotopes shown in the resulting
    molecules}
  \item{molecule}{an initialized molecule as returned by
    getMolecule() or the decomposeMass() and decomposeIsotope() functions}
  \item{index}{return the n-th isotope mass/abundance pair of the molecule}
}

\details{
  getMolecule() Parse the sum formula and calculate the theoretical
  exact monoisotopic mass and
  the isotope distribution. For a given element, return the different
  mass values. 
}
\value{
    getMolecule: A list with the elements
      \item{formula}{repeated sum formula}
      \item{mass}{exact monoisotopic mass of molecule}
      \item{score}{probability, for given molecules a dummy value which
      is always 1.0}
      \item{valid}{result of neutrogen rule check}
      \item{isotopes}{a list of isotopes}
    
      getMass, getFormula and getScore: return the mass of the
      molecule as string or real value} 

\examples{
# For Ethanol:
getMolecule("C2H6O")
}

\references{
  For a description of the underlying IMS see:
  see citation("Rdisop")
}

\author{Steffen Neumann <sneumann@IPB-Halle.DE>}
\keyword{methods}
