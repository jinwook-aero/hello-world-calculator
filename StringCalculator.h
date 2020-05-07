// stringCalculator.h
// Header file for StringCalculator class
//
// Author: Sungwook Lee
// Update: Jinwook Lee
// First version: April 22, 2020
// Last update  : May 5, 2020
//

#pragma once
#include "utils.h"
#include "Calculator.h"
#include <queue>
#include <stack>

template <typename T = double>
class StringCalculator
{
public:
	StringCalculator(int pFactor, const std::string& str = std::string()) : precisionFactor_(pFactor), str_(str) {}
	void AppendStr(const std::string& str) { str_.append(str); }
	std::pair<bool, T> CalculateStr();

private:
	void ParseStr();
	void DetermineBracketGroup();
	bool IsValidForm();
	T ComputeTopBracketGroup(); // Recursive function that unrolls from the highest bracket group
	T Calculate(T& lhs, T& rhs, INPUT_CMD_TYPE);

	std::string	str_; // string to calculate: "1 + ( 2 + ( 4 + 5 ) ) * ( 6 + 7 )"
	std::vector<std::string> cmdStrVec_; // "1", "+", "(", "2", "+", "(", "4", ...
	std::vector<INPUT_CMD_TYPE> cmdTypeVec_;

	// Bracket group is first determined to determine precedence
	// Example command: 1 + ( 2 + ( 4 + 5 ) ) * ( 6 + 7 )
	// Bracket group  : 0 0 1 1 1 2 2 2 2 2 1 0 3 3 3 3 3
	std::vector<int> bracketGroupVec_;
	int topBracketGroup_;
	int precisionFactor_;
};

// calculate string and return <isValid, answer>
template<typename T>
std::pair<bool, T> StringCalculator<T>::CalculateStr()
{
	ParseStr();

	bool isValid = IsValidForm();
	T asnwer = isValid ? ComputeTopBracketGroup() : T{};

	return std::make_pair(isValid, asnwer);
}

// split number and sign from string
template<typename T>
void StringCalculator<T>::ParseStr()
{
	std::string numberStr;
	cmdStrVec_.clear();
	cmdTypeVec_.clear();

	for (const char c : str_)
		if (isdigit(c) || c == '.')
			numberStr.push_back(c);
		else
		{
			if (numberStr.size() > 0) 
			{
				cmdStrVec_.push_back(numberStr);
				cmdTypeVec_.push_back(INPUT_CMD_TYPE::NUMBER);

				numberStr.clear();
			}

			if (IsArithmeticSign(c)) 
			{
				cmdStrVec_.push_back(std::string(1, c));

				switch (c) 
				{
				case '(':
					cmdTypeVec_.push_back(INPUT_CMD_TYPE::OPEN_BRACKET);
					break;
				case ')':
					cmdTypeVec_.push_back(INPUT_CMD_TYPE::CLOSE_BRACKET);
					break;
				case '*':
					cmdTypeVec_.push_back(INPUT_CMD_TYPE::MULTIPLY);
					break;
				case '/':
					cmdTypeVec_.push_back(INPUT_CMD_TYPE::DIVIDE);
					break;
				case '+':
					cmdTypeVec_.push_back(INPUT_CMD_TYPE::PLUS);
					break;
				case '-':
					cmdTypeVec_.push_back(INPUT_CMD_TYPE::MINUS);
					break;
				default:
					exit(-1);
				}
			}
		}

	if (numberStr.size() > 0)
	{
		cmdStrVec_.push_back(numberStr);
		cmdTypeVec_.push_back(INPUT_CMD_TYPE::NUMBER);
	}
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
	for (const auto& cmdType : cmdTypeVec_)
	{
		if (cmdType == INPUT_CMD_TYPE::OPEN_BRACKET)
			openBracketCnt++;
		else if (cmdType == INPUT_CMD_TYPE::CLOSE_BRACKET)
			closeBracketCnt++;
		else if (cmdType == INPUT_CMD_TYPE::NUMBER)
			numberCnt++;
		else if (cmdType == INPUT_CMD_TYPE::PLUS     ||
				 cmdType == INPUT_CMD_TYPE::MINUS    ||
				 cmdType == INPUT_CMD_TYPE::MULTIPLY ||
				 cmdType == INPUT_CMD_TYPE::DIVIDE)
			signCnt++;
		else
			return false;
	}

	// Bracket pairing
	if (openBracketCnt != closeBracketCnt)
		return false;

	// Sign cnt
	if (numberCnt != signCnt + 1)
		return false;
	
	return true;
}

// split number and sign from string
template<typename T>
void StringCalculator<T>::DetermineBracketGroup()
{
	// Bracket group is first determined before allocating precedence number
	// Example command: 1 + ( 2 + ( 4 + 5 ) ) * ( 6 + 7 )
	// Bracket group  : 0 0 1 1 1 2 2 2 2 2 1 0 3 3 3 3 3
	uint32_t currentBracketGroup = 0;
	bracketGroupVec_.clear(); 
	bracketGroupVec_.resize(cmdTypeVec_.size(), 0);

	for (size_t vecIndex = 0; vecIndex != cmdTypeVec_.size(); ++vecIndex)
		if (cmdTypeVec_.at(vecIndex) == INPUT_CMD_TYPE::OPEN_BRACKET)
		{
			++currentBracketGroup;
			uint32_t bracket2close = 1;

			// in the bracket
			for (vecIndex2 = vecIndex + 1; bracket2close != 0; ++vecIndex2)
			{
				bracketGroupVec_.at(vecIndex2) = currentBracketGroup;

				if (cmdTypeVec_.at(vecIndex2) == INPUT_CMD_TYPE::OPEN_BRACKET)
					++bracket2close;
				else if (cmdTypeVec_.at(vecIndex2) == INPUT_CMD_TYPE::CLOSE_BRACKET)
					--bracket2close;
			}
		}

	if (bracketGroupVec_.size() > 0)
		topBracketGroup_ = *std::max_element(bracketGroupVec_.begin(), bracketGroupVec_.end());
	else
		topBracketGroup_ = 0;

	// Bracket group debug
	for (const auto& cmdStr : cmdStrVec_)
		std::cout << cmdStr << " ";
	std::cout << std::endl;

	for (const auto& bracketGroup : bracketGroupVec_)
		std::cout << bracketGroup << " ";
	std::cout << std::endl;
}

template<typename T>
T StringCalculator<T>::ComputeTopBracketGroup()
{
	// For each layer of call, bracket group is re-determined
	DetermineBracketGroup();

	// Local string vector at top bracket group
	std::vector<std::string> localStrVec;
	std::vector<INPUT_CMD_TYPE> localCmdTypeVec;
	for (int i = 0; i < cmdStrVec_.size(); ++i)
		if (bracketGroupVec_[i] == topBracketGroup_) 
		{
			localStrVec.push_back(cmdStrVec_[i]);
			localCmdTypeVec.push_back(cmdTypeVec_[i]);
		}

	// Compute along precedence
	// Example str: 1 + 2 + 3 * 4 * 5 - 6 / 7
	// Precedence :   4   5   1   2   6   3 
	// After compuation, localStrVec = {"RESULT","\0","\0",...,"\0"}
	for (INPUT_CMD_TYPE cmdType: {INPUT_CMD_TYPE::MULTIPLY, INPUT_CMD_TYPE::DIVIDE, INPUT_CMD_TYPE::PLUS, INPUT_CMD_TYPE::MINUS})
		for (int i = 0; i < localStrVec.size(); ++i)
			if (localCmdTypeVec[i] == cmdType) 
			{
				int iL, iR;
				T lhs, rhs;

				// First identify the lhs and rhs
				for (iL = i; iL >= 0; --iL)
					if (localCmdTypeVec[iL] == INPUT_CMD_TYPE::NUMBER)
						lhs = FromStringConverter<T>::GetInstance()->FromString(precisionFactor_, localStrVec[iL]);
						break;

				for (iR = i; iR != localStrVec.size(); ++iR)
					if (localCmdTypeVec[iR] == INPUT_CMD_TYPE::NUMBER)
						rhs = FromStringConverter<T>::GetInstance()->FromString(precisionFactor_, localStrVec[iR]);
						break;

				// Update the localCmdTypeVec_ and localStrVec_
				for (int ii = iL+1; ii <=iR; ++ii) 
				{
					localStrVec[ii] = std::string(1, 0);
					localCmdTypeVec[ii] = INPUT_CMD_TYPE::COMPUTED;
				}

				localStrVec[iL] = ToStringConverter<T>::GetInstance()->ToString(Calculate(lhs, rhs, cmdType));
			}

	// Remove the first OPEN_BRACKET if exists
	if (localCmdTypeVec[0] == INPUT_CMD_TYPE::OPEN_BRACKET) 
	{
		// Shift result to first cell overriding bracket
		localStrVec[0]     = localStrVec[1];
		localCmdTypeVec[0] = INPUT_CMD_TYPE::NUMBER;

		// Clear the other cells
		for (int i = 1; i < localStrVec.size(); ++i) 
		{
			localStrVec[i]     = std::string(1, 0);
			localCmdTypeVec[i] = INPUT_CMD_TYPE::COMPUTED;
		}
	}

	// Update the top bracket group with result
	// First cell is filled with computed number
	// Other cells reallocated with COMPUTED TYPE
	bool isFirstCell = true;
	for (int i = 0; i != cmdStrVec_.size(); ++i)
		if (bracketGroupVec_[i] == topBracketGroup_)
		{
			bracketGroupVec_[i] -= 1; // Decremented by one

			if (isFirstCell)
			{
				cmdStrVec_[i] = localStrVec.at(0);
				cmdTypeVec_[i] = INPUT_CMD_TYPE::NUMBER;
				isFirstCell = false;
			}
			else
			{
				cmdStrVec_[i] = std::string(1, 0);
				cmdTypeVec_[i] = INPUT_CMD_TYPE::COMPUTED;
			}
		}

	topBracketGroup_ -= 1; // Top bracket group count decremented by one

	// Recursive call
	if (topBracketGroup_ < 0)
		return FromStringConverter<T>::GetInstance()->FromString(precisionFactor_, cmdStrVec_[0]);
	return ComputeTopBracketGroup();
}

template<typename T>
T StringCalculator<T>::Calculate(T& lhs, T& rhs, INPUT_CMD_TYPE cmdType)
{
	switch (cmdType) 
	{
	case INPUT_CMD_TYPE::PLUS:
		return lhs + rhs;
	case INPUT_CMD_TYPE::MINUS:
		return lhs - rhs;
	case INPUT_CMD_TYPE::MULTIPLY:
		return lhs * rhs;
	case INPUT_CMD_TYPE::DIVIDE:
		return lhs / rhs;
	default:
		exit(-1);
	}
}