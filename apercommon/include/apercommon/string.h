/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef APERCOMMON_STRING_H
#define APERCOMMON_STRING_H

#include <vector>
#include "apercommon.h"

namespace apercommon
{
    namespace aperstring
    {
        template<typename Out>
        extern void APERCOMMON_EXPORTS split(const std::string &s, char delim, Out result);

        /**
         * Splits a string into a vector with a delimiter.
         * @param s String to split.
         * @param delim Delimiter.
         * @return The split string as a vector.
         */
        extern std::vector<std::string> APERCOMMON_EXPORTS split(const std::string &s, char delim);

        /**
         * Checks if two chars are equal with case insensitive.
         * @param a One of two chars.
         * @param b One of two chars.
         * @return True if they are equal with case insensitive else false.
         */
        extern bool APERCOMMON_EXPORTS equalsIgnoreCase(const char a, const char b);

        /**
         * Checks if two strings are equal.
         * @param a One of two strings.
         * @param b One of two strings.
         * @return True if they are equal else false.
         */
        extern bool APERCOMMON_EXPORTS equals(std::string const &a, std::string const &b);

        /**
         * Checks if two strings are equal with case insensitive.
         * @param a One of two strings.
         * @param b One of two strings.
         * @return True if they are equal with case insensitive else false.
         */
        extern bool APERCOMMON_EXPORTS equalsIgnoreCase(std::string const &a, std::string const &b);
    }
}

#endif //APERCOMMON_STRING_H
