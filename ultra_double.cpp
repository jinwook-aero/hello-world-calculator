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

// Constructor with UltraDouble
UltraDouble::UltraDouble(const UltraDouble & ud) :
	precisionFactor_{ ud.precisionFactor_ }, validityFlag_{ ud.validityFlag_ }, sign_{ ud.sign_ }, order_{ ud.order_ }
{
	// Initialize udv with all zeros
	udv_ = ud.udv_;
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
	if (str.at(0) == '+') // redundant sign
		str = str.substr(1, str.size() - 1);

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

void UltraDouble::Set(int sign, int order, std::vector<int8_t> udv)
{
	sign_  = sign;
	order_ = order;
	udv_   = udv;
}

std::string UltraDouble::Get()
{
	return ToString();
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

bool UltraDouble::operator==(const UltraDouble& rhs)
{
	if (((this->sign_) == (rhs.sign_)) &&
		((this->order_) == (rhs.order_))) {
		int maxPrecision = (this->precisionFactor_ > rhs.precisionFactor_) ? this->precisionFactor_ : rhs.precisionFactor_;
		int lv, rv;
		for (size_t i = 0; i != maxPrecision; ++i) {
			lv = (i < this->udv_.size()) ? this->udv_[i] : 0;
			rv = (i < rhs.udv_.size()  ) ? rhs.udv_[i]   : 0;
			if (lv != rv)
				break;
		}
		return true;
	}
	return false;
}

bool UltraDouble::operator<(const UltraDouble& rhs)
{
	// sign comparison --> equal positive sign will survive through
	if ((this->sign_) < (rhs.sign_))
		return true;
	else if ((this->sign_) > (rhs.sign_))
		return false;
	else // equal sign
		if (this->sign_ < 0) // Comparison between negative numbers
			return (FlipSign(rhs) < FlipSign(*this));

	// order comparison --> equal order_ will survive through
	if ((this->order_) < (rhs.order_))
		return true;
	else if ((this->order_) > (rhs.order_))
		return false;

	// digit comparison
	int maxPrecision = (this->precisionFactor_ > rhs.precisionFactor_) ? this->precisionFactor_ : rhs.precisionFactor_;
	int lv, rv;
	for (size_t i = 0; i != maxPrecision; ++i) {
		lv = (i < this->udv_.size()) ? this->udv_[i] : 0;
		rv = (i < rhs.udv_.size()) ? rhs.udv_[i] : 0;
		if (lv < rv)
			return true;
		else if (lv > rv)
			return false;
	}
	return false;
}

bool UltraDouble::operator>(const UltraDouble& rhs)
{
	return !((*this < rhs) || (*this == rhs));
}

bool UltraDouble::operator<=(const UltraDouble& rhs)
{
	return ((*this < rhs) ||  (*this == rhs));
}

bool UltraDouble::operator>=(const UltraDouble& rhs)
{
	return ((*this > rhs) || (*this == rhs));
}


UltraDouble UltraDouble::FlipSign(const UltraDouble& rhs)
{
	UltraDouble ud{ rhs };
	ud.sign_ *= -1;
	return ud;
}

UltraDouble UltraDouble::Absolute(const UltraDouble& rhs)
{
	UltraDouble ud{ rhs };
	ud.sign_ = 1;
	return ud;
}

UltraDouble UltraDouble::PushOrder(const UltraDouble& rhs, int deltaOrder) {
	UltraDouble ud{ rhs };
	ud.order_ += deltaOrder;
	return ud;
}

int UltraDouble::PushCarry(std::vector<int8_t>& iv)
{
	// iv[i]>=10 is pushed
	int8_t carry{ 0 };
	for (int i = (iv.size()-1); i >= 0; --i) {
		iv[i] += carry;
		if (iv[i] >= 10) {
			carry = iv[i] / 10;
			iv[i] -= 10 * carry;
		}
		else
			carry = 0;
	}
	if (carry) { // carry still left
		iv.insert(iv.begin(), carry);
		iv.pop_back();
		return 1; // returns order change +1
	}
	else
		return 0; // returns order change 0
}

int UltraDouble::PullCarry(std::vector<int8_t>& iv)
{
	// iv[i]<0 is pulled
	int8_t carry{ 0 };
	for (int i = (iv.size() - 1); i >= 0; --i) {
		iv[i] -= carry;
		if (iv[i] < 0) {
			carry = 1;
			iv[i] += 10 * carry;
		}
	}
	if (!iv[0]) { // If the first digit is zero, decrement the order by 1
		iv.erase(iv.begin());
		iv.push_back(0);
		return -1; // returns order change -1
	}
	else
		return 0; // returns order change 0
}

bool UltraDouble::IsZero(const UltraDouble& rhs) {
	for (auto elem : rhs.udv_)
		if (elem)
			return false;
	return true;
}

UltraDouble UltraDouble::operator+(const UltraDouble& rhs)
{
	// zero handling
	if (IsZero(rhs))
		return *this;
	if (IsZero(*this))
		return rhs;

	// sign operation
	int sign = 0;
	if ((this->sign_) * (rhs.sign_) < 0) { // different sign
		// a+b is a-(-b)
		if ((rhs.sign_) < 0)
			return (*this - FlipSign(rhs));

		// a+b is b-(-a)
		if ((this->sign_) < 0) {
			UltraDouble ud{ rhs };
			return (ud - FlipSign(*this));
		}
	}
	else // equal sign
	{
		if ((this->sign_) < 0)
			--sign;
		else
			++sign;
	}

	// order shifting such that *this is always larger
	if (*this < rhs) {
		UltraDouble ud{ rhs };
		return (ud + *this);
	}
	int rhsOrderShift = this->order_ - rhs.order_;

	// First + operation
	std::vector<int8_t> dv(precisionFactor_, 0);
	int8_t rv{ 0 }; size_t rvIter{ 0 };
	for (size_t i = 0; i != precisionFactor_; ++i) {
		rvIter = i - rhsOrderShift;
		if ((rvIter >= 0) && (rvIter < rhs.udv_.size()))
			rv = rhs.udv_[i - rhsOrderShift];
		else
			rv = 0;
		dv[i] = (this->udv_[i]) + rv;
	}

	// Carry handling
	int order = (this->order_) + PushCarry(dv);

	// Target UltraDouble to be filled
	UltraDouble ud{ precisionFactor_ };
	ud.Set(sign, order, dv);
	return ud;
}

UltraDouble UltraDouble::operator-(const UltraDouble& rhs)
{
	// zero handling
	if (IsZero(rhs))
		return *this;
	if (IsZero(*this))
		return FlipSign(rhs);

	// sign comparison
	if ((this->sign_) * (rhs.sign_) < 0) { // different sign
		// a-b is a+(-b)
		if ((rhs.sign_) < 0)
			return (*this + FlipSign(rhs));

		// a-b is -((-a)+b)
		if ((this->sign_) < 0) {
			UltraDouble ud{ rhs };
			return FlipSign((FlipSign(*this) + ud));
		}
	}
	else { // equal sign
		// a-b is (-b)-(-a)
		if ((this->sign_) < 0){
			UltraDouble ud{ rhs };
			return (FlipSign(ud) - (FlipSign(*this)));
		}
	}

	// order shifting such that *this is always larger
	if (*this < rhs) {
		UltraDouble ud{ rhs };
		return FlipSign(ud - *this);
	}
	int rhsOrderShift = this->order_ - rhs.order_;
	int sign = 1; // Result is always positive

	// First - operation
	std::vector<int8_t> dv(precisionFactor_, 0);
	int8_t rv{ 0 }; size_t rvIter{ 0 };
	for (size_t i = 0; i != precisionFactor_; ++i) {
		rvIter = i - rhsOrderShift;
		if ((rvIter >= 0) && (rvIter < rhs.udv_.size()))
			rv = rhs.udv_[i - rhsOrderShift];
		else
			rv = 0;
		dv[i] = (this->udv_[i]) - rv;
	}

	// Carry handling
	int order = (this->order_) + PullCarry(dv);

	// Target UltraDouble to be filled
	UltraDouble ud{ precisionFactor_ };
	ud.Set(sign, order, dv);
	return ud;
}

UltraDouble UltraDouble::operator*(const UltraDouble& rhs)
{
	// zero handling
	if (IsZero(rhs) || IsZero(*this)){
		UltraDouble ud{ precisionFactor_ };
		return ud;
	}

	// Multiply is series of addition
	UltraDouble SUM{ precisionFactor_ };
	for (size_t iR = 0; iR != rhs.precisionFactor_; ++iR) {
		int deltaOrder = rhs.order_ - iR;
		UltraDouble lud = Absolute(PushOrder(*this, deltaOrder));
		for (size_t iSum = 0; iSum != rhs.udv_[iR]; ++iSum){
			SUM = SUM + lud;
		}
	}

	// sign operation
	int sign = 0;
	if ((this->sign_) * (rhs.sign_) < 0) // different sign
		return FlipSign(SUM);
	else // equal sign
		return SUM;
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
