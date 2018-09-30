/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_STRING_H
#define LAMBDACOMMON_STRING_H

#include <vector>
#include "lambdacommon.h"

namespace lambdacommon
{
	namespace lstring
	{
		template<typename Out>
		extern void LAMBDACOMMON_API split(const std::string &s, char delim, Out result);

		/*!
		 * Splits a string into a vector with a delimiter.
		 * @param s String to split.
		 * @param delim Delimiter.
		 * @return The split string as a vector.
		 */
		extern std::vector<std::string> LAMBDACOMMON_API split(const std::string &s, char delim);

		/*!
		 * Checks if two chars are equal with case insensitive.
		 * @param a One of two chars.
		 * @param b One of two chars.
		 * @return True if they are equal with case insensitive else false.
		 */
		extern bool LAMBDACOMMON_API equalsIgnoreCase(char a, char b);

		/*!
		 * Checks if two strings are equal.
		 * @param a One of two strings.
		 * @param b One of two strings.
		 * @return True if they are equal else false.
		 */
		extern bool LAMBDACOMMON_API equals(const std::string &a, const std::string &b);

		/*!
		 * Checks if two strings are equal with case insensitive.
		 * @param a One of two strings.
		 * @param b One of two strings.
		 * @return True if they are equal with case insensitive else false.
		 */
		extern bool LAMBDACOMMON_API equalsIgnoreCase(const std::string &a, const std::string &b);

		/*!
		 * Transforms a string to a full lower case string.
		 * @param from The string to transform.
		 * @return The new string.
		 */
		extern std::string LAMBDACOMMON_API toLowerCase(const std::string &from);

		/*!
		 * Transforms a string to a full lower case string.
		 * @param from The string to transform.
		 * @return The new string.
		 */
		extern std::string LAMBDACOMMON_API toUpperCase(const std::string &from);

		extern std::string LAMBDACOMMON_API replaceAll(std::string subject, const char &from, const char &to);

		extern std::string LAMBDACOMMON_API
		replaceAll(std::string subject, const std::string &from, const std::string &to);

		/*!
		 * Transforms a boolean value into a string value.
		 * @param value A boolean value.
		 * @return The boolean value as a string.
		 */
		extern std::string LAMBDACOMMON_API to_string(bool value);

		/*!
		 * Transforms a pointer address into a string value.
		 * @param pointer The pointer address.
		 * @return The pointer address as a string.
		 */
		extern std::string LAMBDACOMMON_API to_string(const void *pointer);

		/*!
		 * Transforms a string vector into a string value.
		 * @param vec The vector.
		 * @return The vector value as a string.
		 */
		extern std::string LAMBDACOMMON_API to_string(const std::vector<std::string> &vec);

		/*!
		 * Checks whether the specified string does end with the specified suffix.
		 * @param str The string to check.
		 * @param suffix The suffix.
		 * @return True if the string ended with the specified suffix, else false.
		 */
		extern bool LAMBDACOMMON_API endsWith(const std::string &str, const std::string &suffix);

		/*!
		 * Checks whether the specified string does start with the specified prefix.
		 * @param str The string to check.
		 * @param prefix The prefix.
		 * @return True if the string started with the specified prefix, else false.
		 */
		extern bool LAMBDACOMMON_API startsWith(const std::string &str, const std::string &prefix);

		extern const std::string LAMBDACOMMON_API mergePath(std::string parent, const std::string &child);

		/*!
		 * Parses int from a string.
		 * @param integer The integer to parse.
		 * @param base The base of the integer.
		 * @return The parsed integer, may be 0 if parse failed.
		 */
		extern int LAMBDACOMMON_API parseInt(const std::string &integer, int base = 10);

		/*!
		 * Parses long from a string.
		 * @param longNumber The long to parse.
		 * @param base The base of the long.
		 * @return The parsed long, may be 0 if parse failed.
		 */
		extern long LAMBDACOMMON_API parseLong(const std::string &longNumber, int base = 10);

		/*
		 * String conversions
		 */

		namespace utf8
		{
			/*!
			 * Converts an UTF-8 character to an UTF-32 character.
			 * @param character Value representing an UTF-8 character.
			 * @return The UTF-32 character.
			 */
			extern char32_t LAMBDACOMMON_API toUTF32(const std::string &character);

			/*!
			 * Converts an UTF-8 character to an UTF-32 character.
			 * @param character Value representing an UTF-8 character.
			 * @return The UTF-32 character.
			 */
			extern char32_t LAMBDACOMMON_API toUTF32(const char *character);
		}

		/**
		 * Converts a std::wstring to a std::string.
		 * @param wstring The std::wstring to convert.
		 * @return The converted string.
		 */
		extern std::string LAMBDACOMMON_API convertWStringToString(std::wstring wstring);

		/**
		 * Converts a std::string to a std::wstring.
		 * @param wstring The std::string to convert.
		 * @return The converted wstring.
		 */
		extern std::wstring LAMBDACOMMON_API convertStringToWString(std::string string);
	}
}

#endif //LAMBDACOMMON_STRING_H