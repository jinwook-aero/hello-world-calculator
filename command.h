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
#include <type_traits>

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
	operator const char*();

private:
	std::string commandStrList_;
};

template <typename T> 
typename std::enable_if_t<std::is_arithmetic_v<T>, std::string>
CommandBase<T>::ToString()
{
	commandStrList_.clear();

	for (auto iter = std::vector<T>::begin(); iter != std::vector<T>::end(); iter++) 
	{
		commandStrList_ += std::to_string(*iter);
		commandStrList_ += " ";
	}

	return commandStrList_;
}

template <typename T>
std::string CommandBase<T>::ToString()
{
	commandStrList_.clear();
	commandStrList_ = std::string("can not convert ") + typeid(T).name() + std::string(" type to string");
	return commandStrList_;
}

template <typename T>
CommandBase<T>::operator const char* ()
{
	ToString(); // This updates (std::string cmdVector_all)
	return commandStrList_.c_str();
}


// Command is vector of strings
// Each string can be number, digit, operator, or execution command
// cmdVector is accumulated during build up
class Command : public CommandBase<std::string> {};

// sungwook new command
class SungwookCommand : public CommandBase<int> {};

// gomdori command (2 Dimensional Array)
class GomdoriCommand : public CommandBase<std::vector<int>> {};
