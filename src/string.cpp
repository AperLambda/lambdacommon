/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
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
        void split(const string &s, char delim, Out result)
        {
            stringstream ss;
            ss.str(s);
            string item;
            while (getline(ss, item, delim))
            {
                *(result++) = item;
            }
        }

        vector<string> split(const string &s, char delimiter)
        {
            vector<string> elems;
            split(s, delimiter, back_inserter(elems));
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
                return equal(begin(a), end(a), begin(b), [](const char charA, const char charB)
                    { return equalsIgnoreCase(charA, charB); });
            }
            else
                return false;
        }

        string replaceAll(string subject, const char &from, const char &to)
        {
            return replaceAll(subject, to_string(from), to_string(to));
        }

        string replaceAll(string subject, const string &from, const string &to)
        {
            size_t start_pos = 0;
            while ((start_pos = subject.find(from, start_pos)) != string::npos)
            {
                subject.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }
            return subject;
        }

#ifdef LAMBDA_WINDOWS
#ifndef __GNUC__

#include <windows.h>

        string convertWStringToString(wstring wstring)
        {
            string string;
            if (!wstring.empty())
            {
                int size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int) wstring.size(), NULL, 0, NULL, NULL);
                string.resize(size, 0);
                WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int) wstring.size(), &string[0], size, NULL, NULL);
            }
            return string;
        }

        wstring convertStringToWString(string string)
        {
            int size = MultiByteToWideChar(CP_UTF8, 0, &string[0], (int) string.size(), NULL, 0);
            wstring result(size, 0);
            MultiByteToWideChar(CP_UTF8, 0, &string[0], (int) string.size(), &result[0], size);
            return result;
        }

#else

        string convertWStringToString(wstring wstring)
        {
            string out;
            copy(wstring.begin(), wstring.end(), back_inserter(out));
            return out;
        }

        wstring convertStringToWString(string string)
        {
            wstring out;
            copy(string.begin(), string.end(), back_inserter(out));
            return out;
        }

#endif
#endif

    }
}