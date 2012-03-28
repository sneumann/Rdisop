\name{initializeCHNOPS}
\alias{initializeElements}
\alias{initializeCHNOPS}
\alias{initializeCHNOPSMgKCaFe}
\alias{initializeCHNOPSNaK}
\alias{initializeCharges}
\alias{initializePSE}

\title{Initialize (a subset of) elements of the periodic system of
  elements (PSE)}

\description{
  Initialize the information about name, mass and isotopes.
  To reduce the number of decomposition hypotheses,
  subsets of elements can be created.
}

\usage{
initializeCHNOPS()       
initializeCHNOPSMgKCaFe()
initializePSE()       
initializeElements(names)     
}
\arguments{
  \item{names}{vector of element names within PSE}
}

\details{
  These functions return full, pre-defined or user-defined (sub-) lists of elements.
}
\value{
  A list with the elements
   \item{name}{
    repeated sum formula
}
   \item{mass}{
    nominal mass of molecule
  }

       \item{isotope}{
	 a list of isotopes 
  }
  The initializeCharges() is special, since it allows to parse charges
	 such as \code{getMolecule("H3O+", elements=c(initializeCHNOPS(),initializeCharges()))}
}

\examples{
# For Ethanol:
elements <- initializeCHNOPS()
}

\references{
  For a description of the underlying IMS see:
  see citation("Rdisop")

  Isotope patterns obtained through wikipedia.org
}

\author{Steffen Neumann <sneumann@IPB-Halle.DE>}
\seealso{\code{\link{getMolecule}}}
\keyword{methods}
