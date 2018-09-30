/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_TERMINAL_H
#define CLAMBDACOMMON_TERMINAL_H

#include "../clambdacommon.h"

#ifdef __cplusplus
extern "C" {
#endif

enum lc_TermFormatting
{
	/*
	 * RESET
	 */
			LC_TERM_RESET = 0,
	/*
	 * SET
	 */
			LC_TERM_BOLD = 1,
	LC_TERM_DIM = 2,
	LC_TERM_UNDERLINED = 4,
	LC_TERM_BLINK = 5,
	LC_TERM_REVERSE = 7,
	LC_TERM_HIDDEN = 8,
	/*
	 * Foreground
	 */
			LC_TERM_DEFAULT_FCOLOR = 39,
	LC_TERM_BLACK = 30,
	LC_TERM_RED = 31,
	LC_TERM_GREEN = 32,
	LC_TERM_YELLOW = 33,
	LC_TERM_BLUE = 34,
	LC_TERM_MAGENTA = 35,
	LC_TERM_CYAN = 36,
	LC_TERM_LIGHT_GRAY = 37,
	LC_TERM_DARK_GRAY = 90,
	LC_TERM_LIGHT_RED = 91,
	LC_TERM_LIGHT_GREEN = 92,
	LC_TERM_LIGHT_YELLOW = 93,
	LC_TERM_LIGHT_BLUE = 94,
	LC_TERM_LIGHT_MAGENTA = 95,
	LC_TERM_LIGHT_CYAN = 96,
	LC_TERM_WHITE = 97,
	/*
	 * Background
	 */
			LC_TERM_DEFAULT_BCOLOR = 49,
	LC_TERM_B_BLACK = 40,
	LC_TERM_B_RED = 41,
	LC_TERM_B_GREEN = 42,
	LC_TERM_B_YELLOW = 43,
	LC_TERM_B_BLUE = 44,
	LC_TERM_B_MAGENTA = 45,
	LC_TERM_B_CYAN = 46,
	LC_TERM_B_LIGHT_GRAY = 47,
	LC_TERM_B_DARK_GRAY = 100,
	LC_TERM_B_LIGHT_RED = 101,
	LC_TERM_B_LIGHT_GREEN = 102,
	LC_TERM_B_LIGHT_YELLOW = 103,
	LC_TERM_B_LIGHT_BLUE = 104,
	LC_TERM_B_LIGHT_MAGENTA = 105,
	LC_TERM_B_LIGHT_CYAN = 106,
	LC_TERM_B_WHITE = 107
};
typedef enum lc_TermFormatting lc_TermFormatting;

struct lc_TermSize
{
	unsigned short columns;
	unsigned short rows;
};
typedef struct lc_TermSize lc_TermSize;

void lc_term_setFormat(lc_TermFormatting formatting);

void lc_term_setFormats(int count, ...);

void lc_term_clear();

/*!
 * Sets the cursor at the specified position.
 * @param x The x position.
 * @param y The y position.
 */
void lc_term_setCursorPosition(unsigned short x, unsigned short y);

/*
 * Sound manipulations
 */

/*!
 * Do a beep/bell in the terminal.
 * @param stream The stream to affect.
 * @return The affected stream.
 */
void lc_term_bell();

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
bool lc_term_setup();

void lc_term_useUTF8();

/*!
 * Gets the title of the terminal.
 * Warning: Only works on Windows, on Unix system this returns an empty string.
 * @return The title of the terminal.
 */
const char *lc_term_getTerminalTitle();

/*!
 * Sets the title of the terminal.
 * @param title The new title of the terminal.
 * @return True if success else false.
 */
bool lc_term_setTerminalTitle(const char *title);

/*!
 * Gets the terminal's size.
 * @return The {@code TermSize} struct describing the terminal's size.
 */
lc_TermSize lc_term_getTerminalSize();

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_TERMINAL_H