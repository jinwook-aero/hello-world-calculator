// Command.h
// Header file for Command class
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 12, 2020
//

#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <vector>

class Command // Command is accumulated during build up
{
	public:
		Command() {}
		Command(const std::string& str);
		~Command() {}

	private:
		std::vector<char> cmdVector;
};

#endif