/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/system/terminal.h"
#include "../../include/lambdacommon/lstring.h"

#if defined(LAMBDA_WINDOWS) || defined(__CYGWIN__)

#  define WIN_FRIENDLY

#  include <io.h>
#  include <Windows.h>

#  ifdef __CYGWIN__
#    include <unistd.h>
#  endif

#  ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#    define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#  endif

#else

#  include <sys/ioctl.h>
#  include <unistd.h>

#endif

namespace lambdacommon
{
	namespace terminal
	{
		bool use_ansi_escape_codes = false;

		/*
		 * INTERNAL
		 */
		inline
		FILE *get_standard_stream(const std::ostream &stream)
		{
			if (&stream == &std::cout)
				return stdout;
			else if ((&stream == &std::cerr) || (&stream == &std::clog))
				return stderr;

			return nullptr;
		}

#ifdef WIN_FRIENDLY

		HANDLE getTermHandle(std::ostream &stream)
		{
			// Get terminal handle
			HANDLE h_terminal = INVALID_HANDLE_VALUE;
			if (&stream == &std::cout)
				h_terminal = GetStdHandle(STD_OUTPUT_HANDLE);
			else if (&stream == &std::cerr)
				h_terminal = GetStdHandle(STD_ERROR_HANDLE);
			return h_terminal;
		}

		void cls(HANDLE hConsole)
		{
			COORD coord_screen{0, 0};    // home for the cursor
			DWORD c_chars_written;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			DWORD dw_con_size;

			// Get the number of character cells in the current buffer.
			if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
			{
				return;
			}

			dw_con_size = static_cast<DWORD>(csbi.dwSize.X * csbi.dwSize.Y);

			// Fill the entire screen with blanks.
			if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
											(TCHAR) ' ',     // Character to write to the buffer
											dw_con_size,       // Number of cells to write
											coord_screen,     // Coordinates of first cell
											&c_chars_written))// Receive number of characters written
				return;

			// Get the current text attribute.
			if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
				return;

			// Set the buffer's attributes accordingly.
			if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
											csbi.wAttributes, // Character attributes to use
											dw_con_size,        // Number of cells to set attribute
											coord_screen,      // Coordinates of first cell
											&c_chars_written)) // Receive number of characters written
				return;

			// Put the cursor at its home coordinates.

			SetConsoleCursorPosition(hConsole, coord_screen);
		}

		inline void win_change_attributes(std::ostream &stream, int foreground, int background)
		{
			static WORD default_attributes = 0;

			// Get terminal handle
			auto hTerminal = getTermHandle(stream);

			// Save default terminal attributes if it unsaved
			if (!default_attributes)
			{
				CONSOLE_SCREEN_BUFFER_INFO info;
				if (!GetConsoleScreenBufferInfo(hTerminal, &info))
					return;
				default_attributes = info.wAttributes;
			}

			// Restore all default settings
			if (foreground == -1 && background == -1)
			{
				SetConsoleTextAttribute(hTerminal, default_attributes);
				return;
			}

			// Get current settings
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (!GetConsoleScreenBufferInfo(hTerminal, &info))
				return;

			if (foreground != -1)
			{
				info.wAttributes &= ~(info.wAttributes & 0x0F);
				info.wAttributes |= static_cast<WORD>(foreground);
			}

			if (background != -1)
			{
				info.wAttributes &= ~(info.wAttributes & 0xF0);
				info.wAttributes |= static_cast<WORD>(background);
			}

			SetConsoleTextAttribute(hTerminal, info.wAttributes);
		}

#endif // LAMBDA_WINDOWS

		/*
		 * IMPLEMENTATION
		 */

		std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, TermFormatting term_formatting)
		{
			std::vector<TermFormatting> formats{term_formatting};
			return stream << formats;
		}

		std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, std::vector<TermFormatting> term_formatting)
		{
			if (is_tty(stream))
			{
#ifdef WIN_FRIENDLY
				if (!use_ansi_escape_codes)
				{
					for (auto format : term_formatting)
					{
						switch (format)
						{
							case RESET:
								win_change_attributes(stream, -1, -1);
								break;
							case BLACK:
								win_change_attributes(stream, 0x0, -1);
								break;
							case BLUE:
								win_change_attributes(stream, FOREGROUND_BLUE, -1);
								break;
							case GREEN:
								win_change_attributes(stream, FOREGROUND_GREEN, -1);
								break;
							case CYAN:
								win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN, -1);
								break;
							case RED:
								win_change_attributes(stream, FOREGROUND_RED, -1);
								break;
							case MAGENTA:
								win_change_attributes(stream, 0x5, -1);
								break;
							case YELLOW:
								win_change_attributes(stream, 0x6, -1);
								break;
							case LIGHT_GRAY:
								win_change_attributes(stream, 0x7, -1);
								break;
							case DARK_GRAY:
								win_change_attributes(stream, 0x8, -1);
								break;
							case LIGHT_BLUE:
								win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_INTENSITY, -1);
								break;
							case LIGHT_GREEN:
								win_change_attributes(stream, FOREGROUND_GREEN | FOREGROUND_INTENSITY, -1);
								break;
							case LIGHT_CYAN:
								win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
													  -1);
								break;
							case LIGHT_RED:
								win_change_attributes(stream, FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
								break;
							case LIGHT_MAGENTA:
								win_change_attributes(stream, 0xD, -1);
								break;
							case LIGHT_YELLOW:
								win_change_attributes(stream, 0xE, -1);
								break;
							case WHITE:
								win_change_attributes(stream, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, -1);
								break;
							case B_BLACK:
								win_change_attributes(stream, -1, 0x00);
								break;
							case B_BLUE:
								win_change_attributes(stream, -1, BACKGROUND_BLUE);
								break;
							case B_GREEN:
								win_change_attributes(stream, -1, BACKGROUND_GREEN);
								break;
							case B_CYAN:
								win_change_attributes(stream, -1, BACKGROUND_BLUE | BACKGROUND_GREEN);
								break;
							case B_RED:
								win_change_attributes(stream, -1, BACKGROUND_RED);
								break;
							case B_MAGENTA:
								win_change_attributes(stream, -1, (0x5 << 4));
								break;
							case B_YELLOW:
								win_change_attributes(stream, -1, (0x6 << 4));
								break;
							case B_LIGHT_GRAY:
								win_change_attributes(stream, -1, (0x7 << 4));
								break;
							case B_DARK_GRAY:
								win_change_attributes(stream, -1, (0x8 << 4));
								break;
							case B_LIGHT_BLUE:
								win_change_attributes(stream, -1, (0x9 << 4));
								break;
							case B_LIGHT_GREEN:
								win_change_attributes(stream, -1, (0xA << 4));
								break;
							case B_LIGHT_CYAN:
								win_change_attributes(stream, -1, (0xB << 4));
								break;
							case B_LIGHT_RED:
								win_change_attributes(stream, -1, (0xC << 4));
								break;
							case B_LIGHT_MAGENTA:
								win_change_attributes(stream, -1, (0xD << 4));
								break;
							case B_LIGHT_YELLOW:
								win_change_attributes(stream, -1, (0xE << 4));
								break;
							case B_WHITE:
								win_change_attributes(stream, -1, (0xF << 4));
								break;
							default:
								break;
						}
					}
					return stream;
				}
				else
					goto write_ansi;
#else
				goto write_ansi;
#endif
			} else
				goto write_ansi;
			write_ansi:
			std::string ansi_sequence{((char) 0x1B)};
			ansi_sequence += "[";
			auto formattings = term_formatting.size();
			for (size_t i = 0; i < formattings; i++)
			{
				std::string str = std::to_string(static_cast<int>(term_formatting[i]));
				if (i != formattings - 1)
					ansi_sequence += (str + ";");
				else
					ansi_sequence += str;
			}
			ansi_sequence += "m";
			stream << ansi_sequence;
			return stream;
		}

		std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, std::vector<std::string> string_vector)
		{
			stream << lstring::to_string(string_vector);
			return stream;
		}

		std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, const Color &color)
		{
			stream << color.to_string();
			return stream;
		}

		std::ostream LAMBDACOMMON_API &erase_current_line(std::ostream &stream)
		{
#ifdef WIN_FRIENDLY
#else
			stream << "\033[2K";
#endif
			return stream;
		}

		std::ostream LAMBDACOMMON_API &clear(std::ostream &stream)
		{
#ifdef WIN_FRIENDLY
			if (is_tty(stream))
			{
				cls(getTermHandle(stream));
				return stream;
			}
#endif
			stream << "\033[2J";
			return stream;
		}

		void LAMBDACOMMON_API set_cursor_position(unsigned short x, unsigned short y, std::ostream &stream)
		{
#ifdef WIN_FRIENDLY
			if (is_tty(stream))
			{
				COORD coord;
				coord.X = x;
				coord.Y = y;
				SetConsoleCursorPosition(getTermHandle(stream), coord);
				return;
			}
#endif
			stream << ("\033[" + std::to_string(y) + ';' + std::to_string(x) + "H");
		}

		/*
		 * Sound manipulations
		 */

		std::ostream LAMBDACOMMON_API &bell(std::ostream &stream)
		{
			stream << ((char) 0x7);
			return stream;
		}

		/*
		 * Terminal manipulations
		 */

		bool LAMBDACOMMON_API setup()
		{
#ifdef  WIN_FRIENDLY
			HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
			if (h_out != INVALID_HANDLE_VALUE)
			{
				DWORD dwMode = 0;
				if (GetConsoleMode(h_out, &dwMode))
				{
					dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
					if (SetConsoleMode(h_out, dwMode))
						use_ansi_escape_codes = true;
				}
			}
#else
#endif
			use_utf8();
			return use_ansi_escape_codes;
		}

		void LAMBDACOMMON_API use_utf8()
		{
#ifdef WIN_FRIENDLY
			SetConsoleCP(CP_UTF8);
			SetConsoleOutputCP(CP_UTF8);
#else
			setlocale(LC_ALL, "en_US.UTF-8");
#endif
		}

		bool LAMBDACOMMON_API is_tty(const std::ostream &stream)
		{
		FILE *std_stream = get_standard_stream(stream);

#ifdef LAMBDA_WINDOWS
		return ::_isatty(_fileno(std_stream)) != 0;
#else
		return static_cast<bool>(::isatty(fileno(std_stream)));
#endif
		}

		std::string LAMBDACOMMON_API get_title()
		{
#ifdef WIN_FRIENDLY
			TCHAR current_title[MAX_PATH];
			if (GetConsoleTitle(current_title, MAX_PATH))
				return {current_title};
			else
				return "";
#else
			return "";
#endif
		}

		bool LAMBDACOMMON_API set_title(const std::string &title, std::ostream &stream)
		{
#ifdef WIN_FRIENDLY
			if (is_tty(stream))
				return (bool) SetConsoleTitle(TEXT(title.c_str()));
#endif
			stream << ("\033]0;" + title + "\007");
			return true;
		}

		TermSize LAMBDACOMMON_API get_size()
		{
			TermSize size{};
#ifdef WIN_FRIENDLY
			CONSOLE_SCREEN_BUFFER_INFO csbi;

			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			size.columns = static_cast<unsigned short>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
			size.rows = static_cast<unsigned short>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#else
			struct winsize w{};
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			size.columns = w.ws_col;
			size.rows = w.ws_row;
#endif
			return size;
		}
	}
}

#undef WIN_FRIENDLY