# to be more comparable with package `enviPat` and to allow easier access to the
# isotope information the original functions defining the PSE as a list is used
# to set up a data.frame which can be attended by Rdisop::isotopes
ele <- initializePSE()
isotopes <- plyr::ldply(ele, function(x) {
    data.frame(
        "element" = rep(x$name, length(x$isotope$mass)),
        "isotope" = paste0(x$mass + (1:length(x$isotope$mass))-1, x$name),
        "mass" = x$mass + x$isotope$mass + (1:length(x$isotope$mass))-1,
        "abundance" = x$isotope$abundance
    )
})
usethis::use_data(isotopes, overwrite = TRUE)
