/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/clambdacommon/system/terminal.h"
#include <lambdacommon/system/terminal.h>
#include <stdarg.h>

#if defined(LAMBDA_WINDOWS) || defined(__CYGWIN__)

# include <Windows.h>

# define TERM_TITLE_MAX_SIZE MAX_PATH
#else
# define TERM_TITLE_MAX_SIZE 256

# include <string.h>

#endif

void lc_term_setFormat(lc_TermFormatting formatting)
{
	std::cout << static_cast<lambdacommon::terminal::TermFormatting>(formatting);
}

void lc_term_setFormats(int count, ...)
{
	std::vector<lambdacommon::terminal::TermFormatting> formats;
	va_list vaList;

	va_start(vaList, count);

	for (size_t i = 0; i < static_cast<size_t>(count); i++)
	{
		formats.push_back(static_cast<lambdacommon::terminal::TermFormatting>(va_arg(vaList, int)));
	}

	va_end(vaList);

	std::cout << formats;
}

void lc_term_clear()
{
	lambdacommon::terminal::clear();
}

void lc_term_setCursorPosition(unsigned short x, unsigned short y)
{
	lambdacommon::terminal::setCursorPosition(x, y);
}

void lc_term_bell()
{
	lambdacommon::terminal::bell();
}

bool lc_term_setup()
{
	return lambdacommon::terminal::setup();
}

void lc_term_useUTF8()
{
	lambdacommon::terminal::useUTF8();
}

const char *lc_term_getTerminalTitle()
{
	static char title[TERM_TITLE_MAX_SIZE];
	lambdacommon::terminal::getTerminalTitle().copy(title, TERM_TITLE_MAX_SIZE);
	return title;
}

bool lc_term_setTerminalTitle(const char *title)
{
	return lambdacommon::terminal::setTerminalTitle(title);
}

lc_TermSize lc_term_getTerminalSize()
{
	lc_TermSize termSize{};
	auto cppTermSize = lambdacommon::terminal::getTerminalSize();
	termSize.columns = cppTermSize.columns;
	termSize.rows = cppTermSize.rows;
	return termSize;
}