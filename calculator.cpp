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

#include <chrono>
#include <thread>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

// Calculator implementations
Calculator::Calculator(int pFacotr, nHistory):
	precisionFactor(pFactor),
	sizeHistory(nHistory)
{
	deque<unique_ptr<Command>>     cmdHistory(sizeHistory) = {new Command    };
	deque<unique_ptr<UltraDouble>> ansHistory(sizeHistory) = {new UltraDouble};
}

// Calculator::ReadCommand() 
// updates cmdCurrent and cmdHistory
// returns isCompleted
bool Calculator::ReadCommand()
{

	// Reading command lines
	String str{""};
	cout << "Enter command [EXIT to exit]: \n";
	getline(cin, str);
	cmdCurrent(str);

	// Update cmd history
	cmdHistory.push_back(cmdCurrent);
	cmdHistory.pop_front();

	// Return if completed
	if str.find("EXIT") != string::npos
		return true; // Completed
	return false;  // Not completed
}

Calculator::ExecuteCommand()
{
	// Compute new answer
	UltraDouble ansCurrent(Calculator::precisionFactor);
	
	// Update ans history
	ansHistory.push_back(ansCurrent);
	ansHistory.pop_front();
}

Calculator::Display()
{
	system("CLS");
	for (int i=0; i!=Calculator::sizeHistory; ++i)
	{
		cout << "CMD: " << cmdHistory[i] << endl;
		cout << "ANS: " << ansHistory[i] << endl;
		cout << endl;
	}
}

Calculator::Wait(int waitMilliseconds)
{
	std::this_thread::sleep_for(std::chrno:miliseconds(waitMilliseconds);
}

// Command implementations
Command::Command(){}
Command::Command(const String & str)
{
	cmdVector.resize(str.size());
	for (int i=0; i!=String.size(); ++i)
		cmdInputElement[i] = str[i];
}

