
<!-- README.md is generated from README.Rmd. Please edit that file -->

# Rdisop

<!-- badges: start -->

[![R-CMD-check](https://github.com/janlisec/Rdisop/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/janlisec/Rdisop/actions/workflows/R-CMD-check.yaml)
<!-- [![test-coverage](https://github.com/janlisec/Rdisop/actions/workflows/test-coverage.yaml/badge.svg)](https://github.com/janlisec/Rdisop/actions/workflows/test-coverage.yaml) -->
[![DOI](https://img.shields.io/badge/doi-10.1093/bioinformatics/btm631-yellow.svg)](https://doi.org/10.1093/bioinformatics/btm631)
<!-- badges: end -->

In high resolution mass spectrometry (HR-MS), the measured masses can be
decomposed into potential element combinations (chemical sum formulas).
Where additional mass/intensity information of respective isotopic peaks
is available, decomposition can take this information into account to
better rank the potential candidate sum formulas. To compare measured
mass/intensity information with the theoretical distribution of
candidate sum formulas, the latter needs to be calculated. This package
implements fast algorithms to address both tasks, the calculation of
isotopic distributions for arbitrary sum formulas (assuming a HR-MS
resolution of roughly 30,000), and the ranked list of sum formulas
fitting an observed peak or isotopic peak set.

## Installation

You can install the development version of **Rdisop** using:

``` r
# install.packages("devtools")
devtools::install_github("sneumann/Rdisop")
```

## Example

The user can use built in sets of chemical elements or define such sets
specifically and get informations on a specified sum formula, *i.e.* the
isotopic distribution which would be observed in HR-MS.

``` r
ele <- initializeElements(c("C","H","N","O","Mg"))
chlorophyll <- getMolecule("C55H72MgN4O5H", elements = ele)
getIsotope(chlorophyll, 1:4)
#>             [,1]        [,2]       [,3]         [,4]
#> [1,] 893.5431390 894.5459934 895.546247 896.54752708
#> [2,]   0.4140648   0.3171228   0.178565   0.06773657
```

For individual masses, potential molecules can be calculated.

``` r
decomposeMass(mass = 46.042, ppm = 20, maxisotopes = 4)
#> $formula
#> [1] "C2H6O"
#> 
#> $score
#> [1] 1
#> 
#> $exactmass
#> [1] 46.04186
#> 
#> $charge
#> [1] 0
#> 
#> $parity
#> [1] "e"
#> 
#> $valid
#> [1] "Valid"
#> 
#> $DBE
#> [1] 0
#> 
#> $isotopes
#> $isotopes[[1]]
#>            [,1]        [,2]        [,3]         [,4]
#> [1,] 46.0418648 47.04534542 48.04631711 4.904960e+01
#> [2,]  0.9749152  0.02293559  0.00210353 4.540559e-05
```

This decomposition can also be performed for measurements containing
several isotopes.

``` r
# glutamic acid (C5H9NO4)
mol <- decomposeIsotopes(masses = c(147.053, 148.056), intensities = c(93, 5.8))
data.frame(getFormula(mol), getScore(mol), getValid(mol))
#>   getFormula.mol. getScore.mol. getValid.mol.
#> 1         C5H9NO4  1.000000e+00         Valid
#> 2        C3H17P2S  1.935336e-09       Invalid
```
