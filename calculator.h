// calculator.h
// Header file for Calculator class and its constituting classes
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 11, 2020
//

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <cmath>
#include <vector>

class Calculator
{
	public:
		// Constructor and destructor
		Calculator(int pFactor); // Precision factor is set at construction
		~Calculator(){} // All pointers handled by smart pointers thus no need for resource initialization

		// Input handling
		bool readInput(unique_ptr<Command> cmd); // *cmd is updated with new input

		// Core of calculator
		bool executeCommand(unique_ptr<Command> cmd, unique_ptr<UltraDouble> res);

	private:
		vector<unique_ptr<Command>> cmdHistory; // Previous command list saved in container
		Command cmdCurrent; // Current command 
		int precisionFactor; // Amplification factor for significant digit beyond double precision
	friend class UltraDouble; //
};

class Command // Command is accumulated during build up
{	
	public:
		Command()
		~Command()

	private:
		vector<CommandElement> cmdInputElement;
}

class UltraDouble
{
	public:
		UltraDouble(){}
		~UltraDouble(){}
		UltraDouble operator+(UltraDouble, UltraDouble);
	private:
		vector<double> udv; // ultra double vector

}

#endif // End of include guard
