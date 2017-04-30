/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include <sstream>
#include "../include/apercommon/string.h"
#include <iterator>

using namespace std;

namespace apercommon
{
    namespace aperstring
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
    }
}
