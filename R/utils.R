#' @title .CountChemicalElements.
#' @description \code{.CountChemicalElements} will split a character (chemical formula)
#'  into its elements and count their occurrence.
#' @details No testing for any chemical alphabet is performed. Elements may occur
#'  several times and will be summed up in this case without a warning.
#'  Information within brackets, i.e. [13]C will be removed prior to counting
#'  together with other symbols not contained in the R set 'alnum'.
#'  The result is filtered and ordered according to parameter 'ele' if provided.
#' @param x Chemical formula.
#' @param ele Character vector of elements to count particularly or counting all contained in string if NULL.
#' @return A named numeric with counts for all contained or specified elements.
#' @references This function is a copy from the `CorMID` package by Jan Lisec.
#'   It is not (and should bot be) exported in Rdisop but only used internally.
#' @noRd
#' @keywords internal
.CountChemicalElements <- function(x = NULL, ele = NULL) {
    # ensure character and length=1
    x <- as.character(x[1])
    # count all elements present within 'x'
    # remove square bracket constructs (e.g. [13]C6 --> C6) upfront
    x <- gsub("[[].+[]]", "", x)
    # all elements start with a LETTER...
    p <- gregexpr("[[:upper:]]", x)[[1]]
    p_end <- c(p[-1] - 1, nchar(x))
    # split initial string at the large letter positions
    out <- sapply(1:length(p), function(i) {
        # substr(x, p[i], ifelse(i == length(p), nchar(x), p[i + 1] - 1))
        substr(x, p[i], p_end[i])
    })
    # remove all non letter/digit (e.g. further brackets, charges...)
    out <- gsub("[^[:alnum:]]", "", out)
    count <- as.numeric(gsub("[^[:digit:]]", "", out))
    count[is.na(count)] <- 1
    names(count) <- gsub("[^[:alpha:]]", "", out)
    # sum up in case that elements were found repeatedly
    if (any(duplicated(names(count)))) {
        for (i in rev(which(duplicated(names(count))))) {
            count[which((names(count) == names(count)[i]))[1]] <- count[which((names(count) == names(count)[i]))[1]] + count[i]
            count <- count[-i]
        }
    }
    # reorder or limit output vector according to 'ele' and 'order_ele'
    if (!is.null(ele)) {
        count <- sapply(ele, function(e) {
            ifelse(e %in% names(count), count[names(count) == e], 0)
        })
    }
    return(count)
}

#' @title .check_maxisotopes.
#' @description \code{.check_maxisotopes} will check this parameter in various
#'     functions to ensure that it is within the specifications.
#' @param x maxisotopes parameter.
#' @return Either the input or stops with an error message.
#' @noRd
#' @keywords internal
.check_maxisotopes <- function(x) {
    if (!(is.numeric(x) && x>=1 && x<=100)) stop("maxisotopes is not a number within the allowed range (1..100)")
    return(x)
}

#' @title .minset_elements.
#' @description \code{.minset_elements} will check the formula parameter in various
#'     functions and return a minimal elements list to perform the functions
#'     calculations on this formula.
#' @param fml formula parameter.
#' @return The output of \code{\link{initializeElements}} required to process this formula.
#' @noRd
#' @keywords internal
.minset_elements <- function(fml) {
    # $$ToDo$$ JL: .minset_elements() can be substituted against the more general .check_elements()
    x <- names(.CountChemicalElements(fml))
    return(initializeElements(x))
}

#' @title .check_elements
#' @description \code{.check_elements} will check this parameter in various
#'     functions to ensure that it is within the specifications. It also allows
#'     the user to provide the element definitions via the legacy way (as a list
#'     similar to the initializeElements() output) or similar to the way the 
#'     parameters `minElements`and `maxElements` are defined (character).
#' @param x elements parameter.
#' @param default A default valid element set.
#' @return Either the default set or a correct elemental set based on x.
#' @examples
#' .check_elements(x = NULL)
#' .check_elements(x = "CHNOPS")
#' .check_elements(x = "NA")
#' .check_elements(x = "Na")
#' .check_elements(x = "AllWrong")
#' .check_elements(x = initializeElements(c("C","H","N")))
#' 
#' @noRd
#' @keywords internal
.check_elements <- function(x = NULL, default = initializePSE()) {
    # return default element set (PSE if not defined otherwise) for x = NULL
    if (is.null(x)) return(default)
    
    # extract valid element name if x is a character vector of length = 1 and return their definitions
    if (length(x)==1 & is.character(x)) {
        ele <- names(.CountChemicalElements(x))
        PSE <- initializePSE()
        nms <- sapply(PSE, function(x) { x$name })
        idx <- !(ele %in% nms)
        if (any(idx)) {
            warning("Could not find a definition for element", ifelse(sum(idx)>=2, "s ", " "), paste(ele[idx], collapse = ", "))
        }
        if (any(!idx)) {
            return(PSE[nms %in% ele])
        } else {
            return(default)
        }
    }
    
    # return default element set (PSE if not defined otherwise) for x = anything else but a list
    if (!is.list(x) || length(x) == 0) {
        return(default)
    } else {
        # return the original input as it seems to be ok
        return(x)
    }
}

#' @title .check_limElements
#' @description \code{.check_limElements} will check elemental limit parameters 
#'     `minElements`and `maxElements` to ensure that they are within the 
#'     specifications.
#' @param x limit parameter.
#' @param elements A valid element list.
#' @param default A default value to be used as a limit.
#' @return Either the input or stops with an error message.
#' @examples
#' .check_limElements(x = "C0", elements = initializeElements("Na"))
#' .check_limElements(x = "CHNOPS")
#' 
#' @noRd
#' @keywords internal
.check_limElements <- function(x = NULL, elements = initializePSE(), default = 0) {
    nms <- sapply(elements, function(x) { x$name })
    if (is.null(x)) {
        ele <- stats::setNames(rep(default, length(nms)), nms)
    } else {
        ele <- .CountChemicalElements(x)
    }
    idx <- !(names(ele) %in% nms)
    if (any(idx)) {
        # elements with limit specifications but which are not present in the defined element set should be removed
        warning("Removed element", ifelse(sum(idx)>=2, "s ", " "), paste(names(ele[idx]), collapse = ", "), " from input ", x)
    }
    if (any(!idx)) {
        # filter original definition for allowed elements
        x <- paste(names(ele[!idx]), ele[!idx], sep = "", collapse="")
    } else {
        # provide a vector based on the element set
        x <- paste(nms, default, sep = "", collapse="")
    }
    return(x)
}