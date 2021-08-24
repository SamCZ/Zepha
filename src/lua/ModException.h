#pragma once

#include "util/Types.h"

class ModException : public std::exception {
public:
	ModException(const string& message): message(message) {}
	
	const char* what() const throw() {
		return message.data();
	}
	
private:
	string message;
};


