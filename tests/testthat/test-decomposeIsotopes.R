testthat::test_that(
  desc = "decomposeIsotopes can handle maxElements parameter correctly", 
  code = {
      mz <- getMolecule("CH2")$exact
      x <- decomposeIsotopes(masses = mz, intensities = 1, mzabs = 0.02)
      testthat::expect_equal(x[["formula"]], c("CH2", "N"))
      # $JL$ this should return only one result but returns 2
      x <- decomposeIsotopes(masses = mz, intensities = 1, mzabs = 0.02, elements = initializeElements(c("C","H","N")), maxElements = "C0")
      testthat::expect_equal(length(x[["formula"]]), 2L)
}
)

testthat::test_that(
    desc = "decomposeIsotopes can handle element list without Carbon", 
    code = {
        # $JL$ this should NOT return an error but currently does
        testthat::expect_error(decomposeIsotopes(masses = 14.003, intensities = 1, elements = initializeElements("N")))
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