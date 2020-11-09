//
// Created by aurailus on 11/07/19.
//

#pragma once

#include <string>

struct Address {
	std::string host;
	unsigned short port = Address::DEFAULT_PORT;
	
	static Address fromString(const std::string& addressString);
	
	constexpr static unsigned short DEFAULT_PORT = 13110;
};
