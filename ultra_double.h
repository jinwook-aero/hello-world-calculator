// ultra_double.h
// Header file for UltraDouble
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : April 12, 2020
//

#ifndef ULTRA_DOUBLE_H
#define ULTRA_DOUBLE_H

#include <string>
#include <vector>
#include <cmath>

// Number and arithmetic operation
// Precision is directly read from calculator object
class UltraDouble
{
	public:
		UltraDouble(){}
		UltraDouble(int pFactor);
		~UltraDouble(){}
		// UltraDouble operator+(UltraDouble, UltraDouble);

		// Conversion operator for display
		operator const char* ();
	private:
		int precisionFactor;
		std::vector<double> udv; // Ultra precision value represented as double vector
		std::string udv_str; // Ultra precision value in string for display
};

#endif // End of include guard

