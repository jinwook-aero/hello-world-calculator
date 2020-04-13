// calculator.cpp
// Header file for Calculator class and its constituting classes
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 12, 2020
//

#ifndef ULTRA_DOUBLE_H
#include "ultra_double.h"
#endif
#ifndef CALCULATOR_H
#include "calculator.h"
#endif

#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string>
#include <cmath>
#include <vector>
#include <deque>
#include <memory>

// Calculator implementations
Calculator::Calculator(int pFactor, int nHistory):
	precisionFactor(pFactor),
	sizeHistory(nHistory)
{
	cmdHistory.clear(); cmdHistory.resize(sizeHistory,Command());
	ansHistory.clear(); ansHistory.resize(sizeHistory,UltraDouble());
}

// Calculator::ReadCommand() 
// updates cmdCurrent and cmdHistory
// returns isCompleted
INPUT_CMD_TYPE Calculator::ReadCommand()
{

	// Reading command lines
	std::cout << "Build command line " << std::endl;
	std::cout << "Special action commdands: EXIT, EXECUTE, CLEAR, ALL_CLEAR" << std::endl;
	std::cout << "Operation commdands: PLUS, MINUS, MULTIPLY, DIVIDE" << std::endl;
	std::cout << std::endl;

	// Accumulating single input to cmdCurrent
	// untile EXIT or EXECUTE is entered
	// CLEAR or ALL_CLEAR performs local action
	std::string tempStr{""};
	Command cmdCurrent; // Empty command object
	INPUT_CMD_TYPE input_cmd_type = INPUT_CMD_TYPE::UNDETERMINED;
	while ( (input_cmd_type != INPUT_CMD_TYPE::EXECUTE) &&
		    (input_cmd_type != INPUT_CMD_TYPE::EXIT) )
	{
		// Read single input
		tempStr.clear();
		std::cout << "Single input (number, action command, operator): ";
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
			// NOTE: without .clear(), cmdHistory can still hold previous data
			cmdHistory.clear(); cmdHistory.resize(sizeHistory, Command()); // Clear and reinitialize command history
			ansHistory.clear(); ansHistory.resize(sizeHistory, UltraDouble()); // Clear and reinitialize  answer history			
		}
		else // NUMBER or OPERATOR, temporarilly assigned with UNDETERMINED
			input_cmd_type = INPUT_CMD_TYPE::UNDETERMINED;
		
		// Append command only for number or operator
		if (input_cmd_type == INPUT_CMD_TYPE::UNDETERMINED)
			cmdCurrent.Append(tempStr);
	}

	// Update cmdHistory and pop the old history data
	cmdHistory.push_back(cmdCurrent);
	cmdHistory.pop_front();

	// Return INPUT_CMD_TYPE
	return input_cmd_type;
}

int Calculator::ExecuteCommand()
{
	// Parsing command line

	// Compute new answer
	UltraDouble ansCurrent(precisionFactor);
	
	// Update ans history
	ansHistory.push_back(ansCurrent);
	ansHistory.pop_front();

	return 0;
}

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

int Calculator::Wait(int waitMilliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(waitMilliseconds));
	return 0;
}