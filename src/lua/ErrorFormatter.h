//
// Created by aurailus on 2020-02-11.
//

#pragma once

#include <string>

class ErrorFormatter {
	public:
	static std::string formatError(const std::string& fileName, int line,
		const std::string& stack, std::string file, bool ansiColors = true) noexcept;
};
