/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/lstring.h"
#include <sstream>
#include <iterator>
#include <utility>
#include <algorithm>
#include <cuchar>
#include <codecvt>
#include <locale>

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4101)
#endif

namespace lambdacommon
{
	namespace lambdastring
	{
		template<typename Out>
		void LAMBDACOMMON_API split(const std::string &s, char delim, Out result)
		{
			std::stringstream ss;
			ss.str(s);
			std::string item;
			while (getline(ss, item, delim))
			{
				*(result++) = item;
			}
		}

		std::vector<std::string> LAMBDACOMMON_API split(const std::string &s, char delimiter)
		{
			std::vector<std::string> elems;
			split(s, delimiter, back_inserter(elems));
			return elems;
		}

		bool LAMBDACOMMON_API equalsIgnoreCase(const char a, const char b)
		{
			return tolower(a) == tolower(b);
		}

		bool LAMBDACOMMON_API equals(std::string const &a, std::string const &b)
		{
			if (a.length() == b.length())
				return a == b;
			else
				return false;
		}

		bool LAMBDACOMMON_API equalsIgnoreCase(std::string const &a, std::string const &b)
		{
			if (a.length() == b.length())
			{
				return equal(begin(a), end(a), begin(b),
							 [](const char charA, const char charB) { return equalsIgnoreCase(charA, charB); });
			}
			else
				return false;
		}

		std::string LAMBDACOMMON_API toLowerCase(const std::string &from)
		{
			std::string result = from;
			std::transform(result.begin(), result.end(), result.begin(), ::tolower);
			return result;
		}

		std::string LAMBDACOMMON_API toUpperCase(const std::string &from)
		{
			std::string result = from;
			std::transform(result.begin(), result.end(), result.begin(), ::toupper);
			return result;
		}

		std::string LAMBDACOMMON_API replaceAll(std::string subject, const char &from, const char &to)
		{
			return replaceAll(std::move(subject), std::to_string(from), std::to_string(to));
		}

		std::string LAMBDACOMMON_API replaceAll(std::string subject, const std::string &from, const std::string &to)
		{
			size_t start_pos = 0;
			while ((start_pos = subject.find(from, start_pos)) != std::string::npos)
			{
				subject.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
			return subject;
		}

		std::string LAMBDACOMMON_API to_string(bool value)
		{
			if (value)
				return "true";
			else
				return "false";
		}

		std::string LAMBDACOMMON_API to_string(const void *pointer)
		{
			std::ostringstream ss;
			ss << std::hex << pointer;
			auto result = ss.str();
			if (!startsWith(result, "0x"))
				result = "0x" + result;
			return result;
		}

		std::string LAMBDACOMMON_API to_string(const std::vector<std::string> &vec)
		{
			std::string result{"{"};
			for (const auto &str : vec)
				result += (str + ", ");
			result = result.substr(0, result.size() - 2) + '}';
			return result;
		}

		bool LAMBDACOMMON_API endsWith(const std::string &str, const std::string &suffix)
		{
			return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
		}

		bool LAMBDACOMMON_API startsWith(const std::string &str, const std::string &prefix)
		{
			return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
		}

		const std::string LAMBDACOMMON_API mergePath(std::string parent, const std::string &child)
		{
			auto merged = parent;
			if (endsWith(parent, "/") || startsWith(child, "/"))
				merged += child;
			else
				merged += '/' + child;
			return merged;
		}

		int LAMBDACOMMON_API parseInt(const std::string &integer, int base)
		{
			try
			{
				return std::stoi(integer, nullptr, base);
			}
			catch (const std::invalid_argument &error)
			{
				return 0;
			}
			catch (const std::out_of_range &error)
			{
				return 0;
			}
		}

		long LAMBDACOMMON_API parseLong(const std::string &longNumber, int base)
		{
			try
			{
				return std::stol(longNumber, nullptr, base);
			}
			catch (const std::invalid_argument &error)
			{
				return 0;
			}
			catch (const std::out_of_range &error)
			{
				return 0;
			}
		}

		namespace utf8
		{
			char32_t LAMBDACOMMON_API toUTF32(const std::string &character)
			{
				return toUTF32(character.c_str());
			}

			char32_t LAMBDACOMMON_API toUTF32(const char *character)
			{
				auto result = static_cast<char32_t>(-1);

				if (!character)
					return result;

				if ((character[0] & 0x80) == 0x0)
					result = (char32_t) character[0];

				if ((character[0] & 0xC0) == 0xC0)
					result = static_cast<char32_t>(((character[0] & 0x3F) << 6) | (character[1] & 0x3F));

				if ((character[0] & 0xE0) == 0xE0)
					result = static_cast<char32_t>(((character[0] & 0x1F) << (6 + 6)) | ((character[1] & 0x3F) << 6) | (character[2] & 0x3F));

				if ((character[0] & 0xF0) == 0xF0)
					result = static_cast<char32_t>(((character[0] & 0x0F) << (6 + 6 + 6)) | ((character[1] & 0x3F) << (6 + 6)) |
												   ((character[2] & 0x3F) << 6) | (character[3] & 0x3F));

				if ((character[0] & 0xF8) == 0xF8)
					result = static_cast<char32_t>(((character[0] & 0x07) << (6 + 6 + 6 + 6)) | ((character[1] & 0x3F) << (6 + 6 + 6)) |
												   ((character[2] & 0x3F) << (6 + 6)) | ((character[3] & 0x3F) << 6) | (character[4] & 0x3F));

				return result;
			}
		}

#ifdef LAMBDA_WINDOWS
#ifndef __GNUC__
#define CONVERT_WSTRING_WINDOWS_WAY
#endif
#endif

#ifdef CONVERT_WSTRING_WINDOWS_WAY

#include <Windows.h>

		std::string LAMBDACOMMON_API convertWStringToString(std::wstring wstring)
		{
			std::string string;
			if (!wstring.empty())
			{
				int size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int) wstring.size(), nullptr, 0, nullptr,
											   nullptr);
				string.resize(size, 0);
				WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int) wstring.size(), &string[0], size, nullptr, nullptr);
			}
			return string;
		}

		std::wstring LAMBDACOMMON_API convertStringToWString(std::string string)
		{
			int size = MultiByteToWideChar(CP_UTF8, 0, &string[0], (int) string.size(), nullptr, 0);
			std::wstring result(size, 0);
			MultiByteToWideChar(CP_UTF8, 0, &string[0], (int) string.size(), &result[0], size);
			return result;
		}

#else

		std::string LAMBDACOMMON_API convertWStringToString(std::wstring wstring)
		{
			std::string out;
			std::copy(wstring.begin(), wstring.end(), std::back_inserter(out));
			return out;
		}

		std::wstring LAMBDACOMMON_API convertStringToWString(std::string string)
		{
			std::wstring out;
			std::copy(string.begin(), string.end(), std::back_inserter(out));
			return out;
		}

#endif

	}
}