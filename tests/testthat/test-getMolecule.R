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
