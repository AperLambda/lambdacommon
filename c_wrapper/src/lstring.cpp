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

size_t lcommon_str_split(const char *s, char delim, char **output)
{
	std::vector<std::string> result = lambdacommon::lambdastring::split(s, delim);
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

bool lcommon_char_equalsIgnoreCase(char a, char b)
{
	return lambdacommon::lambdastring::equalsIgnoreCase(a, b);
}

bool lcommon_str_equals(const char *a, const char *b)
{
	return lambdacommon::lambdastring::equals({a}, {b});
}

bool lcommon_str_equalsIgnoreCase(const char *a, const char *b)
{
	return lambdacommon::lambdastring::equalsIgnoreCase({a}, {b});
}

const char *lcommon_bool_to_string(bool value)
{
	return value ? "true" : "false";
}

const char *lcommon_pointer_to_string(const void *pointer)
{
	auto output = lambdacommon::lambdastring::to_string(pointer);
	char *result = new char[output.length()];
	output.copy(result, output.length());
	return result;
}

bool lcommon_str_endsWith(const char *str, const char *suffix)
{
	return lambdacommon::lambdastring::endsWith({str}, {suffix});
}

bool lcommon_str_startsWith(const char *str, const char *prefix)
{
	return lambdacommon::lambdastring::startsWith({str}, {prefix});
}

int lcommon_str_parseInt(const char *integer)
{
	return lcommon_str_parseIntBase(integer, 10);
}

int lcommon_str_parseIntBase(const char *integer, int base)
{
	return lambdacommon::lambdastring::parseInt(integer, base);
}

long lcommon_str_parseLong(const char *longNumber)
{
	return lcommon_str_parseLongBase(longNumber, 10);
}

long lcommon_str_parseLongBase(const char *longNumber, int base)
{
	return lambdacommon::lambdastring::parseLong(longNumber, base);
}