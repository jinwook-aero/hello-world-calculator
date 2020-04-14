// calculator.cpp
// Header file for Calculator class and its constituting classes
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 12, 2020
//

#pragma once
#include "ultra_double.h"
#include "Command.h"
#include <chrono>
#include <thread>
#include <queue>

enum class INPUT_CMD_TYPE { ZERO = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
				DOT = 10, NUMBER,
			    PLUS = 20, MINUS, MULTIPLY, DIVIDE,
			    OPEN_BRACKET = 30, CLOSE_BRACKET,
			    EXIT = 40, CLEAR, ALL_CLEAR, BACKSPACE, EXECUTE, 
			    UNDETERMINED = 100 };

class Calculator
{
	public:
		// Constructor and destructor
		Calculator(int pFactor=1, int nHistory=10); // Precision factor is set at construction
		~Calculator() = default;

		// Input accumulation
		INPUT_CMD_TYPE ReadCommand(); // cmdCurrent is updated

		// Parsing command line and execute command
		int ExecuteCommand(); // Execute current command and update cmdHistory and ansHistory

		// Display current status
		int DisplayStatus();

		int Wait(int waitMilliseconds = 10);

	private:
		std::deque<Command>     cmdHistory; // Command history
		std::deque<UltraDouble> ansHistory; // Answer history

		Command cmdCurrent; // Current command 
		UltraDouble ansCurrent; // Current answer

		int precisionFactor; // Amplification factor for significant digit beyond double precision
		int sizeHistory; // Size of history to be recorded
};
