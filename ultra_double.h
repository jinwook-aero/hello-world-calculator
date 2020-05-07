// ultra_double.h
// Header file for UltraDouble
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : April 12, 2020
//

#pragma once
#include <string>
#include <vector>

// Number and arithmetic operation
// Precision is directly read from calculator object
class UltraDouble
{
public:
	UltraDouble();
	UltraDouble(int pFactor);
	~UltraDouble() = default;
	void Set(std::vector<double> dv);
	UltraDouble operator+(UltraDouble&);
	UltraDouble operator-(UltraDouble&);
	UltraDouble operator*(UltraDouble&);
	UltraDouble operator/(UltraDouble&);

	// Conversion operator for display
	std::string ToString();
	operator const char* ();

private:
	int precisionFactor;
	bool validityFlag; // Validity of current object
	std::vector<double> udv; // Ultra precision value represented as double vector
	std::string udv_str; // Ultra precision value in string for display
};
