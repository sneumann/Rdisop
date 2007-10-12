#ifndef IMS_SEQUENCEGENERATOR_H
#define IMS_SEQUENCEGENERATOR_H

namespace ims {

/**
 * Interface for a sequence generator.
 */
class SequenceGenerator {
	public:
		virtual std::string getSequence(size_t length) = 0;
		virtual ~SequenceGenerator() { }
};

}

#endif

