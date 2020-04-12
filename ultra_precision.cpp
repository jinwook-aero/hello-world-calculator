// ultra_precision.cpp
// Source file for UltraDouble
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : April 12, 2020
//

#ifndef ULTRA_PRECISION_H
#include "ultra_precision.h"
#endif

#include <string>
#include <cmath>

UltraDouble::UltraDouble(int pFactor=1):
	precisionFactor{pFactor}
{
	udv.resize(precisionFactor);
}
