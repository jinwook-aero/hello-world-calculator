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
#include "ultra_precision.h"
using namespace std;

int main()
{
	// Calculator intialization
	int precisionFactor = 1;  // Default precision factor
	int sizeHistory     = 10; // Default size of history

	// User input
	cout << "Hello world.\n";
	cout << "Enter precision factor [e.g. 1]: ";
	cin >> precisionFactor;
	cout << "Enter size of history [e.g. 10]: ";
	cin >> sizeHistory;

	// Create Calculator
	Calculator theCalc{precisionFactor,sizeHistory};

	// Run calculator
	bool isCompleted = false;
	while(isCompleted)
	{
		isCompleted = theCalc.ReadCommand();
		theCalc.ExecuteCommand();
		theCalc.DisplayStatus();	
		theCalc.Wait();
	}

	// End of program
	return 0;
}
