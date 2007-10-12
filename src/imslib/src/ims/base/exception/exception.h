#ifndef IMS_EXCEPTION_H
#define IMS_EXCEPTION_H

#include <string>
#include <stdexcept>

class ExceptionTest;

namespace ims {

/**
 * Base class for all ims-specific errors 
 */
class Exception : public std::exception {
	public:
		explicit Exception() : msg("") { }
		explicit Exception(const std::string& message) : msg(message) { }
		virtual ~Exception() throw() { }
		
		/** @deprecated Use message() instead.
		 * @todo think about deprecating this. the method is needed because
		 * we derive from std::exception */
		virtual const char* what() const throw() {
			return this->msg.c_str();
		}
		
		virtual const std::string message() const {
			return this->msg;
		}
	
	protected:
		void setMessage(const std::string& message) {
			this->msg = message;
		}
	
	private:
		std::string msg;

	friend class ::ExceptionTest; // needs access to setMessage()
};
 
}

#endif
