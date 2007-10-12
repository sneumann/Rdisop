#ifndef IMS_PEAKPROPERTYITERATOR_H
#define IMS_PEAKPROPERTYITERATOR_H

#include <iterator>

namespace ims {

template <typename PeakListType, typename PropertyGetter>
class ConstPeakPropertyIterator;


/** @ingroup peaks */
template <typename PeakListType, typename PropertyGetter>
class PeakPropertyIterator : public std::iterator<std::random_access_iterator_tag, typename PropertyGetter::value_type> {
	public:
		typedef PeakListType peaklist_type;
		typedef typename PropertyGetter::value_type value_type;
		typedef typename PeakListType::iterator::difference_type difference_type;
		typedef typename PeakListType::size_type size_type;

		PeakPropertyIterator() {}

		PeakPropertyIterator(typename PeakListType::iterator iterator): it(iterator) {}

		const value_type& operator*() const {
			return PropertyGetter::get(*it);
		}

		value_type& operator*() {
			return PropertyGetter::get(*it);
		}

		const value_type* operator->() const {
			value_type& v=PropertyGetter::get(*it);
			return &v;
		}

		value_type* operator->() {
			value_type& v=PropertyGetter::get(*it);
			return &v;
		}

		const value_type& operator[](difference_type n) const {
			return PropertyGetter::get(it[n]);
		}

		value_type& operator[](difference_type n) {
			return PropertyGetter::get(it[n]);
		}

		bool operator==(const PeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return ((this == &iterator) || (this->it == iterator.it));
		}

		bool operator!=(const PeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return !(operator==(iterator));
		}

		bool operator<=(const PeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return (this->it <= iterator.it);
		}

		bool operator>=(const PeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return (this->it >= iterator.it);
		}

		bool operator<(const PeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return (this->it < iterator.it);
		}

		bool operator>(const PeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return (this->it > iterator.it);
		}

		/** Prefix operator++ */
		PeakPropertyIterator<PeakListType, PropertyGetter>& operator++() {
			++it;
			return *this;
		}
		/** Prefix operator-- */
		PeakPropertyIterator<PeakListType, PropertyGetter>& operator--() {
			--it;
			return *this;
		}

		/** Postfix operator++ */
		PeakPropertyIterator<PeakListType, PropertyGetter> operator++(int) {
			PeakPropertyIterator<PeakListType, PropertyGetter> p = *this;
			++it;
			return p;
		}
		/** Postfix operator-- */
		PeakPropertyIterator<PeakListType, PropertyGetter> operator--(int) {
			PeakPropertyIterator<PeakListType, PropertyGetter> p = *this;
			--it;
			return p;
		}

		PeakPropertyIterator<PeakListType, PropertyGetter>& operator+=(size_type n) {
			it+=n;
			return *this;
		}

		PeakPropertyIterator<PeakListType, PropertyGetter>& operator-=(size_type n) {
			it-=n;
			return *this;
		}

		difference_type operator-(const PeakPropertyIterator<PeakListType, PropertyGetter>& ppi) const {
			return it-ppi.it;
		}

	private:
		typename PeakListType::iterator it;

	friend class ConstPeakPropertyIterator<PeakListType,PropertyGetter>;
};

template <typename PeakListType, typename PropertyGetter>
PeakPropertyIterator<PeakListType, PropertyGetter> operator+ (
	const PeakPropertyIterator<PeakListType, PropertyGetter>& ppi, typename PeakListType::size_type n)
{
	PeakPropertyIterator<PeakListType, PropertyGetter> result=ppi;
	result+=n;
	return result;
}

template <typename PeakListType, typename PropertyGetter>
PeakPropertyIterator<PeakListType, PropertyGetter> operator+ (
	typename PeakListType::size_type n, const PeakPropertyIterator<PeakListType, PropertyGetter>& ppi)
{
	PeakPropertyIterator<PeakListType, PropertyGetter> result=ppi;
	result+=n;
	return result;
}


template <typename PeakListType, typename PropertyGetter>
PeakPropertyIterator<PeakListType, PropertyGetter> operator- (
	const PeakPropertyIterator<PeakListType, PropertyGetter>& ppi, typename PeakListType::size_type n)
{
	PeakPropertyIterator<PeakListType, PropertyGetter> result=ppi;
	result-=n;
	return result;
}


/**
 * this used to be a template specialization of PeakPropertyIterator but that didn't quite work
 * TODO try that again, the "didn't work" was due to a different problem.
 * @ingroup peaks
 */
template <typename PeakListType, typename PropertyGetter>
class ConstPeakPropertyIterator : public std::iterator<std::random_access_iterator_tag, typename PropertyGetter::value_type> {
	public:
		typedef PeakListType peaklist_type;
		typedef typename PropertyGetter::value_type value_type;
		typedef typename PeakListType::size_type size_type;
		typedef typename PeakListType::const_iterator::difference_type difference_type;

		ConstPeakPropertyIterator() {}

		ConstPeakPropertyIterator(typename PeakListType::const_iterator iterator): cit(iterator) {}

		/** regular copy constructor */
		ConstPeakPropertyIterator(const ConstPeakPropertyIterator<PeakListType,PropertyGetter>& iterator): cit(iterator.cit) {}

		/** construct from regular (non-const) PeakPropertyIterators */
		ConstPeakPropertyIterator(const PeakPropertyIterator<PeakListType,PropertyGetter>& iterator): cit(iterator.it) {}

		ConstPeakPropertyIterator<PeakListType, PropertyGetter>& operator=(const PeakPropertyIterator<PeakListType,PropertyGetter>& iterator) {
			cit = iterator.it;
			return *this;
		}

		const value_type& operator*() const {
			return PropertyGetter::get(*cit);
		}

		const value_type* operator->() const {
			value_type& v=PropertyGetter::get(*cit);
			return &v;
		}

		const value_type& operator[](difference_type n) const {
			return PropertyGetter::get(cit[n]);
		}

		bool operator==(const ConstPeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return ((this == &iterator) || (this->cit == iterator.cit));
		}

		bool operator!=(const ConstPeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return !(operator==(iterator));
		}

		bool operator<=(const ConstPeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return (this->cit <= iterator.cit);
		}

		bool operator>=(const ConstPeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return (this->cit >= iterator.cit);
		}

		bool operator<(const ConstPeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return (this->cit < iterator.cit);
		}

		bool operator>(const ConstPeakPropertyIterator<PeakListType, PropertyGetter>& iterator) const {
			return (this->cit > iterator.cit);
		}

		/** Prefix operator++ */
		ConstPeakPropertyIterator<PeakListType, PropertyGetter>& operator++() {
			++cit;
			return *this;
		}
		/** Prefix operator-- */
		ConstPeakPropertyIterator<PeakListType, PropertyGetter>& operator--() {
			--cit;
			return *this;
		}

		/** Postfix operator++ */
		ConstPeakPropertyIterator<PeakListType, PropertyGetter> operator++(int) {
			ConstPeakPropertyIterator<PeakListType, PropertyGetter> p = *this;
			++cit;
			return p;
		}
		/** Postfix operator-- */
		ConstPeakPropertyIterator<PeakListType, PropertyGetter> operator--(int) {
			ConstPeakPropertyIterator<PeakListType, PropertyGetter> p = *this;
			--cit;
			return p;
		}

		ConstPeakPropertyIterator<PeakListType, PropertyGetter>& operator+=(size_type n) {
			cit+=n;
			return *this;
		}

		ConstPeakPropertyIterator<PeakListType, PropertyGetter>& operator-=(size_type n) {
			cit-=n;
			return *this;
		}

		difference_type operator-(const ConstPeakPropertyIterator<PeakListType, PropertyGetter>& ppi) const {
			return cit-ppi.cit;
		}

	private:
		typename PeakListType::const_iterator cit;
};

}

#endif // IMS_PEAKPROPERTYITERATOR_H
