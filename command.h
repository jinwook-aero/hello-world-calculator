// Command.h
// Header file for Command class
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 12, 2020
//

#pragma once
#include <iostream>
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

	std::string ToString() 
	{ 
		if (std::is_same_v<T, std::string>)
			return ToString_alreadyString();
		else
			return ToString(std::is_arithmetic<T>());
	}

	// Conversion operator for display
	operator const char*();

private:
	std::string ToString_alreadyString();  // Already string
	std::string ToString(std::true_type);  // Can be converted
	std::string	ToString(std::false_type); // Cannot be converted

	std::string commandStrList_;
};

template <typename T> // already string
std::string CommandBase<T>::ToString_alreadyString()
{
	commandStrList_.clear();

	for (auto iter = std::vector<T>::begin(); iter != std::vector<T>::end(); iter++)
	{
		commandStrList_ += *iter;
		commandStrList_ += " ";
	}

	return commandStrList_;
}

template <typename T> // is_arithmetic
std::string CommandBase<T>::ToString(std::true_type)
{
	commandStrList_.clear();

	for (auto iter = std::vector<T>::begin(); iter != std::vector<T>::end(); iter++)
	{
		commandStrList_ += std::to_string(*iter); // std::to_string cannot be called on string
		commandStrList_ += " ";
	}

	return commandStrList_;
}

template <typename T> // !is_arithmetic
std::string CommandBase<T>::ToString(std::false_type)
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
