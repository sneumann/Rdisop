\name{addMolecules}
\alias{addMolecules}
\alias{subMolecules}

\title{Add/subtract sum formulae}
\description{
  Simple arithmetic modifications of sum formulae.
}
\usage{
addMolecules(formula1, formula2, elements = NULL, maxisotopes = 10)
subMolecules(formula1, formula2, elements = NULL, maxisotopes = 10)
}
\arguments{
  \item{formula1}{Sum formula}
  \item{formula2}{Sum formula}
  \item{elements}{list of allowed chemical elements, defaults to full
    periodic system of elements}   
  \item{maxisotopes}{maximum number of isotopes shown in the resulting
    molecules}
}

\details{
  addMolecules() adds the second argument to the first.
  subMolecules() subtracts the second argument from the first.

  This can be useful to revert
  e.g. adduct/fragment formation found in ESI mass spectrometry,
  or to mimick simple chemical reactions. No chemical checks are
  performed.
}

\value{
    A list with the elements
      \item{formula}{repeated sum formula}
      \item{mass}{exact monoisotopic mass of molecule}      
      \item{score}{dummy value, always 1.0}      
      \item{isotopes}{a list of isotopes}
    
}

\examples{
# For proton-Adduct of Ethanol:
subMolecules("C2H7O", "H")
}

\author{Steffen Neumann <sneumann@IPB-Halle.DE>}
\keyword{methods}
