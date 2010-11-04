
test.monoisotopicMasses <- function() {
  elem <- initializePSE()
  all(sapply(elem, function(x) {x$mass == as.integer(getMolecule(x$name, elem)$exactmass)}))
  
}



