test.addformula1 <- function() {
   checkEquals(addMolecules("H", "H")$formula, "H2")   
}

test.addformula2 <- function() {
   checkEqualsNumeric(addMolecules("C2H6O", "C2H6O")$exactmass, 2*46.04, tolerance=0.1)   
}


test.subformula1 <- function() {
   checkEquals(subMolecules("C5H10NO4", "H")$formula, "C5H9NO4")
}


test.subformula2 <- function() {
   checkEquals(subMolecules("C14H23N4O8P2S", "C3H4O3")$formula, "C11H19N4O5P2S")
}

test.subformula1 <- function() {
   checkEquals(subMolecules("CH", "H")$formula, "C")
}
