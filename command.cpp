// command.cpp
// Source file for Command class
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : April 12, 2020
//

#ifndef COMMAND_H
#include "Command.h"
#endif

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

Command::operator const char*()
{
	std::string tempStr;
	for (int i=0; i!=cmdVector.size(); ++i)
		tempStr += cmdVector[i];
	return tempStr.c_str();
}


