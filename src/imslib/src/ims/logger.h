#ifndef IMS_LOGGER_H
#define IMS_LOGGER_H

#include <iostream>

namespace ims {

////////////////////////////////////////////////////////////////////////////////
// written by MAT @ UNIBI 2004
//
// defines a logging object, that prints log messages or not depending on a certein log level
// it's log, log, it's big, it's heavy, it's wood
// it's log, log, it's better than bad, it's good
//
// level of output has to be set for each operation, cause otherwise the old one is used

// Tobias: a few modifications...

////////////////////////////////////////////////////////////////////////////////
// certain log levels

typedef enum {
	Silent = -1,
	Messages = 10,
	Details = 20,
	Everything = 30
} ELogLevel;

////////////////////////////////////////////////////////////////////////////////
//logging class

class Logger
{
public:
	//type for stream
	typedef std::ostream ostream_type;

private:
	//logging stream
	//TODO: totally bad with pointers, but because of stupid gcc we have no choice (where is _io_withassign?)
	ostream_type* log_out;
	//std::_IO_ostream_withassign log_out;

	//current log level
	ELogLevel curr_level;

	//overall logging level
	ELogLevel log_level;

public:
	//constructors
	//default messages only to std::cout
	Logger();
	//given level to cout
	explicit Logger (ELogLevel level);
	//specify everything
	explicit Logger (ELogLevel level, ostream_type* out_stream);

	//operator to set level for following outputs
	Logger& 		operator() (ELogLevel level);

	//operator for performing output operation (of every possible object)
	// TODO: maybe use reference (T& x)
	template <typename T> Logger& operator<< (const T& x);

	//TODO: we have to da the following "specification" of da function, cause in gcc 3.2, thingies like endl and flush (manipulators)
	//are template functions and not normal functions like in gcc 2.95. and in order to allow correct
	// template inference, we have to explicitly specify a function for this
	Logger& operator<< (ostream_type&(*x)(ostream_type&));

	//getter/setter for overall log level
	ELogLevel	GetLogLevel() const { return log_level; }
	void 		SetLogLevel (ELogLevel level) { log_level = level; }

	//funtion for checking, if certain level will be outputtet
	bool		isLog (ELogLevel level) { return (level <= log_level); }
};

////////////////////////////////////////////////////////////////////////////////

////////////     inline constructors

inline Logger::Logger()
	: curr_level (Messages), log_level (Messages)
{
	log_out = &std::cout;
}

inline Logger::Logger (ELogLevel level)
	: curr_level (level), log_level (level)
{
	log_out = &std::cout;
}

inline Logger::Logger (ELogLevel level, Logger::ostream_type* out_stream)
	: curr_level (level), log_level (level)
{
	log_out = out_stream;
}

////////////    inline operator<<

// for everything

// not good, because argument gets copied

template <typename T> Logger& Logger::operator<< (const T& x)
{
	if (curr_level <= log_level)
		*log_out << x;
	return (*this);
}

// for our neat template stream functions

inline Logger& Logger::operator<< (ostream_type& (*x) (ostream_type&))
{
	if (curr_level <= log_level)
		*log_out << x;
	return (*this);
}

////////////     inline function for level setting

inline Logger& Logger::operator() (ELogLevel level) {
	curr_level = level;
	return (*this);
}

////////////////////////////////////////////////////////////////////////////////

}

#endif

