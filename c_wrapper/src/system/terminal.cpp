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

#if defined(LAMBDA_WINDOWS) || defined(__CYGWIN__)

# include <Windows.h>

# define TERM_TITLE_MAX_SIZE MAX_PATH
#else
# define TERM_TITLE_MAX_SIZE 256

# include <string.h>

#endif

void lcommon_term_setFormat(lcommon_TermFormatting formatting)
{
	std::cout << static_cast<lambdacommon::terminal::TermFormatting>(formatting);
}

void lcommon_term_setFormats(int count, ...)
{
	std::vector<lambdacommon::terminal::TermFormatting> formats;
	va_list vaList;

	va_start(vaList, count);

	for (size_t i = 0; i < count; i++)
	{
		formats.push_back(static_cast<lambdacommon::terminal::TermFormatting>(va_arg(vaList, int)));
	}

	va_end(vaList);

	std::cout << formats;
}

void lcommon_term_clear()
{
	lambdacommon::terminal::clear();
}

void lcommon_term_setCursorPosition(unsigned short x, unsigned short y)
{
	lambdacommon::terminal::setCursorPosition(x, y);
}

void lcommon_term_bell()
{
	lambdacommon::terminal::bell();
}

bool lcommon_term_setup()
{
	return lambdacommon::terminal::setup();
}

void lcommon_term_useUTF8()
{
	lambdacommon::terminal::useUTF8();
}

const char *lcommon_term_getTerminalTitle()
{
	static char title[TERM_TITLE_MAX_SIZE];
	lambdacommon::terminal::getTerminalTitle().copy(title, TERM_TITLE_MAX_SIZE);
	return title;
}

bool lcommon_term_setTerminalTitle(const char *title)
{
	return lambdacommon::terminal::setTerminalTitle(title);
}

lcommon_TermSize lcommon_term_getTerminalSize()
{
	lcommon_TermSize termSize{};
	auto cppTermSize = lambdacommon::terminal::getTerminalSize();
	termSize.columns = cppTermSize.columns;
	termSize.rows = cppTermSize.rows;
	return termSize;
}