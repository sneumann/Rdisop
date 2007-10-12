#ifndef IMS_INVALIDARGUMENTEXCEPTION_H
#define IMS_INVALIDARGUMENTEXCEPTION_H

#include <ims/base/exception/exception.h>

namespace ims {

/**
 * Invalid arguments (parameters).
 */
class InvalidArgumentException : public Exception {
	public:
		explicit InvalidArgumentException() : Exception() { }
		explicit InvalidArgumentException(const std::string& msg) : Exception(msg) { }
};

}

#endif // IMS_INVALIDARGUMENTEXCEPTION_H
