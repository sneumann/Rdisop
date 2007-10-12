\name{addMolecules}
\alias{addMolecules}
\alias{subMolecules}

\title{Add/subtract sum formulae}
\description{
  Parse the sum formula and calculate the theoretical exacat mass and
  the isotope distribution.
}
\usage{
addMolecules(formula1, formula2, elements = NULL)
subMolecules(formula1, formula2, elements = NULL)
}
\arguments{
  \item{formula1}{Sum formula}
  \item{formula2}{Sum formula}
  \item{elements}{list of allowed chemical elements, defaults to full
    periodic system of elements}   
}

\details{
  Subtracts first from second molecule. This can be useful to revert
  e.g. adduct/fragment formation found in ESI mass spectrometry,
  or to mimick simple chemical reactions. No chemical checks are
  performed.
}

\value{
    \item{A list with the elements
      \item{formula}{
	repeated sum formula
      }
      \item{mass}{
	exact mass of molecule
      }      
      \item{score}{
	dummy value, always 1.0
      }      
      \item{isotopes}{
	a list of isotopes 
      }
    }
}

\examples{
# For proton-Adduct of Ethanol:
subMolecules("C2H7O", "H")
}

\author{Steffen Neumann <sneumann@IPB-Halle.DE>}
\keyword{methods}
