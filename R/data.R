#' @title isotope information.
#' @docType data
#' @format A data frame of 4 columns for 398 chemical isotopes.
#' \describe{
#' \item{element}{The element name.}
#' \item{isotope}{The isotope name.}
#' \item{mass}{The absolute mass of this isotope in Dalton.}
#' \item{abundance}{The absolute abundance of this isotope.}
#' }
#' @source Isotope patterns were obtained through wikipedia.org.
"isotopes"

#' @title Monoisotopic mass information..
#' @docType data
#' @format A named vector containing for each chemical element the most abundant isotope mass.
#' @source Isotope patterns were obtained through wikipedia.org.
"mono_masses"
