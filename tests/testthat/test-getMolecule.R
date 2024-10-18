testthat::test_that(
    desc = "getMolecule handles z parameter (charge) correctly", 
    code = {
        
        # exactmass is corrected for electron mass if z!=0 specified
        testthat::expect_equal(getMolecule("H", z=0)$exactmass, 1.00782503)
        testthat::expect_equal(getMolecule("H", z=1)$exactmass, 1.00727645)
        
        # also isotope masses need to be adjusted
        testthat::expect_equal(getMolecule("C6H12O6", z=2)$isotopes[[1]][1,1], 90.031146)
        
    }
)

testthat::test_that(
    desc = "getMono returns monoisotopic masses correctly", 
    code = {
        
        # example where exactmass and monoisotopic mass differ
        fml <- "C89H176O16P2"
        out <- getMolecule(fml)
        
        # the exact mass is the second isotope
        testthat::expect_equal(getMass(out), getIsotope(out, 2)[[1]][1,])
        
        # the monoisotopic mass is the first isotope
        testthat::expect_equal(unname(getMono(out)), getIsotope(out, 1)[[1]][1,])
        
        # getMono works on a fml directly
        testthat::expect_equal(getMono(out), getMono(fml))
        
        # getMono works also on decomposeIsotopes output
        out <- decomposeIsotopes(c(147.0529, 148.0563), c(100.0, 5.56))
        testthat::expect_equal(unname(getMono(out)), sapply(getIsotope(out, 1), function(x) { x[1,] }))
    }
)
