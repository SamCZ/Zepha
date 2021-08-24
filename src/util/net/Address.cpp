#include <stdexcept>

#include "Address.h"

Address Address::fromString(const std::string& addressString) {
	std::string address;
	unsigned short port;
	
	size_t sep = addressString.find(':');
	if (sep == std::string::npos) {
		port = Address::DEFAULT_PORT;
	}
	else {
		address = addressString.substr(0, sep++); // Increment sep for next substr call.
		unsigned int p = stoi(addressString.substr(sep, addressString.length() - sep));
		if (p > 65535) throw std::out_of_range("Port value is greater than 65535.");
		port = static_cast<unsigned short>(p);
	}
	
	return Address{ address, port };
}

string Address::toString() {
	return host + ":" + std::to_string(port);
}
