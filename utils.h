#pragma once
#include "macro.h"
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

/*
	std::is_string이 없어서 직접 만들어 봄
*/
template <typename T>
struct is_string : std::false_type {};

template <>
struct is_string<std::string> : std::true_type {};

template <typename T>
constexpr bool is_string_v = is_string<T>::value;

/*
	해당 타입에 ToString()이라는 함수가 존재하는지 확인
 */
template<typename T, typename NotUsed = void>
struct has_ToString
	: std::false_type {};

template<typename T>
struct has_ToString<T, std::void_t<decltype(std::declval<T>().ToString())>>
	: std::true_type {};

template <typename T>
constexpr bool has_ToString_v = has_ToString<T>::value;

/*
	vector나 list같이 for문으로 순회 할 수 있는 타입인지 확인
	size(), begin(), end() 함수가 있으면 순회 가능한 것으로 판단
 */
template<typename T, typename NotUsed = void>
struct is_iterateAble 
	: std::false_type {};

template<typename T>
struct is_iterateAble<T, std::void_t<decltype(std::declval<T>().size()), decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> 
	: std::true_type {};

template <typename T>
constexpr bool is_iterateAble_v = is_iterateAble<T>::value;

/*
	T 타입 변수를 std::string으로 만들어 주는 싱글턴 클래스
*/
template <typename T>
class StringConverter
{
	THIS_CLASS_IS_SINGLETON(StringConverter)

public:
	std::string& ToString(std::string& str) { return str; }
	std::string ToString(const T& val);

private:
	std::string ToString_iterate(const T& val);
};

template<typename T>
std::string StringConverter<T>::ToString(const T& val)
{
	if constexpr (std::is_arithmetic_v<T>)
		return std::to_string(val);
	else if constexpr (has_ToString_v<T>)
		return val.ToString();
	else if constexpr (is_iterateAble_v<T>)
		return ToString_iterate(val);
	else
		return std::string("can not convert [") + typeid(T).name() + std::string("] type to string");
}

/*
	val 값을 순회 하면서 하나씩 변환
*/
template<typename T>
std::string StringConverter<T>::ToString_iterate(const T & val)
{
	std::string ret, delimiter;

	// 하나씩 변환하면서 콤마로 붙여 나간다
	for (const auto& v : val) {
		ret += delimiter;
		ret += ToString(v);

		delimiter = ", ";
	}
		
	return ret;
}
