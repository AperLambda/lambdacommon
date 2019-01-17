/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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

void lc_term_set_format(lc_TermFormatting formatting)
{
	std::cout << static_cast<lambdacommon::terminal::TermFormatting>(formatting);
}

void lc_term_set_formats(int count, ...)
{
	std::vector<lambdacommon::terminal::TermFormatting> formats;
	va_list vaList;

	va_start(vaList, count);

	for (size_t i = 0; i < static_cast<size_t>(count); i++) {
		formats.push_back(static_cast<lambdacommon::terminal::TermFormatting>(va_arg(vaList, int)));
	}

	va_end(vaList);

	std::cout << formats;
}

void lc_term_clear()
{
	lambdacommon::terminal::clear();
}

void lc_term_set_cursor_position(unsigned short x, unsigned short y)
{
	lambdacommon::terminal::set_cursor_position(x, y);
}

void lc_term_bell()
{
	lambdacommon::terminal::bell();
}

bool lc_term_setup()
{
	return lambdacommon::terminal::setup();
}

void lc_term_use_ut8()
{
	lambdacommon::terminal::use_utf8();
}

const char *lc_term_get_terminal_title()
{
	static char title[TERM_TITLE_MAX_SIZE];
	lambdacommon::terminal::get_title().copy(title, TERM_TITLE_MAX_SIZE);
	return title;
}

bool lc_term_set_terminal_title(const char *title)
{
	return lambdacommon::terminal::set_title(title);
}

lc_TermSize lc_term_get_terminal_size()
{
	lc_TermSize term_size{};
	auto cpp_term_size = lambdacommon::terminal::get_size();
	term_size.columns = cpp_term_size.get_width();
	term_size.rows = cpp_term_size.get_height();
	return term_size;
}
