#ifndef KTROBOGAMEERROR_H
#define KTROBOGAMEERROR_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <exception>
namespace KTROBO {
	const int FATAL_ERROR = 1;
	const int WARNING = -1;
	const int INFO = -2;
	const int NOTICE = -3;
}

namespace KTROBO {

class GameError : public std::exception
{
private:
	int error_code;
	std::string message;
public:
	GameError(void) throw() : error_code(KTROBO::FATAL_ERROR), message("Undefined Error in game"){};
	GameError(const GameError& e) throw() : std::exception(e), error_code(e.error_code),message(e.message) {};
	GameError(int code, const std::string &s) throw() :error_code(code), message(s) {}
	GameError& operator = (const GameError& rhs) throw() {
		std::exception::operator=(rhs);
		this->error_code = rhs.error_code;
		this->message = rhs.message;
	}

	virtual ~GameError() throw() {};
	virtual const char* what() const throw() {
		return this->getMessage();
	}
	const char* getMessage() const throw() {return message.c_str(); }
	int getErrorCode() const throw() {return error_code;}
	static const char* getErrorCodeString(int error_code) {
		if (error_code == KTROBO::FATAL_ERROR) {
			return "FATAL_ERROR";
		} else if(error_code == KTROBO::WARNING) {
			return "WARNING";
		} else if(error_code == KTROBO::INFO) {
			return "INFO";
		} else if(error_code == KTROBO::NOTICE) {
			return "NOTICE";
		} else {
			return "UNKNOWN";
		}
	}

	
};

}

#endif
