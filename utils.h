#pragma once
#include "macro.h"
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

/*
	std::is_string�� ��� ���� ����� ��
*/
template <typename T>
struct is_string : std::false_type {};

template <>
struct is_string<std::string> : std::true_type {};

template <typename T>
constexpr bool is_string_v = is_string<T>::value;

/*
	�ش� Ÿ�Կ� ToString()�̶�� �Լ��� �����ϴ��� Ȯ��
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
	vector�� list���� for������ ��ȸ �� �� �ִ� Ÿ������ Ȯ��
	size(), begin(), end() �Լ��� ������ ��ȸ ������ ������ �Ǵ�
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
	T Ÿ�� ������ std::string���� ����� �ִ� �̱��� Ŭ����
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
	val ���� ��ȸ �ϸ鼭 �ϳ��� ��ȯ
*/
template<typename T>
std::string StringConverter<T>::ToString_iterate(const T & val)
{
	std::string ret, delimiter;

	// �ϳ��� ��ȯ�ϸ鼭 �޸��� �ٿ� ������
	for (const auto& v : val) {
		ret += delimiter;
		ret += ToString(v);

		delimiter = ", ";
	}
		
	return ret;
}
