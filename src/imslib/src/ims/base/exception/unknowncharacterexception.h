#ifndef IMS_UNKNOWNCHARACTEREXCEPTION_H
#define IMS_UNKNOWNCHARACTEREXCEPTION_H

#include <ims/base/exception/exception.h>

namespace ims {
	
/** 
 * An exception thrown if a character is unknown. Note that the word "character" 
 * is used in an abstract sense, meaning "element of an alphabet".
 */
class UnknownCharacterException : public Exception {
	public:
		explicit UnknownCharacterException() : Exception() { }
		explicit UnknownCharacterException(const std::string& msg) : Exception(msg) { }
};

}

#endif // IMS_UNKNOWNCHARACTEREXCEPTION_H
