// ultra_double.cpp
// Source file for UltraDouble
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : April 12, 2020
//

#ifndef ULTRA_DOUBLE_H
#include "ultra_double.h"
#endif

#include <string>
#include <cmath>

UltraDouble::UltraDouble(int pFactor):
	precisionFactor{pFactor}
{
	// Initialize with all zeros
	udv.clear();
	udv.resize(precisionFactor);
	for (int i = 0; i != udv.size(); ++i)
		udv[i] = 0.0;
}

UltraDouble::operator const char* ()
{
	udv_str.clear();
	for (int i = 0; i != udv.size(); ++i)
		udv_str += std::to_string(udv[i]);
	return udv_str.c_str();
}
