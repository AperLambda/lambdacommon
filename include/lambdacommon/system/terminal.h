/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_TERMINAL_H
#define LAMBDACOMMON_TERMINAL_H

#include "os.h"
#include "../graphics/color.h"
#include "../types.h"
#include <vector>
#include <iostream>

namespace lambdacommon
{
    namespace terminal
    {
        enum TermFormatting
        {
            RESET           = 0,
            /*
             * SET
             */
            BOLD            = 1,
            DIM             = 2,
            UNDERLINED      = 4,
            BLINK           = 5,
            REVERSE         = 7,
            HIDDEN          = 8,
            /*
             * Foreground
             */
            DEFAULT_FCOLOR  = 39,
            BLACK           = 30,
            RED             = 31,
            GREEN           = 32,
            YELLOW          = 33,
            BLUE            = 34,
            MAGENTA         = 35,
            CYAN            = 36,
            LIGHT_GRAY      = 37,
            DARK_GRAY       = 90,
            LIGHT_RED       = 91,
            LIGHT_GREEN     = 92,
            LIGHT_YELLOW    = 93,
            LIGHT_BLUE      = 94,
            LIGHT_MAGENTA   = 95,
            LIGHT_CYAN      = 96,
            WHITE           = 97,
            /*
             * Background
             */
            DEFAULT_BCOLOR  = 49,
            B_BLACK         = 40,
            B_RED           = 41,
            B_GREEN         = 42,
            B_YELLOW        = 43,
            B_BLUE          = 44,
            B_MAGENTA       = 45,
            B_CYAN          = 46,
            B_LIGHT_GRAY    = 47,
            B_DARK_GRAY     = 100,
            B_LIGHT_RED     = 101,
            B_LIGHT_GREEN   = 102,
            B_LIGHT_YELLOW  = 103,
            B_LIGHT_BLUE    = 104,
            B_LIGHT_MAGENTA = 105,
            B_LIGHT_CYAN    = 106,
            B_WHITE         = 107
        };

        extern std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, TermFormatting term_formatting);

        extern std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, const std::vector<TermFormatting> &term_formatting);

        extern std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, const std::vector<std::string> &string_vector);

        inline std::vector<TermFormatting> formats(std::initializer_list<TermFormatting> format_list)
        {
            return format_list;
        }

        /*!
         * This function will erase the current line in the stream.
         *
         * @param stream Stream will be affect.
         * @return The current stream.
         */
        extern std::ostream LAMBDACOMMON_API &erase_current_line(std::ostream &stream = std::cout);

        /*!
         * Clears the specified stream.
         * @param stream The stream to affect.
         * @return The affected stream.
         */
        extern std::ostream LAMBDACOMMON_API &clear(std::ostream &stream = std::cout);

        /*!
         * Gets the cursor position in the tty.
         * @param stream The stream handle of the tty.
         * @return The position of the cursor.
         */
        extern Point2D_u16 LAMBDACOMMON_API get_cursor_position(std::ostream &stream = std::cout);

        /*!
         * Sets the cursor at the specified position.
         * @param x The x position.
         * @param y The y position.
         * @param stream The stream to which it applies.
         */
        extern void LAMBDACOMMON_API set_cursor_position(unsigned short x, unsigned short y, std::ostream &stream = std::cout);

        /*!
         * Sets the cursor at the specified position.
         * @param pos The position.
         * @param stream The stream to which it applies.
         */
        inline void set_cursor_position(const Point2D_u16 &pos, std::ostream &stream = std::cout)
        {
            set_cursor_position(pos.get_x(), pos.get_y(), stream);
        }

        /*!
         * Sets the cursor at the specified position.
         * @param pos The position.
         * @param stream The stream to which it applies.
         */
        inline void set_cursor_position(std::pair<unsigned short, unsigned short> pos, std::ostream &stream = std::cout)
        {
            set_cursor_position(pos.first, pos.second, stream);
        }

        /*
         * Sound manipulations
         */

        /*!
         * Do a beep/bell in the terminal.
         * @param stream The stream to affect.
         * @return The affected stream.
         */
        extern std::ostream LAMBDACOMMON_API &bell(std::ostream &stream = std::cout);

        /*
         * Terminal manipulations
         */

        /*!
         * Setups the terminal.
         * On Windows it enables ANSI escape codes if available.
         * On every system:
         *   - Calls useUTF8().
         * @return True if success else false.
         */
        extern bool LAMBDACOMMON_API setup();

        extern bool LAMBDACOMMON_API use_utf8();

        /*!
         * Checks whether the terminal output and input has UTF-8 enabled.
         * @return True if the terminal has UTF-8 support, else false.
         */
        extern bool LAMBDACOMMON_API has_utf8();

        /*!
         * Gets whether the specified stream is a TTY.
         * @param stream THe specified stream to check, default is {@code std::cout}.
         * @return True if the stream is a TTY, else false.
         */
        extern bool LAMBDACOMMON_API is_tty(const std::ostream &stream = std::cout);

        /*!
         * Gets the title of the terminal.
         * Warning: Only works on Windows, on Unix systems this returns an empty string.
         * @return The title of the terminal.
         */
        extern std::string LAMBDACOMMON_API get_title();

        /*!
         * Sets the title of the terminal.
         * @param title The new title of the terminal.
         * @return True if success else false.
         */
        extern bool LAMBDACOMMON_API set_title(const std::string &title, std::ostream &stream = std::cout);

        /*!
         * Gets the terminal's size.
         * @param stream The stream handle.
         * @return The {@code TermSize} struct describing the terminal's size.
         */
        extern const Size2D_u16 LAMBDACOMMON_API get_size(const std::ostream &stream = std::cout);
    }
}

#endif //LAMBDACOMMON_TERMINAL_H
