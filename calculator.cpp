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
	//deque<Command>     cmdHistory(sizeHistory);
	//deque<UltraDouble> ansHistory(sizeHistory);
	std::deque<std::string> cmdHistory(sizeHistory);
	std::deque<double> ansHistory(sizeHistory);
}

// Calculator::ReadCommand() 
// updates cmdCurrent and cmdHistory
// returns isCompleted
bool Calculator::ReadCommand()
{

	// Reading command lines
	std::string str{""};
	std::cout << "Enter command [EXIT to exit]: \n";
	std::getline(std::cin, str);
	Command cmdCurrent(str);

	// Update cmd history
	cmdHistory.push_back(cmdCurrent);
	cmdHistory.pop_front();

	// Return if completed
	if (str.find("EXIT") != std::string::npos)
		return true; // Completed
	return false;  // Not completed
}

int Calculator::ExecuteCommand()
{
	// Compute new answer
	//UltraDouble ansCurrent(Calculator::precisionFactor);
	double ansCurrent = 0;
	
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
		//std::cout << "CMD: " << cmdHistory[i] << std::endl;
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

