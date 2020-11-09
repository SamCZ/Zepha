//
// Created by auri on 2020-11-03.
//

#include <stdexcept>

#include "Address.h"


/**
 * Returns an Address object from the provided address string.
 * Address string should be in standard dot + optional colon notation, e.g: 192.168.0.1:8000, 127.0.0.1.
 * @throws std::invalid_argument - There is a value after a colon that cannot be converted into a valid numeric value.
 * @throws std::out_of_range - There is a numeric port specified, but it is greater than the maximum unsigned short.
 * @param addressString - The string to parse.
 * @returns - An Address object with the specified address.
 */

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
