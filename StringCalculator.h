// stringCalculator.h
// Header file for StringCalculator class
//
// Author: Sungwook Lee
// First version: April 22, 2020
// Last update  : April 22, 2020
//

#pragma once
#include "utils.h"
#include <queue>
#include <stack>

template <typename T = double>
class StringCalculator
{
public:
	StringCalculator(const std::string& str = std::string()) : str_(str) {}
	void AppendStr(const std::string& str) { str_.append(str); }
	std::pair<bool, T> CalculateStr();

private:
	void SplitStrBySign();
	bool IsValidForm();
	void ConvertPostOrderForm();
	T GetAnswer();
	T Calculate(const T& lhs, const T& rhs, const char arithmeticSign);

	std::string	str_; // string to calculate: "3+4*7"
	std::list<std::string> splittedStr_; // "3", "+", "4", "*", "7"
};

// calculate string and return <isSuccess, answer>
template<typename T>
std::pair<bool, T> StringCalculator<T>::CalculateStr()
{
	SplitStrBySign();
	if (IsValidForm() == false)
		return std::make_pair(false, T());

	ConvertPostOrderForm();

	return std::make_pair(true, GetAnswer());
}

// split number and sign from string
template<typename T>
void StringCalculator<T>::SplitStrBySign()
{
	std::string number;
	splittedStr_.clear();

	for (const char c : str_)
		if (isdigit(c))
			number.push_back(c);
		else
		{
			if (number.empty() == false)
				splittedStr_.push_back(number);
			number.clear();

			if (IsArithmeticSign(c))
				splittedStr_.push_back(std::string(1, c));
		}

	if (number.empty() == false)
		splittedStr_.push_back(number);
}

// check validation 
template<typename T>
bool StringCalculator<T>::IsValidForm()
{
	unsigned int signCnt = 0;
	unsigned int numberCnt = 0;
	unsigned int openBracketCnt = 0;
	unsigned int closeBracketCnt = 0;

	// count elements
	for (const auto& str : splittedStr_)
	{
		char c = str[0];

		if (c == '(')
			openBracketCnt++;
		else if (c == ')')
			closeBracketCnt++;
		else if (isdigit(c))
			numberCnt++;
		else if (IsArithmeticSign(c))
			signCnt++;
		else
			return false;
	}

	// check bracket pairing
	if (openBracketCnt != closeBracketCnt)
		return false;

	// check sign cnt
	if (numberCnt != signCnt + 1)
		return false;
	
	return true;
}

// [3 + 4 * 7 - 4] => [3 4 7 * + 4 -]
template<typename T>
void StringCalculator<T>::ConvertPostOrderForm()
{
	// todo
	splittedStr_;

	return;
}

template<typename T>
T StringCalculator<T>::GetAnswer()
{
	std::stack<T> numbers;

	for (const auto& str : splittedStr_)
		if (IsArithmeticSign(str.c_str()) == false)
		{
			T number = FromStringConverter<T>::GetInstance()->FromString(str);
			numbers.push(std::move(number));
		}
		else
		{
			const char arithmeticSign = *(str.c_str());
			const T& rhs = std::move(numbers.top());
			numbers.pop();
			const T& lhs = std::move(numbers.top());
			numbers.pop();

			T number = Calculate(lhs, rhs, arithmeticSign);
			numbers.push(std::move(number));
		}

	return numbers.top();
}

template<typename T>
T StringCalculator<T>::Calculate(const T & lhs, const T & rhs, const char arithmeticSign)
{
	switch (arithmeticSign)
	{
	case '+':
		return lhs + rhs;
	case '-':
		return lhs - rhs;
	case '*':
		return lhs * rhs;
	case '/':
		return lhs / rhs;
	default:
		exit(-1);
	}
}
