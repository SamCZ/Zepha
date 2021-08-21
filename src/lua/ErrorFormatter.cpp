//
// Created by aurailus on 2020-02-11.
//

#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "ErrorFormatter.h"

std::string ErrorFormatter::formatError(const string& fileName, usize line,
	const string& stack, string file, bool ansiColors) noexcept {
	const string RED = "`cf";
	const string LTGRAY = "`c1";
	const string GRAY = "`c2";
	const string BOLD = "`b";
	const string UNDL = "`u";
	const string ENDL = "`r\n";
	
	std::stringstream out {};
	
	out << BOLD << UNDL << fileName << ".lua" << ENDL << "\n";
	
	vec<string> fileLines {};
	usize pos = 0;
	string token;
	
	while ((pos = file.find("\n")) != string::npos) {
		token = file.substr(0, pos);
		fileLines.push_back(token);
		file.erase(0, pos + 1);
	}
	fileLines.push_back(file);
	
	while (fileLines.back() == "") fileLines.pop_back();
	
	usize printStart = (std::max)(0, static_cast<i32>(line - LOOK_AROUND - 1));
	usize printEnd = (std::min)(fileLines.size(), line + LOOK_AROUND);
	
	if (printStart != 0) {
		for (usize j = 0; j < 3 - std::to_string(printStart).length(); j++) out << " ` ` ";
		out << GRAY << printStart << " | -- snip --" << ENDL;
	}
	
	for (i32 i = printStart; i < printEnd; i++) {
		for (usize j = 0; j < 3 - std::to_string(i + 1).length(); j++) out << " ` ` ";
		out << (i + 1 == line ? RED : LTGRAY) << (i + 1)
			<< (i + 1 == line ? "" : "`cr")
			<< " | " << (i + 1 == line ? BOLD : "")
			<< fileLines[i] << ENDL;
	}
		
	if (printEnd != fileLines.size()) {
		for (usize j = 0; j < 3 - std::to_string(printEnd + 1).length(); j++) out << " ` ` ";
		out << GRAY << (printEnd + 1) << " | -- snip --" << ENDL;
	}
	
	out << "\n" << BOLD << UNDL << "Traceback" << ENDL << "\n" << RED << stack << ENDL;
	
	return out.str();
}
