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
#define CALCULATOR_H

#include <chrono>
#include <thread>
#include <string>
#include <cmath>
#include <vector>

class Calculator
{
	public:
		// Constructor and destructor
		Calculator(int pFactor=1, int nHistory=10); // Precision factor is set at construction
		~Calculator(){} 

		// Input accumulation
		bool ReadCommand(); // cmdCurrent is updated

		// Parsing command line and execute command
		bool ExecuteCommand(); // Execute current command and update cmdHistory and ansHistory

		void Wait(int waitMilliseconds=10)

	private:
		deque<unique_ptr<Command>>     cmdHistory; // Command history
		deque<unique_ptr<UltraDouble>> ansHistory; // Answer history

		Command     cmdCurrent; // Current command 
		UltraDouble ansCurrent; // Current answer

		int precisionFactor; // Amplification factor for significant digit beyond double precision
		int sizeHistory; // Size of history to be recorded

	friend class UltraDouble; //
};

class Command // Command is accumulated during build up
{	
	public:
		Command();
		~Command(){}

	private:
		vector<Char> cmdVector;
}


#endif // End of include guard
