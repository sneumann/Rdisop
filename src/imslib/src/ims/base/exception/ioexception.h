#ifndef IMS_IOEXCEPTION_H
#define IMS_IOEXCEPTION_H

#include <ims/base/exception/exception.h>

namespace ims {

/**
 * An error occurred during I/O.
 */
class IOException : public Exception {
	public:
		explicit IOException() : Exception() { }
		explicit IOException(const std::string& msg) : Exception(msg) { }
};

}

#endif // IMS_IOEXCEPTION_H
