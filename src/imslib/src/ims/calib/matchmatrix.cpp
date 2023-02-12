#include <ims/calib/matchmatrix.h>

namespace ims {

/**
 * @author Tobias Marschall <Tobias.Marschall@CeBiTec.Uni-Bielefeld.DE>
 */
MatchMatrix::MatchMatrix(size_t newRows) {
	matrix = new row_t[newRows];
	rows = newRows;
	for (size_t i=0 ; i<rows ; i++) {
		matrix[i].start=-1;
		matrix[i].end=-1;
	}
}

MatchMatrix::~MatchMatrix() {
	delete [] matrix;
}

void MatchMatrix::set(size_t row, size_t column) /*throw (IndexOutOfBounds, InvalidMatchMatrix)*/ {
	// TODO: This function does not enforce staircase property in all cases
	// for example:
	// mm.set(6,2);
	// mm.set(7,1); <-- Exception should but will not be thrown
	// However, this check would be costly

	// is given row valid?
	if (row>=rows) {
		throw IndexOutOfBounds();
	}
	// is row empty?
	if (matrix[row].start == -1) {
		matrix[row].start=column;
		matrix[row].end=column;
		return;
	}
	// row isn't empty, lets see if addition is valid
	if (((int)column<matrix[row].start-1) || ((int)column>matrix[row].end+1)) {
		throw InvalidMatchMatrix();
	}
	// enlarge "run of ones" in given row
	matrix[row].start=std::min(matrix[row].start, (int)column);
	matrix[row].end=std::max(matrix[row].end, (int)column);
}

void MatchMatrix::unset(size_t row, size_t column) /*throw (IndexOutOfBounds, InvalidMatchMatrix)*/ {
	// is given row valid?
	if (row>=rows) {
		throw IndexOutOfBounds();
	}
	// maybe row is already empty?
	if (matrix[row].start == -1) {
		// --> nothing to be done here...
		return;
	}
	// check if column is in the middle of a "run of ones"
	if (((int)column>matrix[row].start) && ((int)column<matrix[row].end)) {
		// --> this would violate staircase property
		throw InvalidMatchMatrix();
	}
	// downsize from front?
	if (matrix[row].start == (int)column) {
		matrix[row].start++;
	}
	// downsize from back?
	if (matrix[row].end == (int)column) {
		matrix[row].end--;
	}
	// is row empty now?
	if (matrix[row].start > matrix[row].end) {
		matrix[row].start=-1;
		matrix[row].end=-1;
	}
}

size_t MatchMatrix::getRows() {
	return rows;
}

std::unique_ptr<std::map<int,int> > MatchMatrix::countMatches() {
	std::unique_ptr<std::map<int,int> > m(new std::map<int,int>);
	int last_match=-1;
	int score = 0;
	// iterate over rows
	for (size_t row=0 ; row<rows ; row++) {
		// row empty --> skip
		if (matrix[row].start==-1) {
			continue;
		}
		int candidate = std::max(matrix[row].start, last_match+1);
		// is candidate inside a "run of ones"?
		if (candidate<=matrix[row].end) {
			(*m)[row]=candidate;
			last_match=candidate;
			score++;
		}
	}
	return m;
}

std::unique_ptr<std::map<int,int> > MatchMatrix::countMatchesRestrictive() {
	std::unique_ptr<std::map<int,int> > m(new std::map<int,int>);
	
	int last_match=-1;
	int score = 0;
	// iterate over rows
	for (size_t row=0 ; row<rows ; row++) {
		// row empty --> skip
		if (matrix[row].start==-1) {
			continue;
		}
		// only consider row if start==end, this are the non-ambiguous matches
		if (matrix[row].start != matrix[row].end) {
			continue;
		}
		// finding a candidate isn't that difficult here... :-)
		int candidate = matrix[row].start;
		// check if last_match didn't already map to candidate
		if (candidate > last_match) {
			(*m)[row]=candidate;
			last_match=candidate;
			score++;
		}
	}
	return m;
}

}
