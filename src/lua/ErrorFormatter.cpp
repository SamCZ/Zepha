//
// Created by aurailus on 2020-02-11.
//

#include <vector>
#include <sstream>
#include <algorithm>

#include "ErrorFormatter.h"

#include "../util/Log.h"

std::string
ErrorFormatter::formatError(const string& fileName, usize line, const string& stack, string file,
	bool ansiColors) noexcept {
	const string red = (ansiColors ? Log::red : "");
	const string unbl = (ansiColors ? Log::unbl : "");
	const string endl = (ansiColors ? Log::endl : "\n");
	
	std::stringstream out{};
	
	// Split the file into lines, and add them to a vector
	vec<string> fileLines{};
	usize pos = 0;
	string token;
	
	while ((pos = file.find("\n")) != string::npos) {
		token = file.substr(0, pos);
		fileLines.push_back(token);
		file.erase(0, pos + 1);
	}
	fileLines.push_back(file);
	
	while (fileLines.back() == "") fileLines.pop_back();
	
	// Format and add lines to the stringstream
	for (usize i = (std::max)(static_cast<usize>(0), line - 6); i < (std::min)(fileLines.size(), line + 5); i++) {
		for (usize j = 0; j < 3 - std::to_string(i + 1).length(); j++) out << " ";
		out << red << (i + 1 == line ? unbl : "") << (i + 1) << (i + 1 == line ? " # " : " | ") << fileLines[i] << endl;
	}
	
	// Add the stack trace at the bottom
	out << endl << red << stack << endl;
	
	return out.str();
}
