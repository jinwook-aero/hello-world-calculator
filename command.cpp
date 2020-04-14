// command.cpp
// Source file for Command class
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : April 12, 2020
//

#include "Command.h"
#include "calculator.h"

// Command implementations
Command::Command()
{
	cmdVector.clear();
}

Command::Command(const std::string & str)
{
	cmdVector.clear();
	cmdVector.push_back(str);
}

void Command::Append(const std::string & str)
{
	cmdVector.push_back(str);
}

void Command::Clear()
{
	cmdVector.clear();
}

void Command::Pop_back()
{
	cmdVector.pop_back();
}

std::string Command::ToString()
{
	cmdVector_all.clear();
	for (int i = 0; i != cmdVector.size(); ++i)
	{
		cmdVector_all += " ";
		cmdVector_all += cmdVector.at(i);
	}
	return cmdVector_all;
}

Command::operator const char* ()
{
	ToString(); // This updates (std::string cmdVector_all)
	return cmdVector_all.c_str();
}


