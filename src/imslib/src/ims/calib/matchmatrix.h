#ifndef IMS_MATCHMATRIX_H
#define IMS_MATCHMATRIX_H

#include <memory>
#include <map>
#include <ims/base/exception/exception.h>

namespace ims {
/**
 *
 * @author Tobias Marschall <Tobias.Marschall@CeBiTec.Uni-Bielefeld.DE>
 *
 */
// TODO: where to put this one? Maybe there already is a IndexOutOfBounds Exception somewhere?!?
////////////////////////////////////////////////////////////////////////////////////////
// Anton: temporary workaround is done, in order to compile the code with CC. Redesign is needed!
class IndexOutOfBounds : public Exception {
	public:
		explicit IndexOutOfBounds() : Exception() { }
		explicit IndexOutOfBounds(const std::string& msg) : Exception(msg) { }
};

class InvalidMatchMatrix : public Exception {
	public:
		explicit InvalidMatchMatrix() : Exception() { }
		explicit InvalidMatchMatrix(const std::string& msg) : Exception(msg) { }
};
////////////////////////////////////////////////////////////////////////////////////////
// TODO: add excact reference to mentioned paper
/** Represents a matrix which contains entries either 0 or 1 and has a staircase property.
 *  See Velis/Sebastians paper on recalibration for exact definition.
 */
class MatchMatrix {
private:
	// The matrix is a staircase matrix, so we only store the start and the end
	// of a run of 1s for each row. If a row is zero, start and end are set to -1
	typedef struct {
		// start of run of 1s
		int start;
		// end of run of 1s
		int end;
	} row_t;
	row_t *matrix;
	std::size_t rows;
public:
	/** Construct MatchMatrix with specified number of rows.
	 *  The number of columns need not be known because of the staircase property.
	 */
	explicit MatchMatrix(std::size_t rows);
	~MatchMatrix();

	/** Set specified entry to 1. */
	void set(std::size_t row, std::size_t column) /*throw (IndexOutOfBounds, InvalidMatchMatrix)*/;
	/** Set specified entry to 0. */
	void unset(std::size_t row, std::size_t column) /*throw (IndexOutOfBounds, InvalidMatchMatrix)*/;

	/** Returns number of rows. */
	std::size_t getRows();

	/** Greedily compute one-to-one matches. */
	std::unique_ptr<std::map<int,int> > countMatches();
	/** Similar to countMatches() with the restriction, to allow only real one2one matches
	 * (i.e. matches that are non-ambiguous).
	 */
	std::unique_ptr<std::map<int,int> > countMatchesRestrictive();
};

}

#endif
