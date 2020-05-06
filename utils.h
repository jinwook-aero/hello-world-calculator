// stringCalculator.h
// Header file for StringCalculator class
//
// Author: Sungwook Lee
// First version: April 22, 2020
// Last update  : April 22, 2020
//

#pragma once
#include "macro.h"
#include "ultra_double.h"
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

bool IsArithmeticSign(const char  c);
bool IsArithmeticSign(const char *c);

/*
	std::is_string does not seem to exist
	so we made a handcrafted one
*/
template <typename T>
struct is_string : std::false_type {};

template <>
struct is_string<std::string> : std::true_type {};

template <typename T>
constexpr bool is_string_v = is_string<T>::value;

/*
	Check if ToString() exist in the target class
 */
template <typename T, typename NotUsed = void>
struct has_ToString
	: std::false_type {};

template <typename T>
struct has_ToString<T, std::void_t<decltype(std::declval<T>().ToString())>>
	: std::true_type {};

template <typename T>
constexpr bool has_ToString_v = has_ToString<T>::value;

/*
	Check if the class is iterable with for loop as in vector or list
	with existence of begin() and end()
	// size() is not necessary - Jinwook May 5, 2020
 */
template <typename T, typename NotUsed = void>
struct is_iterable
	: std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
	: std::true_type {};

template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

/*
	T type is converted to std::string
*/
template <typename T>
class ToStringConverter
{
	THIS_CLASS_IS_SINGLETON(ToStringConverter)

public:
	// std::string& ToString(std::string& str) { return str; }
	std::string ToString(const T& val);

private:
	std::string ToString_iterate(const T& val);
};

template <typename T>
std::string ToStringConverter<T>::ToString(const T& val)
{
	if constexpr (is_string_v<T>)
		return val;
	else if constexpr (std::is_arithmetic_v<T>)
		return std::to_string(val);
	else if constexpr (has_ToString_v<T>)
		return val.ToString();
	else if constexpr (is_iterable_v<T>)
		return ToString_iterate(val);
	else
		return std::string("can not convert [") + typeid(T).name() + std::string("] type to string");
}

/*
	Iterate through val
*/
template <typename T>
std::string ToStringConverter<T>::ToString_iterate(const T & val)
{
	std::string ret, delimiter;

	// Concatenated with ,
	for (const auto& v : val) {
		ret += delimiter;
		ret += ToString(v);

		delimiter = ", ";
	}

	return ret;
}

/*
	std::string back to T
*/
template <typename T>
class FromStringConverter
{
	THIS_CLASS_IS_SINGLETON(FromStringConverter)

public:
	T FromString(const std::string& str) { return static_cast<T>(str); }
};

template <>
class FromStringConverter<UltraDouble>
{
	THIS_CLASS_IS_SINGLETON(FromStringConverter)

public:
	UltraDouble FromString(int precision, const std::string& str)
	{
		UltraDouble ud{ precision };
		std::vector<double> dv(precision,0);
		dv[0] = atof(str.c_str());
		ud.Set(dv);
		return ud;
	}
};

template <>
class FromStringConverter<int>
{
	THIS_CLASS_IS_SINGLETON(FromStringConverter)

public:
	int FromString(const std::string& str) { return atoi(str.c_str()); }
};

template <>
class FromStringConverter<double>
{
	THIS_CLASS_IS_SINGLETON(FromStringConverter)

public:
	double FromString(const std::string& str) { return atof(str.c_str()); }
};

template <>
class FromStringConverter<long long>
{
	THIS_CLASS_IS_SINGLETON(FromStringConverter)

public:
	long long FromString(const std::string& str) { return atoll(str.c_str()); }
};
