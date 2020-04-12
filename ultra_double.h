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
		UltraDouble(int pFactor=1);
		~UltraDouble(){}
		// UltraDouble operator+(UltraDouble, UltraDouble);
	private:
		int precisionFactor;
		std::vector<double> udv; // Ultra precision value represented as double vector
};

#endif // End of include guard

