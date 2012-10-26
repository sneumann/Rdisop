test.empty <- function() {
   checkEquals(length(decomposeMass(12)$formula), 1)
}

test.exact <- function() {
   checkEquals(length(decomposeMass(12, minElements="C", maxElements="C")$formula), 1)
}

test.remove <- function() {
  checkEquals(length(decomposeMass(12, minElements="C2", maxElements="C4")$formula), 0)
}

## Martin: Add more unit tests

# decomposeMass(1.00785, minElements="C0", maxElements="C99999")
