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
UltraDouble::UltraDouble() : precisionFactor{ 1 }, validityFlag{ false }
{
	// Initialize udv with all zeros
	udv.resize(precisionFactor, 0.0);
}

// Constructor with precision facotor is valid
UltraDouble::UltraDouble(int pFactor) : precisionFactor{ pFactor }, validityFlag{ true }
{
	// Initialize udv with all zeros
	udv.resize(precisionFactor, 0.0);
}


void UltraDouble::Set(std::vector<double> dv)
{
	for (int i = 0; i != precisionFactor; ++i)
		udv[i] = dv[i];
}

std::string UltraDouble::ToString ()
{
	udv_str.clear();
	if (validityFlag)
		for (const auto& val : udv)
			udv_str += std::to_string(val);

	return udv_str;
}

UltraDouble::operator const char* ()
{
	return ToString().c_str();
}

UltraDouble UltraDouble::operator+(UltraDouble& rhs)
{
	// Temporary routine using only the first element
	UltraDouble ud{ precisionFactor };
	std::vector<double> dv(precisionFactor, 0);
	dv[0] = this->udv[0] + rhs.udv[0];
	ud.Set(dv);
	return ud;
}

UltraDouble UltraDouble::operator-(UltraDouble& rhs)
{
	// Temporary routine using only the first element
	UltraDouble ud{ precisionFactor };
	std::vector<double> dv(precisionFactor, 0);
	dv[0] = this->udv[0] - rhs.udv[0];
	ud.Set(dv);
	return ud;
}

UltraDouble UltraDouble::operator*(UltraDouble& rhs)
{
	// Temporary routine using only the first element
	UltraDouble ud{ precisionFactor };
	std::vector<double> dv(precisionFactor, 0);
	dv[0] = this->udv[0] * rhs.udv[0];
	ud.Set(dv);
	return ud;
}

UltraDouble UltraDouble::operator/(UltraDouble& rhs)
{
	// Temporary routine using only the first element
	UltraDouble ud{ precisionFactor };
	std::vector<double> dv(precisionFactor, 0);
	dv[0] = this->udv[0] / rhs.udv[0];
	ud.Set(dv);
	return ud;
}
