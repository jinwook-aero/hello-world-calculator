// Command.h
// Header file for Command class
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 12, 2020
//

#pragma once
#include <string>
#include <vector>

// Command is vector of strings
// Each string can be number, digit, operator, or execution command
// cmdVector is accumulated during build up
class Command 
{
public:
	Command();
	Command(const std::string& str); // Constructor with starting point
	~Command() = default;

	void Append(const std::string &str);
	void Clear();
	void Pop_back();
	std::string ToString();

	// Conversion operator for display
	operator const char*();

private:
	// All data is cmdVector
	std::vector<std::string> cmdVector;
	std::string cmdVector_all;
};
