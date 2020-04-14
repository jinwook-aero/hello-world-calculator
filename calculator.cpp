// calculator.cpp
// Header file for Calculator class and its constituting classes
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 13, 2020
//

#include "ultra_double.h"
#include "calculator.h"
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string>
#include <cmath>
#include <vector>
#include <deque>
#include <memory>
#include <algorithm> // std::max_element

// Calculator constructor
// clear and properly size cmdHistory and ansHistory
Calculator::Calculator(int pFactor, int nHistory):
	precisionFactor(pFactor),
	sizeHistory(nHistory)
{
	cmdHistory.clear(); cmdHistory.resize(sizeHistory,Command());
	ansHistory.clear(); ansHistory.resize(sizeHistory,UltraDouble());
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
	// untile EXIT or EXECUTE is entered
	// CLEAR or ALL_CLEAR performs local action
	std::string tempStr{""};
	cmdCurrent.Clear(); // Clear current command object
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
			cmdCurrent.Clear(); // Clear only current command object
		}
		else if (tempStr == "ALL_CLEAR")
		{
			input_cmd_type = INPUT_CMD_TYPE::ALL_CLEAR;
			// NOTE: without .clear(), cmdHistory still holds previous data
			// To Sungwook: Figure out why and fix this issue such that .clear() does not need to be invoked
			cmdHistory.clear(); cmdHistory.resize(sizeHistory, Command()); // Clear and reinitialize command history
			ansHistory.clear(); ansHistory.resize(sizeHistory, UltraDouble()); // Clear and reinitialize  answer history			
		}
		else if (tempStr == "BACKSPACE")
		{
			input_cmd_type = INPUT_CMD_TYPE::BACKSPACE;
			cmdCurrent.Pop_back(); // Delete the most recent command element
		}
		else // NUMBER or OPERATOR, temporarilly assigned with UNDETERMINED
			input_cmd_type = INPUT_CMD_TYPE::UNDETERMINED;
		
		// Append command only for number or operator
		if (input_cmd_type == INPUT_CMD_TYPE::UNDETERMINED)
			cmdCurrent.Append(tempStr);
	}

	// Update cmdHistory and pop the oldest history data
	cmdHistory.push_back(cmdCurrent);
	cmdHistory.pop_front();

	// Return INPUT_CMD_TYPE
	return input_cmd_type;
}

// int Calculator::ExecuteCommand()
// cmdCurrent.ToString() is decoded character by character
// Each valid character is categorized to operator or number
// Each valid character is allocated with precedence number
// Calculation is unrolled from zero precedence  (lower precedence number computed earlier)
int Calculator::ExecuteCommand()
{
	// Identify operators and numbers
	std::string cmdStr = cmdCurrent.ToString();
	std::vector<INPUT_CMD_TYPE> cmdTypeVec;	
	std::vector<char> cmdCharVec;

	int cmdTypeCount{0};
	bool isValid = true;
	for (unsigned strIndex = 0 ; strIndex != cmdStr.length(); ++strIndex)
	{
		// Identify operators
		isValid = true; // Assume valid
		if (cmdStr.at(strIndex) == '(')
			cmdTypeVec.push_back(INPUT_CMD_TYPE::OPEN_BRACKET);
		else if (cmdStr.at(strIndex) == ')')
			cmdTypeVec.push_back(INPUT_CMD_TYPE::CLOSE_BRACKET);
		else if (cmdStr.at(strIndex) == '+')
			cmdTypeVec.push_back(INPUT_CMD_TYPE::PLUS);
		else if (cmdStr.at(strIndex) == '-')
			cmdTypeVec.push_back(INPUT_CMD_TYPE::MINUS);
		else if (cmdStr.at(strIndex) == '*')
			cmdTypeVec.push_back(INPUT_CMD_TYPE::MULTIPLY);
		else if (cmdStr.at(strIndex) == '/')
			cmdTypeVec.push_back(INPUT_CMD_TYPE::DIVIDE);
		else if ((cmdStr.at(strIndex) == '.') ||
			 (cmdStr.at(strIndex) == '0') ||
			 (cmdStr.at(strIndex) == '1') ||
			 (cmdStr.at(strIndex) == '2') ||
			 (cmdStr.at(strIndex) == '3') ||
			 (cmdStr.at(strIndex) == '4') ||
			 (cmdStr.at(strIndex) == '5') ||
			 (cmdStr.at(strIndex) == '6') ||
			 (cmdStr.at(strIndex) == '7') ||
			 (cmdStr.at(strIndex) == '8') ||
			 (cmdStr.at(strIndex) == '9'))
			cmdTypeVec.push_back(INPUT_CMD_TYPE::NUMBER);
		else
			isValid = false; // This is not valid character

		// Append to cmdCharVec only for valid character
		if (isValid)
			cmdCharVec.push_back(cmdStr.at(strIndex));
	}

	// Do nothing and return if insufficient input
	if (cmdCharVec.size() == 0)
		return 0;

	// Bracket group is first determined before allocating precedence number
	// Example command: 1 + ( 2 + ( 4 + 5 ) ) + 6
	// Bracket group  : 0 0 - 1 1 - 2 2 2 - - 0 0
	int currentBracketGroup{0};
	std::vector<int> bracketGroupVec(cmdTypeVec.size(),currentBracketGroup);
	for (unsigned int vecIndex = 0; vecIndex != cmdTypeVec.size(); ++vecIndex)
	{
		if (cmdTypeVec.at(vecIndex) == INPUT_CMD_TYPE::OPEN_BRACKET)
		{
			++currentBracketGroup;
			unsigned int bracket2close{0};
			for (unsigned int vecIndex2 = vecIndex; vecIndex2 != cmdTypeVec.size(); ++vecIndex2)
			{
				if (cmdTypeVec.at(vecIndex2) == INPUT_CMD_TYPE::OPEN_BRACKET)
				{
					++bracket2close;
					bracketGroupVec.at(vecIndex2) = -1;
				}
				else if (cmdTypeVec.at(vecIndex2) == INPUT_CMD_TYPE::CLOSE_BRACKET)
				{
					--bracket2close;
					bracketGroupVec.at(vecIndex2) = -1;
				}
				else
					bracketGroupVec.at(vecIndex2) = currentBracketGroup;
				
				if (!bracket2close)
					break;
			}
		}
			
	}
	unsigned int maxBracketGroup = *std::max_element(bracketGroupVec.cbegin(), bracketGroupVec.cend());

	// Bracket group debug
	for (std::vector<char>::const_iterator vecIndex = cmdCharVec.cbegin() ; vecIndex != cmdCharVec.cend(); ++vecIndex)
		std::cout << *vecIndex << " ";
	std::cout << std::endl;

	for (std::vector<int>::const_iterator vecIndex = bracketGroupVec.cbegin() ; vecIndex != bracketGroupVec.cend(); ++vecIndex)
	{
		if (*vecIndex < 0)
			std::cout << "-" << " ";
		else
			std::cout << *vecIndex << " ";
	}
	std::cout << std::endl;

	// Allocate precedence
	// Each cmdCharVec character is allocated with precedence number
	// Lower precedence number is computed earlier
	std::vector<unsigned int> cmdCompactPrecedence(cmdCharVec.size(),0); 
	unsigned int maxPrecedence = *std::max_element(cmdCompactPrecedence.cbegin(), cmdCompactPrecedence.cend());

	// Compute new answer by unrolling operations from max precedence
	UltraDouble ansCurrent(precisionFactor);
	
	// Update ans history and return
	ansHistory.push_back(ansCurrent);
	ansHistory.pop_front();
	return 0;
}

// int Calculator::DisplayStatus()
// Clears current display and shows the updated list of cmd and ans history
int Calculator::DisplayStatus()
{
	system("CLS");
	for (int i=0; i!=Calculator::sizeHistory; ++i)
	{
		std::cout << "CMD: " << cmdHistory[i] << std::endl;
		std::cout << "ANS: " << ansHistory[i] << std::endl;
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
