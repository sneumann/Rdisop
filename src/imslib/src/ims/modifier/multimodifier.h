#ifndef IMS_MULTIMODIFIER_H
#define IMS_MULTIMODIFIER_H

#include <list>
#include <memory>

#include <ims/modifier/modifier.h>


namespace ims {
/**
 * Implements a modifier class that applies multiple modifiers. The modifiers
 * are applied in the same order they were added via addModifier.
 *
 * @ingroup modifiers
 */
template <typename PeakListType>
class MultiModifier : public Modifier<PeakListType> {
	public:
		MultiModifier() {}
		/** Copy constructor. All owned modifiers are copied. */
		MultiModifier(const MultiModifier& m) : Modifier<PeakListType>() { copyModifiers(m); }
		virtual ~MultiModifier() { deleteModifiers(); }
		MultiModifier& operator=(const MultiModifier& m);
        /**
		 * Modifies the given peaklist.
		 * @param peaklist A peaklist to be modified.
		 */
		virtual void modify(PeakListType& peakList) const;

		virtual void print(std::ostream& os) const;

		virtual MultiModifier* clone() const {
			return new MultiModifier(*this);
		}

		/**
		 * Adds a modifier
		 */
		void addModifier(std::unique_ptr<Modifier<PeakListType> >);

	private:
		void deleteModifiers();
		void copyModifiers(const MultiModifier& m);
		std::list<Modifier<PeakListType>* > modifiers;
};

template <typename PeakListType>
MultiModifier<PeakListType>& MultiModifier<PeakListType>::operator=(const MultiModifier<PeakListType>& m) { 
	if (&m != this) {
		deleteModifiers(); 
		copyModifiers(m);
	}
	return *this;
}

template <typename PeakListType>
void MultiModifier<PeakListType>::addModifier(std::unique_ptr<Modifier<PeakListType> > modifier) {
	modifiers.push_back(modifier.release());
}

template <typename PeakListType>
void MultiModifier<PeakListType>::modify(PeakListType& peakList) const {
	typename std::list<Modifier<PeakListType>* >::const_iterator it;
	for(it = modifiers.begin(); it != modifiers.end(); ++it) {
		(*it)->modify(peakList);
	}
}


template <typename PeakListType>
void MultiModifier<PeakListType>::print(std::ostream& os) const {
	os << "MultiModifier: [";
	bool first = true;
	typename std::list<Modifier<PeakListType>* >::const_iterator it;
	for(it = modifiers.begin(); it != modifiers.end(); ++it) {
		if (not first) {
			os << ", ";
		}
		else {
			first = false;
		}
		os << **it;
	}
	os << "]";
}

template <typename PeakListType>
void MultiModifier<PeakListType>::deleteModifiers() {
	typename std::list<Modifier<PeakListType>* >::const_iterator cit;
	for (cit=modifiers.begin(); cit!=modifiers.end(); ++cit) {
		delete *cit;
	}
}


template <typename PeakListType>
void MultiModifier<PeakListType>::copyModifiers(const MultiModifier& m) {
	typename std::list<Modifier<PeakListType>* >::const_iterator cit;
	for (cit=m.modifiers.begin(); cit!=m.modifiers.end(); ++cit) {
		modifiers.push_back((*cit)->clone());
	}
}

} // namespace ims
#endif
