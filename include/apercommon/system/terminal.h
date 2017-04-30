/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef APERCOMMON_TERMINAL_H
#define APERCOMMON_TERMINAL_H

#include "os.h"
#include <vector>

namespace apercommon
{
    namespace terminal
    {
        enum TermFormatting
        {
            /*
             * RESET
             */
                    RESET = 0,
            /*
             * SET
             */
                    BOLD = 1,
            DIM = 2,
            UNDERLINED = 4,
            BLINK = 5,
            REVERSE = 7,
            HIDDEN = 8,
            /*
             * Foreground
             */
                    DEFAULT_FCOLOR = 39,
            BLACK = 30,
            RED = 31,
            GREEN = 32,
            YELLOW = 33,
            BLUE = 34,
            MAGENTA = 35,
            CYAN = 36,
            LIGHT_GRAY = 37,
            DARK_GRAY = 90,
            LIGHT_RED = 91,
            LIGHT_GREEN = 92,
            LIGHT_YELLOW = 93,
            LIGHT_BLUE = 94,
            LIGHT_MAGENTA = 95,
            LIGHT_CYAN = 96,
            WHITE = 97,
            /*
             * Background
             */
                    DEFAULT_BCOLOR = 49,
            B_BLACK = 40,
            B_RED = 41,
            B_GREEN = 42,
            B_YELLOW = 43,
            B_BLUE = 44,
            B_MAGENTA = 45,
            B_CYAN = 46,
            B_LIGHT_GRAY = 47,
            B_DARK_GRAY = 100,
            B_LIGHT_RED = 101,
            B_LIGHT_GREEN = 102,
            B_LIGHT_YELLOW = 103,
            B_LIGHT_BLUE = 104,
            B_LIGHT_MAGENTA = 105,
            B_LIGHT_CYAN = 106,
            B_WHITE = 107
        };

        extern std::ostream APERCOMMON_API &operator<<(std::ostream &stream, TermFormatting termFormatting);

        extern std::ostream APERCOMMON_API &operator<<(std::ostream &stream, std::vector<TermFormatting> termFormatting);

        /*!
         * This function will erase the actual line in the stream.
         *
         * @param stream Stream will be affect.
         * @return The current stream.
         */
        extern std::ostream APERCOMMON_API &eraseActualLine(std::ostream &stream);

        /*!
         * This function will do a carriage return in the stream.
         *
         * @param stream Stream will be affect.
         * @return The current stream.
         */
        extern std::ostream APERCOMMON_API &carriageReturn(std::ostream &stream);
    }
}

#endif //APERCOMMON_TERMINAL_H
