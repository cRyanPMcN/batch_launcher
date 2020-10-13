/*
File: rm_lib.hpp
Author: Ryan McNamee
Date Created: Monday, 9, September, 2017
Date Updated: Monday, 9, October, 2017
Purpose: Holds reusable non-member functions
*/
#ifndef RM_LIB_HPP_HEADER_GUARD
#define RM_LIB_HPP_HEADER_GUARD
#include <fstream>
#include <string>

namespace rm {
	// Method: GetLine
	// Purpose: Parses a single line from filestream into input wstring
	// Inputs: input file, output string
	// Outputs: filestream found a newline character 
	inline bool GetLine(std::wifstream& file, std::wstring& output) noexcept {
		wchar_t ch;
		while (file.get(ch)) {
			if (ch == '\n') {
				break;
			}
			output += ch;
		}
		return ch == '\n';
	}

	// Method: GetLine
	// Purpose: Parses a single line from filestream into wstring until delimiter is reached
	// Input: input file, output string, delimiter
	// Output: delimiter was reached before the end of the line
	inline bool GetLine(std::wifstream& file, std::wstring& output, wchar_t delim) noexcept {
		wchar_t ch;
		while (file.get(ch)) {
			if (ch == delim || ch == '\n') {
				break;
			}
			output += ch;
		}
		return ch == delim;
	}
}

#endif