testthat::test_that(
  desc = "Test if .check_limElements() is working properly", 
  code = {
      # returns the value itself if defined correctly
      testthat::expect_equal(.check_limElements(x = "C0H1N2"), "C0H1N2")
      
      # elements with no number will be counted as 1 (as in usual sum formulas)
      testthat::expect_equal(.check_limElements(x = "CHN"), "C1H1N1")
      
      # returns a warning if element in formula not contained in elements set
      testthat::expect_warning(testthat::expect_equal(.check_limElements(x = "C0", elements = initializeElements("Na")), "Na0"))
      
      # default parameter can be used to specify general upper limits
      testthat::expect_equal(.check_limElements(elements = initializeElements("H"), default = 22), "H22")
  }
)

testthat::test_that(
    desc = "Test if .check_elements() is working properly", 
    code = {
        # as default returns full PSE
        testthat::expect_type(.check_elements(x = NULL), "list")
        testthat::expect_length(.check_elements(x = NULL), 104)
        
        # when Chemical names are provided returns list of length of valid element number
        testthat::expect_type(.check_elements(x = "CHNOPS"), "list")
        testthat::expect_length(.check_elements(x = "CHNOPS"), 6)
        testthat::expect_length(.check_elements(x = "Na"), 1)
        
        # when characters can not be assigned to element names a warning is returned
        # together with the valid elements
        testthat::expect_warning(testthat::expect_length(.check_elements(x = "NA"), 1))
        # or returning the default list in case that no element is recognized
        testthat::expect_warning(testthat::expect_length(.check_elements(x = "AllWrong"), 104))
        testthat::expect_warning(testthat::expect_length(.check_elements(x = "AllWrong", default = initializeElements("C")), 1))
        
        # when a correct list is provided it is returned unchanged
        testthat::expect_equal(.check_elements(x = initializeElements(c("C","H","N"))), initializeElements(c("C","H","N")))
    }
)

testthat::test_that(
    desc = "Test if .check_maxisotopes is working properly", 
    code = {
        testthat::expect_equal(.check_maxisotopes(1), 1)
        # no character input
        testthat::expect_error(.check_maxisotopes("1"))
        # no input outside the limits (1..100)
        testthat::expect_error(.check_maxisotopes(0))
        testthat::expect_error(.check_maxisotopes(101))
        # is of length=1
        testthat::expect_error(.check_maxisotopes(1:3))
    }
)

testthat::test_that(
    desc = "Test if .CountChemicalElements() is working properly", 
    code = {
        # The function is from package and rigogously tested there
        x <- .CountChemicalElements("CH4")
        testthat::expect_type(x, "double")
        testthat::expect_length(x, 2)
        testthat::expect_equal(x, stats::setNames(c(1,4),c("C","H")))
        
    }
)

