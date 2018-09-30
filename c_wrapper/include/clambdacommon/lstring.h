/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_LSTRING_H
#define CLAMBDACOMMON_LSTRING_H

#include "clambdacommon.h"
#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * Splits a string into a vector with a delimiter.
 * @param s String to split.
 * @param delim Delimiter.
 * @return The split string count.
 */
//uint64_t lc_str_split(const char *s, char delim, char **output);

/*!
 * Checks if two chars are equal with case insensitive.
 * @param a One of two chars.
 * @param b One of two chars.
 * @return True if they are equal with case insensitive else false.
 */
bool lc_char_equalsIgnoreCase(char a, char b);

/*!
 * Checks if two strings are equal.
 * @param a One of two strings.
 * @param b One of two strings.
 * @return True if they are equal else false.
 */
bool lc_str_equals(const char *a, const char *b);

/*!
 * Checks if two strings are equal with case insensitive.
 * @param a One of two strings.
 * @param b One of two strings.
 * @return True if they are equal with case insensitive else false.
 */
bool lc_str_equalsIgnoreCase(const char *a, const char *b);

/*!
 * Transforms a string to a full lower case string.
 * @param from The string to transform.
 * @return The new string.
 */
//std::string toLowerCase(const char *from);

/*!
 * Transforms a string to a full lower case string.
 * @param from The string to transform.
 * @return The new string.
 */
//std::string toUpperCase(const char *from);

//std::string replaceAll(std::string subject, const char &from, const char &to);

/*std::string
replaceAll(std::string
subject,
const char *from,
const char *to
);*/

/*!
 * Transforms a boolean value into a string value.
 * @param value A boolean value.
 * @return The boolean value as a string.
 */
const char *lc_bool_to_string(bool value);

/*!
 * Transforms a pointer address into a string value.
 * @param pointer The pointer address.
 * @return The pointer address as a string.
 */
const char *lc_pointer_to_string(const void *pointer);

bool lc_str_endsWith(const char *str, const char *suffix);

bool lc_str_startsWith(const char *str, const char *prefix);

int lc_str_parseInt(const char *integer);

int lc_str_parseIntBase(const char *integer, int base);

long lc_str_parseLong(const char *longNumber);

long lc_str_parseLongBase(const char *longNumber, int base);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_LSTRING_H