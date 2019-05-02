
#default settings for minElements and maxElements
test.minmaxElements1 <- function() {

  elem <-  initializeElements(c("C","H","N","O","P"))
  testmass <- 269.2431
  
  res <- decomposeMass(testmass, ppm = 5, mzabs = 0, elements = elem)   
  
  checkEquals(res$formula[order(abs(res$exactmass - testmass))],c("C6H34N6O3P",
                                                                        "C2H37N7O3P2",
                                                                        "C3H43O8P2",
                                                                        "C10H31N5O3",
                                                                        "C3H48NOP5",
                                                                        "C12H33N2O4",
                                                                        "C7H45OP4",
                                                                        "CH41N3O7P2",
                                                                        "H35N10O2P2",
                                                                        "C8H36N3O4P"))
}

#must filter out formulas that do not contain C
test.minmaxElements2 <- function() {
  
  elem <-  initializeElements(c("C","H","N","O","P"))
  testmass <- 269.2431
  
  res <- decomposeMass(testmass, ppm = 5, mzabs = 0, elements = elem, minElements = "C1")   
  
  checkEquals(res$formula[order(abs(res$exactmass - testmass))],c("C6H34N6O3P",
                                                                        "C2H37N7O3P2",
                                                                        "C3H43O8P2",
                                                                        "C10H31N5O3",
                                                                        "C3H48NOP5",
                                                                        "C12H33N2O4",
                                                                        "C7H45OP4",
                                                                        "CH41N3O7P2",
                                                                        "C8H36N3O4P"))
}

#set maxElements
test.minmaxElements3 <- function() {
  
  elem <-  initializeElements(c("C","H","N","O","P"))
  testmass <- 269.2431
  
  res <- decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C1", maxElements = "C6")   
  
  checkEquals(res$formula[order(abs(res$exactmass - testmass))],c("C6H34N6O3P",
                                                                       "C2H37N7O3P2",
                                                                       "C3H43O8P2",
                                                                       "C3H48NOP5",
                                                                       "CH41N3O7P2"))
}


#equal minElements and maxElements
test.minmaxElements4 <- function() {
  
  elem <-  initializeElements(c("C","H","N","O","P"))
  testmass <- 269.2431
  
  res <- decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C6", maxElements = "C6")   
  
  checkEquals(res$formula[order(abs(res$exactmass - testmass))],c("C6H34N6O3P"
                                                                 ))
}

#maxElements > minElements
test.minmaxElements5 <- function() {
  
  elem <-  initializeElements(c("C","H","N","O","P"))
  testmass <- 269.2431
  
  res <- decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C6", maxElements = "C5")   
  
  checkTrue(is.null(res))
}


#Zeros in maxElements are ignored and do not cause an error
test.minmaxElements6 <- function() {
  
  elem <-  initializeElements(c("C","H","N","O","P"))
  testmass <- 269.2431
  
  res <- decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C6", maxElements = "P0")   
  
  checkEquals(res$formula[order(abs(res$exactmass - testmass))],c("C6H34N6O3P", "C10H31N5O3", "C12H33N2O4", "C7H45OP4", "C8H36N3O4P"))
}

#more complex exammple:
test.minmaxElements7 <- function() {
  
  elem <- initializeCHNOPS()       
  testmass <- 347.0630844422
  
  res <- decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C10O6PN", maxElements = "O10P1S3")   
  
  checkEquals(res$formula[order(abs(res$exactmass - testmass))],c("C10H14N5O7P",  "C10H22NO6PS2", "C12H16N2O8P"))
}



