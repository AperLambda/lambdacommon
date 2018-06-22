/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_TERMINAL_H
#define LAMBDACOMMON_TERMINAL_H

#include "os.h"
#include <vector>
#include <iostream>

namespace lambdacommon
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

		struct TermSize
		{
			unsigned short columns;
			unsigned short rows;
		};

		extern std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, TermFormatting termFormatting);

		extern std::ostream LAMBDACOMMON_API &
		operator<<(std::ostream &stream, std::vector<TermFormatting> termFormatting);

		/*!
		 * This function will erase the current line in the stream.
		 *
		 * @param stream Stream will be affect.
		 * @return The current stream.
		 */
		extern std::ostream LAMBDACOMMON_API &eraseCurrentLine(std::ostream &stream = std::cout);

		extern std::ostream LAMBDACOMMON_API &clear(std::ostream &stream = std::cout);

		/*!
		 * This function set the cursor on a new position.
		 */
		extern void LAMBDACOMMON_API
		setCursorPosition(unsigned short x, unsigned short y, std::ostream &stream = std::cout);

		/*!
		 * This function set the cursor on a new position.
		 */
		inline void setCursorPosition(std::pair<unsigned short, unsigned short> pos, std::ostream &stream = std::cout)
		{
			setCursorPosition(pos.first, pos.second, stream);
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
		 * Setup the terminal.
		 * On Windows it enables ANSI escape codes if available.
		 * On every system:
		 *   - Calls useUTF8().
		 * @return True if success else false.
		 */
		extern bool LAMBDACOMMON_API setup();

		extern void LAMBDACOMMON_API useUTF8();

		 /*!
		  * Gets whether the specified stream is a TTY.
		  * @param stream THe specified stream to check, default is {@code std::cout}.
		  * @return True if the stream is a TTY, else false.
		  */
		extern bool LAMBDACOMMON_API isTTY(const std::ostream &stream = std::cout);

		/*!
		 * Returns the title of the terminal. Warning: Only works on Windows, on Unix system this returns an empty string.
		 * @return The title of the terminal.
		 */
		extern std::string LAMBDACOMMON_API getTerminalTitle();

		/*!
		 * Sets the title of the terminal.
		 * @param title The new title of the terminal.
		 * @return True if success else false.
		 */
		extern bool LAMBDACOMMON_API setTerminalTitle(const std::string &title, std::ostream &stream = std::cout);

		/*!
		 * Gets the terminal's size.
		 * @return The {@code TermSize} struct describing the terminal's size.
		 */
		extern TermSize LAMBDACOMMON_API getTerminalSize();
	}
}

#endif //LAMBDACOMMON_TERMINAL_H