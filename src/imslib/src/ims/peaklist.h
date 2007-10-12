#ifndef IMS_PEAKLIST_H
#define IMS_PEAKLIST_H

#include <vector>
#include <ims/peakpropertyiterator.h>

namespace ims {

// TODO: maybe add setAttribute, getAttribute as frontend to a map<string,?>, could be used for ID, filename and stuff

/**
 * A container for peaks. This class is mainly a wrapper around a vector&lt;PeakType&gt;. Its main
 * purpose is to offer access to (fast) iterators over "parts" of PeakType.
 *
 * For an example for using this class, see peaklist.cpp in the example section.
 *
 * @ingroup peaks
 */
template <typename PeakType>
class PeakList {
	public:
		typedef PeakType peak_type;
		typedef std::vector<PeakType> container;
		typedef typename container::iterator iterator;
		typedef typename container::const_iterator const_iterator;
		typedef typename container::size_type size_type;
		typedef PeakType& reference;
		typedef const PeakType& const_reference;

		// One would want to write
		// template <typename PeakPropertyType>
		// typedef PeakPropertyIterator<PeakList<PeakType>, PeakPropertyType> property_iterator;
		// but this (known as typedef templates) is not allowed by current compilers, see
		// http://www.gotw.ca/gotw/079.htm for further details
		template <typename PeakPropertyType>
		struct property_iterator {
			typedef PeakPropertyIterator<PeakList<PeakType>, PeakPropertyType> type;
		};

		template <typename PeakPropertyType>
		struct const_property_iterator {
			typedef ConstPeakPropertyIterator<PeakList<PeakType>, PeakPropertyType> type;
		};

		PeakList() { }

		iterator begin() { return peaks.begin(); }
		const_iterator begin() const { return peaks.begin(); }
		iterator end() { return peaks.end(); }
		const_iterator end() const { return peaks.end(); }
		reference front() { return peaks.front();}
		const_reference front() const { return peaks.front(); }
		reference back() { return peaks.back();}
		const_reference back() const { return peaks.back(); }

		/** Returns an iterator over the PropertyType properties of this PeakList. */
		template <typename PropertyType>
		typename property_iterator<PropertyType>::type begin() {
			return typename property_iterator<PropertyType>::type(begin());
		}

		template <typename PropertyType>
		typename property_iterator<PropertyType>::type end() {
			return typename property_iterator<PropertyType>::type(end());
		}


		template <typename PropertyType>
		typename const_property_iterator<PropertyType>::type begin() const {
			return typename const_property_iterator<PropertyType>::type(begin());
		}

		template <typename PropertyType>
		typename const_property_iterator<PropertyType>::type end() const {
			return typename const_property_iterator<PropertyType>::type(end());
		}

		/** returns number of peaks in the list **/
		size_type size() const { return peaks.size(); }
		/** resize the peaklist **/
		void resize(size_type n) { peaks.resize(n); }
		void push_back(const_reference p);
		iterator erase(iterator it) { return peaks.erase(it); }
		iterator erase(iterator begin, iterator end) { return peaks.erase(begin, end); }
		void clear();
		bool empty() { return peaks.empty(); }
		void reserve(size_type n) { peaks.reserve(n); }
		reference operator[](size_type i) { return peaks[i]; }
		const_reference operator[](size_type i) const { return peaks[i]; }
		reference at(size_type i) { return peaks[i]; }
		const_reference at(size_type i) const { return peaks[i]; }
	private:
		container peaks;
};


/**
  * Adds a new peak to the list.
  * @param p Peak.
  */
template <typename PeakType>
void PeakList<PeakType>::push_back(const PeakType& peak) {
	peaks.push_back(peak);
}


/**
  * Deletes all Peaks from Peaklist.
  */
template <typename PeakType>
void PeakList<PeakType>::clear() {
	peaks.clear();
}


/**
 * Outputs the peaklist.
 */
template <typename PeakType>
std::ostream& operator<< (std::ostream& os, const PeakList<PeakType>& peaklist) {
	os << "PeakList: [";
	typename PeakList<PeakType>::const_iterator cit = peaklist.begin();
	for ( ; cit != peaklist.end(); ++cit) {
		os << *cit << " ";
	}
	os << "]";
	return os;
}

/**
 * @example peaklist.cpp This example shows how a PeakList should be used.
 */

} // namespace ims

#endif
