// Command.h
// Header file for Command class
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 12, 2020
//

#pragma once
#include "utils.h"

template <typename T>
class CommandBase : protected std::vector<T>
{
public:
	CommandBase() = default;
	CommandBase(const T& val)	{ std::vector<T>::push_back(val);	}
	
	void Append(const T& val)	{ std::vector<T>::push_back(val);	}
	void Clear()				{ std::vector<T>::clear();			}
	void Pop_back()				{ std::vector<T>::pop_back();		}

	std::string ToString();

	// Conversion operator for display
	operator const char*()		{ return ToString().c_str(); }

private:
	std::string commandStrList_;
};

template <typename T>
std::string CommandBase<T>::ToString()
{
	commandStrList_.clear();

	for (auto iter = std::vector<T>::begin(); iter != std::vector<T>::end(); iter++)
	{
		commandStrList_ += ToStringConverter<T>::GetInstance()->ToString(*iter);
		commandStrList_ += " ";
	}

	return commandStrList_;
}


// Command is vector of strings
// Each string can be number, digit, operator, or execution command
// cmdVector is accumulated during build up
class Command : public CommandBase<std::string> {};

// sungwook new command
class SungwookCommand : public CommandBase<int> {};

// gomdori command (2 Dimensional Array)
class GomdoriCommand : public CommandBase<std::vector<int>> {};
