RcppVersion <- function() {
  licenseFile <- file(system.file(".","Rcpp-license.txt",package="RcppTemplate"),"r")
  writeLines(readLines(licenseFile))
}
