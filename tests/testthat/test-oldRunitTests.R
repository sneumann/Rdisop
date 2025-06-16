testthat::test_that(
  desc = "Implementation of original Runit tests from file runit.addsub", 
  code = {
      testthat::expect_equal(Rdisop::addMolecules("H", "H")$formula, "H2")
      testthat::expect_equal(Rdisop::addMolecules("C2H6O", "C2H6O")$exactmass, 2*46.04, tolerance=0.1)   
      testthat::expect_equal(Rdisop::subMolecules("C5H10NO4", "H")$formula, "C5H9NO4")
      testthat::expect_equal(Rdisop::subMolecules("C14H23N4O8P2S", "C3H4O3")$formula, "C11H19N4O5P2S")
      testthat::expect_equal(Rdisop::subMolecules("CH", "H")$formula, "C")
  }
)

testthat::test_that(
    desc = "Implementation of original Runit tests from file runit.minmaxElements", 
    code = {
        testthat::expect_equal(Rdisop::addMolecules("H", "H")$formula, "H2")
        
        # using default settings for minElements and maxElements
        elem <-  Rdisop::initializeElements(c("C","H","N","O","P"))
        testmass <- 269.2431
        res <- Rdisop::decomposeMass(testmass, ppm = 5, mzabs = 0, elements = elem)   
        exp_out <- c("C6H34N6O3P","C2H37N7O3P2","C3H43O8P2","C10H31N5O3","C3H48NOP5","C12H33N2O4","C7H45OP4","CH41N3O7P2","H35N10O2P2","C8H36N3O4P")
        testthat::expect_equal(res$formula[order(abs(res$exactmass - testmass))], exp_out)

        # must filter out formulas that do not contain at least one C
        res <- Rdisop::decomposeMass(testmass, ppm = 5, mzabs = 0, elements = elem, minElements = "C1")   
        exp_out2 <- exp_out[grep("C", exp_out)]
        testthat::expect_equal(res$formula[order(abs(res$exactmass - testmass))], exp_out2)

        # set maxElements
        res <- Rdisop::decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C1", maxElements = "C6")
        exp_out3 <- c("C6H34N6O3P","C2H37N7O3P2","C3H43O8P2","C3H48NOP5","CH41N3O7P2")
        testthat::expect_equal(res$formula[order(abs(res$exactmass - testmass))], exp_out3)

        # equal minElements and maxElements
        res <- Rdisop::decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C6", maxElements = "C6")
        testthat::expect_equal(res$formula[order(abs(res$exactmass - testmass))], "C6H34N6O3P")
        
        # maxElements < minElements
        res <- Rdisop::decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C6", maxElements = "C5")   
        testthat::expect_null(res)
        
        # Zeros in maxElements are respected and do not cause an error
        res <- Rdisop::decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C6", maxElements = "P0")
        testthat::expect_equal(res$formula[order(abs(res$exactmass - testmass))],c("C10H31N5O3", "C12H33N2O4"))
        
        # more complex exammple:
        elem <- Rdisop::initializeCHNOPS()       
        testmass <- 347.0630844422
        res <- Rdisop::decomposeMass(testmass, ppm = 5,  mzabs = 0, elements = elem, minElements = "C10O6PN", maxElements = "O10P1S3")
        testthat::expect_equal(res$formula[order(abs(res$exactmass - testmass))],c("C10H14N5O7P",  "C10H22NO6PS2", "C12H16N2O8P"))
        
    }
)

testthat::test_that(
    desc = "Implementation of original Runit tests from file runit.limits", 
    code = {
        testthat::expect_equal(length(Rdisop::decomposeMass(12)$formula), 1)
        testthat::expect_equal(length(Rdisop::decomposeMass(12, minElements="C", maxElements="C")$formula), 1)
        testthat::expect_equal(length(Rdisop::decomposeMass(12, minElements="C2", maxElements="C4")$formula), 0)
    }
)

testthat::test_that(
    desc = "Implementation of original Runit tests from file runit.charges", 
    code = {
        suppressMessages({
            testthat::expect_equal(852.354928, Rdisop::getMolecule("C49H56FeN4O6",Rdisop::initializePSE(),z=0)$exactmass, tolerance = 0.00005) 
            testthat::expect_equal(447.089515, Rdisop::getMolecule("C15H23N6O5Se+",c(Rdisop::initializePSE(),Rdisop::initializeCharges()),z=1)$exactmass, tolerance = 0.00005) 
            testthat::expect_equal(447.089515, Rdisop::getMolecule("C15H23N6O5Se+",c(Rdisop::initializePSE(),Rdisop::initializeCharges()),z=-1)$exactmass, tolerance = 0.00005) 
        })
    }
)

testthat::test_that(
    desc = "Implementation of original Runit tests from file runit.monoisotopic", 
    code = {
        elem <- Rdisop::initializePSE()
        testthat::expect_false(all(sapply(elem, function(x) {x$mass == as.integer(Rdisop::getMolecule(x$name, elem)$exactmass)})))
        # JL: actually the above test is bullshit and was not run before. The base mass defined for all elements is not similar to the exactmass (monoisotopic) for most elements
        # Hence, I implemented it with a 'expect_false' expectation
    }
)

testthat::test_that(
    desc = "Implementation of original Runit tests from file runit.keggexamples", 
    code = {
        # cid46173807
        testthat::expect_equal(852.354928, Rdisop::getMolecule("C49H56FeN4O6",Rdisop::initializePSE(),z=0)$exactmass, tolerance = 0.00005)
        # cid24892761
        suppressMessages({
            testthat::expect_equal(447.089515, Rdisop::getMolecule("C15H23N6O5Se+",c(Rdisop::initializePSE(),Rdisop::initializeCharges()),z=1)$exactmass, tolerance = 0.00005)
        })
    }
)
