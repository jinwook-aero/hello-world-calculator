// ultra_double.cpp
// Source file for UltraDouble
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : April 12, 2020
//

#include "ultra_double.h"

// Default constructor is invalid value
UltraDouble::UltraDouble(): precisionFactor{1}, validityFlag{false}
{
	// Initialize udv with all zeros
	udv.resize(precisionFactor, 0.0);
}

// Constructor with precision facot is valid
UltraDouble::UltraDouble(int pFactor): precisionFactor{pFactor}, validityFlag{true}
{
	// Initialize udv with all zeros
	udv.resize(precisionFactor, 0.0);
}

UltraDouble::operator const char* ()
{
	udv_str.clear();

	if (validityFlag)
		for (const auto& val : udv)
			udv_str += std::to_string(val);

	return udv_str.c_str();
}
