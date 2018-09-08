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

enum lcommon_TermFormatting
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
typedef enum lcommon_TermFormatting lcommon_TermFormatting;

struct lcommon_TermSize
{
	unsigned short columns;
	unsigned short rows;
};
typedef struct lcommon_TermSize lcommon_TermSize;

void lcommon_term_setFormat(lcommon_TermFormatting formatting);

void lcommon_term_setFormats(int count, ...);

void lcommon_term_clear();

/*!
 * Sets the cursor at the specified position.
 * @param x The x position.
 * @param y The y position.
 */
void lcommon_term_setCursorPosition(unsigned short x, unsigned short y);

/*
 * Sound manipulations
 */

/*!
 * Do a beep/bell in the terminal.
 * @param stream The stream to affect.
 * @return The affected stream.
 */
void lcommon_term_bell();

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
bool lcommon_term_setup();

void lcommon_term_useUTF8();

/*!
 * Gets the title of the terminal.
 * Warning: Only works on Windows, on Unix system this returns an empty string.
 * @return The title of the terminal.
 */
const char *lcommon_term_getTerminalTitle();

/*!
 * Sets the title of the terminal.
 * @param title The new title of the terminal.
 * @return True if success else false.
 */
bool lcommon_term_setTerminalTitle(const char *title);

/*!
 * Gets the terminal's size.
 * @return The {@code TermSize} struct describing the terminal's size.
 */
lcommon_TermSize lcommon_term_getTerminalSize();

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_TERMINAL_H