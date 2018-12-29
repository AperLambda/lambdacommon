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

bool lc_char_equals_ignore_case(char a, char b)
{
	return lambdacommon::lstring::equals_ignore_case(a, b);
}

bool lc_str_equals(const char *a, const char *b)
{
	return lambdacommon::lstring::equals({a}, {b});
}

bool lc_str_equals_ignore_case(const char *a, const char *b)
{
	return lambdacommon::lstring::equals_ignore_case({a}, {b});
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

bool lc_str_ends_with(const char *str, const char *suffix)
{
	return lambdacommon::lstring::ends_with({str}, {suffix});
}

bool lc_str_starts_with(const char *str, const char *prefix)
{
	return lambdacommon::lstring::starts_with({str}, {prefix});
}

int lc_str_parse_int(const char *integer)
{
	return lc_str_parse_int_base(integer, 10);
}

int lc_str_parse_int_base(const char *integer, int base)
{
	return lambdacommon::lstring::parse_int(integer, base).value_or(0);
}

long lc_str_parse_long(const char *longNumber)
{
	return lc_str_parse_long_base(longNumber, 10);
}

long lc_str_parse_long_base(const char *longNumber, int base)
{
	return lambdacommon::lstring::parse_long(longNumber, base).value_or(0);
}