// ultra_double.cpp
// Source file for UltraDouble
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
Command::Command(const std::string & str)
{
	cmdVector.resize(str.size());
	for (int i=0; i!=str.size(); ++i)
		cmdVector[i] = str[i];
}
