testthat::test_that(
    desc = "getMolecule handles z parameter (charge) correctly", 
    code = {
        
        suppressMessages({
            # exactmass is corrected for electron mass if z!=0 specified
            testthat::expect_equal(getMolecule("H", z=0)$exactmass, 1.00782503)
            testthat::expect_equal(getMolecule("H", z=1)$exactmass, 1.00727645)
            
            # also isotope masses need to be adjusted
            testthat::expect_equal(getMolecule("C6H12O6", z=2)$isotopes[[1]][1,1], 90.031146)
        })
        
    }
)

testthat::test_that(
    desc = "getMonoisotopic returns monoisotopic masses correctly", 
    code = {
        
        # example where exactmass and monoisotopic mass differ
        fml <- "C89H176O16P2"
        out <- getMolecule(fml)
        
        # the exact mass is the second isotope
        # this test is no longer valid since v.1.67.5 due to defaulting to more precise NIST elemental definitions
        # before v1.67.5 the rounding errors led to the 'exactmass' being the second isotope
        # testthat::expect_equal(getMass(out), getIsotope(out, 2)[[1]][1,])
        
        # the monoisotopic mass is the first isotope
        testthat::expect_equal(unname(getMonoisotopic(out)), getIsotope(out, 1)[[1]][1,])
        
        # getMonoisotopic works on a fml directly
        testthat::expect_equal(getMonoisotopic(out), getMonoisotopic(fml))
        
        # getMonoisotopic works also on decomposeIsotopes output
        out <- decomposeIsotopes(c(147.0529, 148.0563), c(100.0, 5.56))
        testthat::expect_equal(unname(getMonoisotopic(out)), sapply(getIsotope(out, 1), function(x) { x[1,] }))
    }
)

testthat::test_that(
    desc = "getMolecule checks sum formulas to prevent crashes", 
    code = {
        
        # this was an error reported in issue #3
        # strings which can not be processed by the C++ function return an error
        testthat::expect_error(getMolecule("3"))
        
        # elements contained in the formula need to exist in the provided elements list
        testthat::expect_error(getMolecule(formula = "CH4", elements = initializeElements("H")))

    }
)

testthat::test_that(
    desc = "getMolecule improves precision with NIST elemental definitions compared to IUPAC", 
    code = {
        
        # this was an error reported in issue #22
        # example fml returns a wrong monoisotopic peak due to rounding differences
        fml <- "C89H166O17P2"
        res_iupac <- Rdisop::getMolecule(formula = fml, elements = initializePSE(method = "IUPAC"), maxisotopes = 2)$isotopes[[1]]
        res_nist <- Rdisop::getMolecule(formula = fml, elements = initializePSE(method = "NIST"), maxisotopes = 2)$isotopes[[1]]
        
        testthat::expect_equal(res_iupac, structure(c(1569.16002852, 0.494738964752399, 1570.16346006151, 0.505261035247601), dim = c(2L, 2L)))
        testthat::expect_equal(res_nist, structure(c(1569.16002752, 0.502975666994795, 1570.16344446454, 0.497024333005205), dim = c(2L, 2L)))
        
        # the monoisotopic peak has changed
        testthat::expect_true(res_iupac[2,1]<0.5 & res_nist[2,1]>0.5)
        
    }
)

testthat::test_that(
    desc = "getMolecule getter functions work", 
    code = {
        
        # single molecule
        x <- getMolecule("H")
        testthat::expect_equal(getMass(x), 1.00782503)
        testthat::expect_equal(getFormula(x), "H")
        testthat::expect_equal(getScore(x), 1)
        testthat::expect_equal(getValid(x), "Invalid")
        
        # multiple molecules
        x <- decomposeIsotopes(masses = getMolecule("CH2")$exact, intensities = 1, mzabs = 0.02)
        testthat::expect_equal(getMass(x), c(14.01565, 14.00307), tolerance = 10^-4)
        testthat::expect_equal(getFormula(x), c("CH2", "N"))
        testthat::expect_equal(getScore(x), c(1, 0))
        testthat::expect_equal(getValid(x), c("Valid", "Invalid"))
    }
)

