// calculator.cpp
// Header file for Calculator class and its constituting classes
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 13, 2020
//

#include "calculator.h"
#include "StringCalculator.h"
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <cmath>
#include <memory>
#include <algorithm> // std::max_element

// Calculator constructor
// clear and properly size cmdHistory and ansHistory
Calculator::Calculator(int pFactor, int nHistory):
	precisionFactor_(pFactor),
	sizeHistory_(nHistory)
{
	cmdHistory_.clear(); cmdHistory_.resize(sizeHistory_,Command());
	ansHistory_.clear(); ansHistory_.resize(sizeHistory_,UltraDouble());
}

// INPUT_CMD_TYPE Calculator::ReadCommand()
// updates cmdCurrent and cmdHistory
// returns isCompleted
INPUT_CMD_TYPE Calculator::ReadCommand()
{

	// Reading command lines
	std::cout << "Build command line " << std::endl;
	std::cout << "Special action commdands: EXIT, EXECUTE, CLEAR, ALL_CLEAR, BACKSPACE" << std::endl;
	std::cout << "Operation commdands: (, ), +, -, *, /" << std::endl;
	std::cout << std::endl;

	// Accumulating single input to cmdCurrent
	// until EXIT or EXECUTE is entered
	// CLEAR or ALL_CLEAR performs local action
	std::string tempStr{""};
	cmdCurrent_.Clear(); // Clear current command object
	INPUT_CMD_TYPE input_cmd_type = INPUT_CMD_TYPE::UNDETERMINED;
	while ( (input_cmd_type != INPUT_CMD_TYPE::EXECUTE) &&
		    (input_cmd_type != INPUT_CMD_TYPE::EXIT) )
	{
		// Read single input
		tempStr.clear();
		std::cout << "Single input (number, action cmd, operator cmd): ";
		std::getline(std::cin,tempStr);
		if (tempStr == "EXIT")
		{
			input_cmd_type = INPUT_CMD_TYPE::EXIT;
			break;
		}
		else if (tempStr == "EXECUTE")
		{
			input_cmd_type = INPUT_CMD_TYPE::EXECUTE;
			break;
		}
		else if (tempStr == "CLEAR")
		{
			input_cmd_type = INPUT_CMD_TYPE::CLEAR;
			cmdCurrent_.Clear(); // Clear only current command object
		}
		else if (tempStr == "ALL_CLEAR")
		{
			input_cmd_type = INPUT_CMD_TYPE::ALL_CLEAR;
			// NOTE: without .clear(), cmdHistory still holds previous data
			// To Sungwook: Figure out why and fix this issue such that .clear() does not need to be invoked
			cmdHistory_.clear(); cmdHistory_.resize(sizeHistory_, Command()); // Clear and reinitialize command history
			ansHistory_.clear(); ansHistory_.resize(sizeHistory_, UltraDouble()); // Clear and reinitialize  answer history			
		}
		else if (tempStr == "BACKSPACE")
		{
			input_cmd_type = INPUT_CMD_TYPE::BACKSPACE;
			cmdCurrent_.Pop_back(); // Delete the most recent command element
		}
		else // NUMBER or OPERATOR, temporarilly assigned with UNDETERMINED
			input_cmd_type = INPUT_CMD_TYPE::UNDETERMINED;
		
		// Append command only for number or operator
		if (input_cmd_type == INPUT_CMD_TYPE::UNDETERMINED)
			cmdCurrent_.Append(tempStr);
	}

	// Update cmdHistory and pop the oldest history data
	cmdHistory_.push_back(cmdCurrent_);
	cmdHistory_.pop_front();

	// Return INPUT_CMD_TYPE
	return input_cmd_type;
}

// int Calculator::ExecuteCommand()
// cmdCurrent.ToString() is decoded and computed using StringCalculator
// character by character
// Each valid character is categorized to operator or number
// Each valid character is allocated with precedence number
// Calculation is unrolled from zero precedence  (lower precedence number computed earlier)
int Calculator::ExecuteCommand()
{
	// Compute string
	StringCalculator<UltraDouble> localCalculator{ precisionFactor_, cmdCurrent_.ToString() };
	std::pair<bool, UltraDouble> result{localCalculator.CalculateStr()};
	
	// Update ans history
	if (result.first)
		ansHistory_.push_back(result.second);
	else
		ansHistory_.push_back(0);
	ansHistory_.pop_front();
	return 0;
}

// int Calculator::DisplayStatus()
// Clears current display and shows the updated list of cmd and ans history
int Calculator::DisplayStatus()
{
	system("CLS");
	for (int i=0; i!=Calculator::sizeHistory_; ++i)
	{
		std::cout << "CMD: " << cmdHistory_[i].ToString() << std::endl;
		std::cout << "ANS: " << ansHistory_[i].ToString()  << std::endl;
		std::cout << std::endl;
	}
	return 0;
}

// int Calculator::Wait(int waitMilliseconds)
// Wait for the given miliseconds to alleviate burden on CPU
int Calculator::Wait(int waitMilliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(waitMilliseconds));
	return 0;
}
