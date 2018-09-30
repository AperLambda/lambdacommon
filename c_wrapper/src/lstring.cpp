/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/clambdacommon/lstring.h"
#include <lambdacommon/lstring.h>

size_t lc_str_split(const char *s, char delim, char **output)
{
	std::vector<std::string> result = lambdacommon::lstring::split(s, delim);
	output = new char*[result.size()];
	for (size_t i = 0; i < result.size(); i++)
	{
		auto length = result[i].length();
		char *a = new char[length];
		result[i].copy(a, length);
		output[i] = a;
	}
	return result.size();
}

bool lc_char_equalsIgnoreCase(char a, char b)
{
	return lambdacommon::lstring::equalsIgnoreCase(a, b);
}

bool lc_str_equals(const char *a, const char *b)
{
	return lambdacommon::lstring::equals({a}, {b});
}

bool lc_str_equalsIgnoreCase(const char *a, const char *b)
{
	return lambdacommon::lstring::equalsIgnoreCase({a}, {b});
}

const char *lc_bool_to_string(bool value)
{
	return value ? "true" : "false";
}

const char *lc_pointer_to_string(const void *pointer)
{
	auto output = lambdacommon::lstring::to_string(pointer);
	char *result = new char[output.length()];
	output.copy(result, output.length());
	return result;
}

bool lc_str_endsWith(const char *str, const char *suffix)
{
	return lambdacommon::lstring::endsWith({str}, {suffix});
}

bool lc_str_startsWith(const char *str, const char *prefix)
{
	return lambdacommon::lstring::startsWith({str}, {prefix});
}

int lc_str_parseInt(const char *integer)
{
	return lc_str_parseIntBase(integer, 10);
}

int lc_str_parseIntBase(const char *integer, int base)
{
	return lambdacommon::lstring::parseInt(integer, base);
}

long lc_str_parseLong(const char *longNumber)
{
	return lc_str_parseLongBase(longNumber, 10);
}

long lc_str_parseLongBase(const char *longNumber, int base)
{
	return lambdacommon::lstring::parseLong(longNumber, base);
}