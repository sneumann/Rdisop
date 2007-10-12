#ifndef IMS_IDSEQUENCE_H
#define IMS_IDSEQUENCE_H

#include <string>

namespace ims {

/**
 * A class to store a sequence with
 * corresponding string and/or numerical
 * ID.
 */

template <typename IDType>
class IDSequence {
	public:
		IDSequence();
		IDSequence(const IDSequence& ids);
		IDSequence(std::string seq, IDType id);
		~IDSequence();
		IDType getID() const;
		void setID(IDType id);
		std::string getSequence() const;
		void setSequence(const std::string& seq);
		int getLength() const;
	private:
		std::string seq;
		IDType id;
};

/**
 * Builds empty IDSequence. Sequence and ID are not set.
 */
template <typename IDType>
IDSequence<IDType>::IDSequence() {
}

/**
 * Builds new IDSequence as copy from other one.
 */
template <typename IDType>
IDSequence<IDType>::IDSequence(const IDSequence<IDType>& ids) {
    this->id = ids.getID();
    this->seq = ids.getSequence();
}

/**
 * Build IDSequence with seq and id.
 */
template <typename IDType>
IDSequence<IDType>::IDSequence(std::string seq, IDType id) {
    this->seq = seq;
    this->id = id;
}

/**
 * Deletes IDSequence
 */
template <typename IDType>
IDSequence<IDType>::~IDSequence() {
}

/**
 * Gets ID from IDSequence
 * @return ID
 */
template <typename IDType>
IDType IDSequence<IDType>::getID() const{
	return id;
}

/**
 * Set ID of IDSequence
 * @param id ID
 */
template <typename IDType>
void IDSequence<IDType>::setID(IDType id) {
	this->id = id;
}

/**
 * Get string from IDSequence
 * @return string
 */
template <typename IDType>
std::string IDSequence<IDType>::getSequence() const{
	return seq;
}

/**
 * Set string of IDSequence
 * @param seq string
 */
template <typename IDType>
void IDSequence<IDType>::setSequence(const std::string& seq) {
	this->seq = seq;
}

/**
 * Returns length of sequence
 */
template <typename IDType>
int IDSequence<IDType>::getLength() const{
	return seq.length();
}

}
#endif

