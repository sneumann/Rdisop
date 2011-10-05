test.subformula1 <- function() {
   checkEquals(subMolecules("C5H10NO4", "H")$formula, "C5H9NO4")
}


test.subformula2 <- function() {
   checkEquals(subMolecules("C14H23N4O8P2S", "C3H4O3")$formula, "C11H19N4O5P2S")
}

