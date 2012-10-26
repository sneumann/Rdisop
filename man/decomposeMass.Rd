\name{decomposeIsotopes}
\alias{decomposeMass}
\alias{decomposeIsotopes}
\alias{isotopeScore}
\title{Mass Decomposition of Isotope Patterns}
\description{
  Calculate the elementary compositions from an exact Mass or Isotope Pattern,
  obtained e.g.\ by FTICR or TOF mass spectrometers 
}
\usage{
decomposeMass(mass, ppm=2.0, mzabs=0.0001, elements=NULL, filter=NULL,
z=0, maxisotopes = 10, minElements="C0", maxElements="C999999")
decomposeIsotopes(masses, intensities, ppm=2.0, mzabs=0.0001,
elements=NULL, filter=NULL,  z=0, maxisotopes = 10, minElements="C0", maxElements="C999999")
isotopeScore(molecule, masses, intensities, elements = NULL, filter = NULL, z = 0)
}
\arguments{
  \item{mass}{A single exact mass (or m/z value)}
  \item{masses}{A vector of masses (or m/z values) of an isotope cluster}
  \item{intensities}{Abolute or relative intensities of the \code{masses} peaks}
  \item{ppm}{allowed deviation of hypotheses from given mass}
  \item{mzabs}{absolute deviation in dalton (mzabs and ppm will be added)}
  \item{z}{charge z of m/z peaks for calculation of real mass. 0 is for
    auto-detection}
  \item{maxisotopes}{maximum number of isotopes shown in the resulting
    molecules}
  \item{elements}{list of allowed chemical elements, defaults to CHNOPS}
  \item{minElements, maxElements}{Molecular formulas, which contain
    lower and upper boundaries of allowed formula respectively}
  \item{filter}{NYI, will be a selection of DU, DBE and Nitrogen rules}
  \item{molecule}{a molecule as obtained from getMolecule() or
    decomposeMass / decomposeIsotopes}
}
  
\details{
  Sum formulas are calculated which explain the given mass or isotope
  pattern.
}
\value{
  A list of molecules, which contain the sub-lists
   \item{formula}{
    potential formulae
}
   \item{mass}{
    exact monoisotopic mass of hypothesis
  }

     \item{score}{
    calculated score
  }

       \item{isotopes}{
	 a list of isotopes 
  }

}

\examples{
# For Glutamate: 
decomposeIsotopes(c(147.0529,148.0563), c(100.0,5.561173))
}

\references{
  For a description of the underlying IMS see:
  see citation("Rdisop")
}

\author{Steffen Neumann <sneumann@IPB-Halle.DE>}
\seealso{\code{\link{decomposeMass}}}
\keyword{methods}
