//
// Created by aurailus on 2020-02-11.
//

#pragma once

#include "util/Types.h"

class ErrorFormatter {
public:
	static string formatError(const string& fileName, usize line,
		const string& stack, string file, bool ansiColors = true) noexcept;
};
