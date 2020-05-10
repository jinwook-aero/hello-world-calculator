// ultra_double.cpp
// Source file for UltraDouble
//
// Author  : Jinwook Lee
// Reviewer: Sungwook Lee
// First version: April 12, 2020
// Last update  : May 9, 2020
//

#include "ultra_double.h"

// Default constructor is invalid value
UltraDouble::UltraDouble() : 
	precisionFactor_{ 1 }, validityFlag_{ false }, sign_{ 1 }, order_{ 0 }
{
	// Initialize udv with all zeros
	udv_.resize(precisionFactor_, 0);
}

// Constructor with precision facotor is valid
UltraDouble::UltraDouble(int pFactor) :
	precisionFactor_ { pFactor }, validityFlag_{ true }, sign_{ 1 }, order_{ 0 }
{
	// Initialize udv with all zeros
	udv_.resize(precisionFactor_, 0);
}

void UltraDouble::Set(std::string str)
{
	// sign recognition
	if (str.at(0) == '-') {
		sign_ = -1;
		str = str.substr(1, str.size() - 1);
	}
	else
		sign_ = +1;

	// Clear redundant pre-zero's such as: 
	// 000012.345 -> 12.345 or 00.123 -> .123
	for (size_t i = 0; i < str.length(); ++i) {
		if (str.at(i) != '0') {
			str = str.substr(i, str.size()-i);
			break;
		}
	}

	// digit recognition
	if (str.at(0) == '.') // starting with .000abcd
		order_ = 0; // Will be decremented further until not zero
	else
		order_ = str.length()-1; // If '.' does not present
	for (size_t i = 0; i < str.length(); ++i) {
		if (order_ <= 0){
			if ((str.at(i) == '0') || (str.at(i) == '.')) {
				--order_;
			}
			else
				break;
		}
		else // format of ab.xyz
			if (str.at(i) == '.') {
				order_ = i-1;
				break;
			}
	}

	// Remove '.' and all preceding 0: 0.001234 -> 1234
	str.erase(std::remove(str.begin(), str.end(), '.'), str.end()); // '.' is not needed anymore
	for (size_t i = 0; i < str.length(); ++i) {
		if (str.at(i) != '0') {
			str = str.substr(i, str.size() - i);
			break;
		}
	}

	// Fill in vector
	std::vector<int8_t> dv(precisionFactor_, 0);
	for (size_t i = 0; i < dv.size(); ++i) {
		if (i < str.length()){
			std::string tempStr{ str.at(i) };
			dv[i] = stoi(tempStr); // const char * to int
		}
		else
			dv[i] = 0;
	}
	udv_ = dv;
}

const std::vector<int8_t> UltraDouble::Get() const
{
	return udv_;
}

std::string UltraDouble::ToString ()
{
	// Count zero appending: ex) abcd0000 (+4) or 0.0000abcd (-5)
	int zeroAppend;
	if ( order_ <0 )
		zeroAppend = order_;
	else{
		zeroAppend = order_ - (precisionFactor_ - 1);
		if (zeroAppend < 0)
			zeroAppend = 0;
	}

	udv_str_.clear();
	if (validityFlag_){
		// Sign appending
		if (sign_ < 0)
			udv_str_ += "-";

		// Pre-zero appending
		if (zeroAppend <0){
			udv_str_ += "0.";
			for (++zeroAppend; zeroAppend!=0; ++zeroAppend)
				udv_str_ += "0";
		}

		// Number printing
		for (size_t i = 0; i != udv_.size(); ++i) {
			udv_str_ += std::to_string(udv_.at(i));
			if (i==order_)
				udv_str_ += ".";
		}

		// Post-zero appending
		if (zeroAppend > 0) {
			for (; zeroAppend != 0; --zeroAppend)
				udv_str_ += "0";
		}
	}

	return udv_str_;
}

UltraDouble::operator const char* ()
{
	return ToString().c_str();
}

UltraDouble UltraDouble::operator+(const UltraDouble& rhs)
{
	// Temporary routine using only the first element
	UltraDouble ud{ precisionFactor_ };
	std::vector<int8_t> dv(precisionFactor_, 0);
	dv[0] = this->udv_[0] + rhs.udv_[0];
	//ud.Set(dv);
	return ud;
}

UltraDouble UltraDouble::operator-(const UltraDouble& rhs)
{
	// Temporary routine using only the first element
	UltraDouble ud{ precisionFactor_ };
	std::vector<int8_t> dv(precisionFactor_, 0);
	dv[0] = this->udv_[0] - rhs.udv_[0];
	//ud.Set(dv);
	return ud;
}

UltraDouble UltraDouble::operator*(const UltraDouble& rhs)
{
	// Temporary routine using only the first element
	UltraDouble ud{ precisionFactor_ };
	std::vector<int8_t> dv(precisionFactor_, 0);
	dv[0] = this->udv_[0] * rhs.udv_[0];
	//ud.Set(dv);
	return ud;
}

UltraDouble UltraDouble::operator/(const UltraDouble& rhs)
{
	// Temporary routine using only the first element
	UltraDouble ud{ precisionFactor_ };
	std::vector<int8_t> dv(precisionFactor_, 0);
	dv[0] = this->udv_[0] / rhs.udv_[0];
	//ud.Set(dv);
	return ud;
}
