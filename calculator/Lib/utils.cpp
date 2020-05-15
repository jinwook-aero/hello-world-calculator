#include "pch.h"
#include "utils.h"

bool IsArithmeticSign(const char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

bool IsArithmeticSign(const char *c)
{
	return IsArithmeticSign(*c);
}
