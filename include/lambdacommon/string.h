/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
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
	namespace lambdastring
	{
		template<typename Out>
		extern void LAMBDACOMMON_API split(const std::string &s, char delim, Out result);

		/**
		 * Splits a string into a vector with a delimiter.
		 * @param s String to split.
		 * @param delim Delimiter.
		 * @return The split string as a vector.
		 */
		extern std::vector<std::string> LAMBDACOMMON_API split(const std::string &s, char delim);

		/**
		 * Checks if two chars are equal with case insensitive.
		 * @param a One of two chars.
		 * @param b One of two chars.
		 * @return True if they are equal with case insensitive else false.
		 */
		extern bool LAMBDACOMMON_API equalsIgnoreCase(const char a, const char b);

		/**
		 * Checks if two strings are equal.
		 * @param a One of two strings.
		 * @param b One of two strings.
		 * @return True if they are equal else false.
		 */
		extern bool LAMBDACOMMON_API equals(std::string const &a, std::string const &b);

		/**
		 * Checks if two strings are equal with case insensitive.
		 * @param a One of two strings.
		 * @param b One of two strings.
		 * @return True if they are equal with case insensitive else false.
		 */
		extern bool LAMBDACOMMON_API equalsIgnoreCase(std::string const &a, std::string const &b);

		extern std::string LAMBDACOMMON_API replaceAll(std::string subject, const char &from, const char &to);

		extern std::string LAMBDACOMMON_API
		replaceAll(std::string subject, const std::string &from, const std::string &to);

		extern std::string LAMBDACOMMON_API to_string(bool value);

#ifdef LAMBDA_WINDOWS

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

#endif
	}
}

#endif //LAMBDACOMMON_STRING_H