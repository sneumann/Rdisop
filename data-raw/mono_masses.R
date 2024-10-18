# to provide a fast way calculating the mono-isotopic mass of an element, here a
# named vector containing the main isotopes of all PSE elements is prepared to 
# be accessed by Rdisop::mono_masses
ele <- initializePSE()
mono_masses <- sort(sapply(ele, function(x) {
    m <- x$mass + x$isotope$mass + (1:length(x$isotope$mass))-1
    m <- m[which.max(x$isotope$abundance)]
    setNames(m, x$name)
}))
usethis::use_data(mono_masses, overwrite = TRUE)
