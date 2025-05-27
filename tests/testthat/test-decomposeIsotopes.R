testthat::test_that(
  desc = "decomposeIsotopes can handle maxElements parameter correctly", 
  code = {
        mz <- getMolecule("CH2")$exact
        x <- decomposeIsotopes(masses = mz, intensities = 1, mzabs = 0.02)
        testthat::expect_equal(x[["formula"]], c("CH2", "N"))
        # $JL$ from v1.67.3 on this does return only a single result
        x <- decomposeIsotopes(masses = mz, intensities = 1, mzabs = 0.02, elements = initializeElements(c("C","H","N")), maxElements = "C0")
        testthat::expect_equal(length(x[["formula"]]), 1L)
    }
)

testthat::test_that(
    desc = "decomposeIsotopes can handle element list without Carbon", 
    code = {
        # this should return two warnings from v1.67.6 on
        # reason is the default parameter of 'minElements' = C0 in decomposeIsotopes()
        # --> this caused errors if parameter 'elements' did not specify C
        testthat::expect_warning(
            testthat::expect_warning(
                decomposeIsotopes(masses = 14.003, intensities = 1, elements = initializeElements("N"))
            )
        )
    }
)

testthat::test_that(
    desc = "decomposeIsotopes Glutamate example works", 
    code = {
        x <- decomposeIsotopes(c(147.0529, 148.0563), c(100.0, 5.56))
        testthat::expect_equal(length(x), 8L)
        testthat::expect_equal(names(x)[1], "formula")
        testthat::expect_equal(length(x[["formula"]]), 2L)
        testthat::expect_equal(x[["formula"]], c("C5H9NO4", "C3H17P2S"))
    }
)

testthat::test_that(
    desc = "decomposeIsotopes does not modify the object provided as 'intensities' parameter", 
    code = {
        # $JL$ this ensures that the fix for issue #21 works
        mzs <- c(147.0529, 148.0563)
        ints <- c(100.0, 5.561173)
        res <- decomposeIsotopes(mzs, ints)
        testthat::expect_equal(ints, c(100.0, 5.561173))
    }
)

testthat::test_that(
    desc = "'maxisotopes' parameter is checked for all dependend functions", 
    code = {
        # $JL$ this ensures that the fix for issue #20 works
        testthat::expect_error(getMolecule("C2H6O", maxisotopes = 0))
        testthat::expect_error(subMolecules("CH", "H", maxisotopes = "1"))
        testthat::expect_error(addMolecules("C", "H", maxisotopes = 101))
        testthat::expect_error(decomposeIsotopes(c(147.0529, 148.0563), c(100.0, 5.56), maxisotopes = NA))
    }
)

testthat::test_that(
    desc = "decomposeIsotopes parameter 'maxIsotopes' does respect an upper limit of zero", 
    code = {
        # $JL$ this ensures that the fix for issue #12 and #5 works
        testthat::expect_equal(getFormula(decomposeMass(48, mzabs = 0.003)), c("C4", "H3NP"))
        # this did not work before version 1.67.3
        testthat::expect_equal(getFormula(decomposeMass(48, mzabs = 0.003, maxElements = "C0")), "H3NP")
    }
)

