// main.cpp
// Main file for ultra precision calculator
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 11, 2020
// Last update  : April 12, 2020
//

#include <iostream>
#include "calculator.h"
#include "ultra_double.h"

int main()
{
	// Calculator intialization
	int precisionFactor = 1;  // Default precision factor
	int sizeHistory     = 10; // Default size of history

	// User input
	std::cout << "Hello world.\n";
	std::cout << "Enter precision factor [e.g. 1]: ";
	std::cin >> precisionFactor;
	std::cout << "Enter size of history [e.g. 10]: ";
	std::cin >> sizeHistory;
	std::cin.ignore(1); // to flush "enter" from the buffer

	// Create Calculator
	Calculator theCalc{precisionFactor,sizeHistory};

	// Run calculator
	INPUT_CMD_TYPE currentINPUT = INPUT_CMD_TYPE::EXECUTE;
	while(currentINPUT != INPUT_CMD_TYPE::EXIT)
	{
		currentINPUT = theCalc.ReadCommand();
		if (currentINPUT == INPUT_CMD_TYPE::EXIT)
			break;
		theCalc.ExecuteCommand();
		theCalc.DisplayStatus();	
		theCalc.Wait();
	}

	// End of program
	return 0;
}
