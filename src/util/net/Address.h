#pragma once

#include <string>

#include "util/Types.h"

/**
 * Represents an IP address + Port pair.
 */

class Address {
public:

	/**
	 * Returns an Address object from the provided address string.
	 * Address string should be in standard dot + optional colon notation, e.g: 192.168.0.1:8000, 127.0.0.1.
	 * @throws std::invalid_argument - There is a value after a colon that cannot be converted into a valid numeric value.
	 * @throws std::out_of_range - There is a numeric port specified, but it is greater than the maximum unsigned short.
	 * @param addressString - The string to parse.
	 * @returns - An Address object with the specified address.
	 */
	 
	static Address fromString(const string& addressString);
	
	string toString();
	
	string host;
	u16 port = Address::DEFAULT_PORT;
	
	constexpr static u16 DEFAULT_PORT = 13110;
};
