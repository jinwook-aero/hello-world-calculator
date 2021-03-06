// ultra_double.h
// Header file for UltraDouble
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : May 9, 2020
//

#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

// Number and arithmetic operation
// Precision is directly read from calculator object
class UltraDouble
{
public:
	UltraDouble();
	UltraDouble(int pFactor);
	UltraDouble(const UltraDouble & ud);
	~UltraDouble() = default;
	void Set(std::string str);
	void Set(int sign, int order, std::vector<int8_t> udv);
	std::string Get();
	bool operator==(const UltraDouble&);
	bool operator<(const UltraDouble&);
	bool operator>(const UltraDouble&);
	bool operator<=(const UltraDouble&);
	bool operator>=(const UltraDouble&);
	UltraDouble operator+(const UltraDouble&);
	UltraDouble operator-(const UltraDouble&);
	UltraDouble operator*(const UltraDouble&);
	UltraDouble operator/(const UltraDouble&);

	// Conversion operator for display
	std::string ToString();
	operator const char* ();

private:
	// Ultra Double is a decimal number
	// in the format of (sign) X (a.bcd) X (10^order)
	// 1) (int) sign_ indicates if the Ultra Double is positive or negative
	// 2) (int) order_ indicates the exponent of the Ultra Double
	// 3) (int) precisionFactor_ sets udv.size() = precisionFactor_;
	// 4) (std::vector<int8_t>) udv_ saves a.bcd as udv_[0] = a, udv_[1] = b, ...
	bool validityFlag_; // Validity of current object
	int sign_;
	int order_;
	int precisionFactor_;
	std::vector<int8_t> udv_; // Ultra precision value represented as integer vector
	std::string udv_str_; // Ultra precision value in string for display

	// Private functions
	UltraDouble FlipSign(const UltraDouble& rhs); // Returns copy of rhs where sign is flipped
	UltraDouble Absolute(const UltraDouble& rhs); // Returns copy of rhs where sign is always positive
	UltraDouble PushOrder(const UltraDouble& rhs, int deltaOrder); // Returns copy of rhs where order is pushed by deltaOrder
	int PushCarry(std::vector<int8_t>& iv); // iv[i]>=10 is pushed, returns order change
	int PullCarry(std::vector<int8_t>& iv); // iv[i]<0 is pulled, returns order change
	bool IsValid(const UltraDouble& rhs); // Returns validityFlag_
	bool IsZero(const UltraDouble& rhs); // Returns if zero
};
