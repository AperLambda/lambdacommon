/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include <sstream>
#include "../include/lambdacommon/string.h"
#include <iterator>

using namespace std;

namespace lambdacommon
{
    namespace lambdastring
    {
        template<typename Out>
        void split(const std::string &s, char delim, Out result)
        {
            std::stringstream ss;
            ss.str(s);
            std::string item;
            while (std::getline(ss, item, delim))
            {
                *(result++) = item;
            }
        }

        std::vector<std::string> split(const std::string &s, char delimiter)
        {
            std::vector<std::string> elems;
            split(s, delimiter, std::back_inserter(elems));
            return elems;
        }

        bool equalsIgnoreCase(const char a, const char b)
        {
            return tolower(a) == tolower(b);
        }

        bool equals(string const &a, string const &b)
        {
            if (a.length() == b.length())
                return a == b;
            else
                return false;
        }

        bool equalsIgnoreCase(string const &a, string const &b)
        {
            if (a.length() == b.length())
            {
                return std::equal(begin(a), end(a), begin(b), [](const char charA, const char charB)
                    { return equalsIgnoreCase(charA, charB); });
            }
            else
                return false;
        }

        string replaceAll(std::string subject, const char &from, const char &to)
        {
            return replaceAll(subject, to_string(from), to_string(to));
        }

        string replaceAll(std::string subject, const std::string &from, const std::string &to)
        {
            size_t start_pos = 0;
            while ((start_pos = subject.find(from, start_pos)) != std::string::npos)
            {
                subject.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }
            return subject;
        }

#ifdef LAMBDA_WINDOWS
#ifndef __GNUC__

#include <windows.h>

        string convertWStringToString(std::wstring wstring)
        {
            std::string string;
            if (!wstring.empty())
            {
                int size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int) wstring.size(), NULL, 0, NULL, NULL);
                string.resize(size, 0);
                WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int) wstring.size(), &string[0], size, NULL, NULL);
            }
            return string;
        }

        wstring convertStringToWString(std::string string)
        {
            int size = MultiByteToWideChar(CP_UTF8, 0, &string[0], (int) string.size(), NULL, 0);
            wstring result(size, 0);
            MultiByteToWideChar(CP_UTF8, 0, &string[0], (int) string.size(), &result[0], size);
            return result;
        }

#else

        string convertWStringToString(std::wstring wstring)
        {
            string out;
            std::copy(wstring.begin(), wstring.end(), std::back_inserter(out));
            return out;
        }

        wstring convertStringToWString(std::string string)
        {
            wstring out;
            std::copy(string.begin(), string.end(), std::back_inserter(out));
            return out;
        }

#endif
#endif

    }
}
