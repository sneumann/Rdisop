
test.cid46173807 <- function() {
  checkEqualsNumeric(852.354928,
                     getMolecule("C49H56FeN4O6",initializePSE(),z=0)$exactmass) 

}

test.cid24892761 <- function() {
  checkEqualsNumeric(447.089515,
                     getMolecule("C15H23N6O5Se+",c(initializePSE(),initializeCharges()),z=1)$exactmass) 

}

## test.cid <- function() {
##   checkEqualsNumeric(,
##                      getMolecule("",elem,z=0)$exactmass) 
## }


