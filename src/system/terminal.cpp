/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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
#  include <errno.h>
#  include <termios.h>
#endif

namespace lambdacommon
{
	namespace terminal
	{
		bool _use_ansi_escape_codes = false;
		bool _has_utf8 = false;

		/*
		 * INTERNAL
		 */
		inline FILE *get_standard_stream(const std::ostream &stream)
		{
			if (&stream == &std::cout)
				return stdout;
			else if ((&stream == &std::cerr) || (&stream == &std::clog))
				return stderr;

			return nullptr;
		}

#ifdef WIN_FRIENDLY

		HANDLE get_term_handle(const std::ostream &stream)
		{
			// Get terminal handle.
			HANDLE h_terminal = INVALID_HANDLE_VALUE;
			if (&stream == &std::cout)
				h_terminal = GetStdHandle(STD_OUTPUT_HANDLE);
			else if (&stream == &std::cerr)
				h_terminal = GetStdHandle(STD_ERROR_HANDLE);
			return h_terminal;
		}

		void cls(HANDLE h_console)
		{
			COORD coord_screen{0, 0};    // Home for the cursor.
			DWORD c_chars_written;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			DWORD dw_con_size;

			// Get the number of character cells in the current buffer.
			if (!GetConsoleScreenBufferInfo(h_console, &csbi))
				return;

			dw_con_size = static_cast<DWORD>(csbi.dwSize.X * csbi.dwSize.Y);

			// Fill the entire screen with blanks.
			if (!FillConsoleOutputCharacter(h_console,            // Handle to console screen buffer.
											(TCHAR) ' ',        // Character to write to the buffer.
											dw_con_size,        // Number of cells to write.
											coord_screen,        // Coordinates of first cell.
											&c_chars_written))    // Receive number of characters written.
				return;

			// Get the current text attribute.
			if (!GetConsoleScreenBufferInfo(h_console, &csbi))
				return;

			// Set the buffer's attributes accordingly.
			if (!FillConsoleOutputAttribute(h_console,            // Handle to console screen buffer
											csbi.wAttributes,    // Character attributes to use
											dw_con_size,        // Number of cells to set attribute
											coord_screen,        // Coordinates of first cell
											&c_chars_written))    // Receive number of characters written
				return;

			// Put the cursor at its home coordinates.
			SetConsoleCursorPosition(h_console, coord_screen);
		}

		inline void win_change_attributes(std::ostream &stream, int foreground, int background)
		{
			static WORD default_attributes = 0;

			// Get terminal handle
			auto h_terminal = get_term_handle(stream);

			// Save default terminal attributes if it unsaved
			if (!default_attributes) {
				CONSOLE_SCREEN_BUFFER_INFO info;
				if (!GetConsoleScreenBufferInfo(h_terminal, &info))
					return;
				default_attributes = info.wAttributes;
			}

			// Restore all default settings
			if (foreground == -1 && background == -1) {
				SetConsoleTextAttribute(h_terminal, default_attributes);
				return;
			}

			// Get current settings
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (!GetConsoleScreenBufferInfo(h_terminal, &info))
				return;

			if (foreground != -1) {
				info.wAttributes &= ~(info.wAttributes & 0x0F);
				info.wAttributes |= static_cast<WORD>(foreground);
			}

			if (background != -1) {
				info.wAttributes &= ~(info.wAttributes & 0xF0);
				info.wAttributes |= static_cast<WORD>(background);
			}

			SetConsoleTextAttribute(h_terminal, info.wAttributes);
		}

#else
#define RD_EOF -1
#define RD_EIO -2

		static inline int rd(const int fd)
		{
			unsigned char buffer[4];

			while (1) {
				ssize_t n = read(fd, buffer, 1);
				if (n != (ssize_t) -1)
					buffer[n] = '\0';

				if (n > (ssize_t) 0)
					return buffer[0];
				else if (n == (ssize_t) 0)
					return RD_EOF;
				else if (n != (ssize_t) -1)
					return RD_EIO;
				else if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
					return RD_EIO;
			}
		}

		static inline int wr(const int fd, const char *const data, const size_t bytes)
		{
			const char *head = data;
			const char *const tail = data + bytes;
			ssize_t n;

			while (head < tail) {

				n = write(fd, head, (size_t) (tail - head));
				if (n > (ssize_t) 0)
					head += n;

				else if (n != (ssize_t) -1)
					return EIO;

				else if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
					return errno;
			}

			return 0;
		}

#endif // WIN_FRIENDLY

		/*
		 * IMPLEMENTATION
		 */

		std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, TermFormatting term_formatting)
		{
			std::vector<TermFormatting> formats{term_formatting};
			return stream << formats;
		}

		std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, const std::vector<TermFormatting> &term_formatting)
		{
			if (is_tty(stream)) {
#ifdef WIN_FRIENDLY
				if (!_use_ansi_escape_codes) {
					for (auto format : term_formatting) {
						switch (format) {
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
								win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, -1);
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
				} else
					goto write_ansi;
#else
				goto write_ansi;
#endif
			} else
				goto write_ansi;
			write_ansi:
			if (!_use_ansi_escape_codes)
				return stream;
			std::string ansi_sequence{((char) 0x1B)};
			ansi_sequence += "[";
			auto formattings = term_formatting.size();
			for (size_t i = 0; i < formattings; i++) {
				auto str = std::to_string(static_cast<int>(term_formatting[i]));
				if (i != formattings - 1)
					ansi_sequence += (str + ";");
				else
					ansi_sequence += str;
			}
			ansi_sequence += "m";
			stream << ansi_sequence;
			return stream;
		}

		std::ostream LAMBDACOMMON_API &operator<<(std::ostream &stream, const std::vector<std::string> &string_vector)
		{
			stream << lstring::to_string(string_vector);
			return stream;
		}

		void erase_current_line_ansi(std::ostream &stream)
		{
			if (_use_ansi_escape_codes)
				stream << "\033[2K";
		}

		std::ostream LAMBDACOMMON_API &erase_current_line(std::ostream &stream)
		{
#ifdef WIN_FRIENDLY
			if (_use_ansi_escape_codes) {
				erase_current_line_ansi(stream);
				stream << '\r';
			} else {
				auto current_pos = get_cursor_position(stream);
				set_cursor_position(0, current_pos.get_y(), stream);
				if (is_tty(stream)) {
					auto size = get_size(stream);
					for (size_t i = 0; i < size.get_width(); i++)
						stream << ' ';
					set_cursor_position(0, current_pos.get_y(), stream);
				}
			}
#else
			erase_current_line_ansi(stream);
			stream << '\r';
#endif
			return stream;
		}

		std::ostream LAMBDACOMMON_API &clear(std::ostream &stream)
		{
#ifdef WIN_FRIENDLY
			if (is_tty(stream)) {
				cls(get_term_handle(stream));
				return stream;
			}
#endif
			if (_use_ansi_escape_codes)
				stream << "\033[2J";
			return stream;
		}

		Point2D_u16 LAMBDACOMMON_API get_cursor_position(std::ostream &stream)
		{
			if (is_tty(stream)) {
#ifdef WIN_FRIENDLY
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				if (GetConsoleScreenBufferInfo(get_term_handle(stream), &csbi))
					return {static_cast<uint16_t>(csbi.dwCursorPosition.X), static_cast<uint16_t>(csbi.dwCursorPosition.Y)};
#else
				// Based on https://www.linuxquestions.org/questions/programming-9/get-cursor-position-in-c-947833/#post4693254.
				struct termios saved{}, temp{};
				int result;
				auto tty = fileno(get_standard_stream(stream));
				Point2D_u16 cursor_pos(0, 0);

				// Save current terminal settings.
				do {
					result = tcgetattr(tty, &saved);
				} while (result == -1 && errno == EINTR);
				if (result == -1) return cursor_pos;

				// Get current terminal settings.
				do {
					result = tcgetattr(tty, &temp);
				} while (result == -1 && errno == EINTR);
				if (result == -1) return cursor_pos;

				// Disables ICANON, ECHO and CREAD.
				temp.c_lflag &= ~ICANON;
				temp.c_lflag &= ~ECHO;
				temp.c_lflag &= ~CREAD;

				do {
					do {
						result = tcsetattr(tty, TCSANOW, &temp);
					} while (result == -1 && errno == EINTR);
					if (result == -1) break;

					// Request cursor coordinates from the terminal.
					int retval = wr(tty, "\033[6n", 4);
					if (retval)
						break;

					// Expect an ESC
					result = rd(tty);
					if (result != 27)
						break;

					// Expect [
					result = rd(tty);
					if (result != '[')
						break;

					// Parse rows.
					uint16_t rows = 0;
					result = rd(tty);
					while (result >= '0' && result <= '9') {
						rows = static_cast<uint16_t>(10 * rows + result - '0');
						result = rd(tty);
					}

					if (result != ';')
						break;

					// Parse columns.
					uint16_t columns = 0;
					result = rd(tty);
					while (result >= '0' && result <= '9') {
						columns = static_cast<uint16_t>(10 * columns + result - '0');
						result = rd(tty);
					}

					if (result != 'R')
						break;

					cursor_pos.set_x(static_cast<uint16_t>(columns - 1));
					cursor_pos.set_y(static_cast<uint16_t>(rows - 1));
					break;
				} while (true);

				do {
					result = tcsetattr(tty, TCSANOW, &saved);
				} while (result == -1 && errno == EINTR);

				return cursor_pos;
#endif
			}
			return {0, 0};
		}

		void LAMBDACOMMON_API set_cursor_position(unsigned short x, unsigned short y, std::ostream &stream)
		{
#ifdef WIN_FRIENDLY
			if (is_tty(stream)) {
				COORD coord;
				coord.X = x;
				coord.Y = y;
				SetConsoleCursorPosition(get_term_handle(stream), coord);
				return;
			}
#endif
			if (_use_ansi_escape_codes)
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
			bool ok = true;
#ifdef  WIN_FRIENDLY
			if (HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE); h_out != INVALID_HANDLE_VALUE) {
				if (DWORD dw_mode = 0; GetConsoleMode(h_out, &dw_mode)) {
					dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
					if (SetConsoleMode(h_out, dw_mode))
						_use_ansi_escape_codes = true;
				}
			}
			if (!_use_ansi_escape_codes)
				ok = false;
#elif !defined(LAMBDA_WASM)
			_use_ansi_escape_codes = true;
#endif
			if (!use_utf8())
				ok = false;
			return ok;
		}

		bool LAMBDACOMMON_API use_utf8()
		{
#ifdef WIN_FRIENDLY
			if (SetConsoleCP(CP_UTF8) && SetConsoleOutputCP(CP_UTF8))
				_has_utf8 = true;
#else
			if (setlocale(LC_ALL, "en_US.UTF-8") != nullptr)
				_has_utf8 = true;
#endif
			return _has_utf8;
		}

		bool LAMBDACOMMON_API has_utf8()
		{
			return _has_utf8;
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
			if (TCHAR current_title[MAX_PATH]; GetConsoleTitle(current_title, MAX_PATH))
				return {current_title};
			else return "";
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
			if (_use_ansi_escape_codes)
				stream << ("\033]0;" + title + "\007");
			return true;
		}

		const Size2D_u16 get_size(const std::ostream &stream)
		{
			Size2D_u16 size{};
#ifdef WIN_FRIENDLY
			CONSOLE_SCREEN_BUFFER_INFO csbi;

			GetConsoleScreenBufferInfo(get_term_handle(stream), &csbi);
			size.set_width(static_cast<uint16_t>(csbi.srWindow.Right - csbi.srWindow.Left + 1));
			size.set_height(static_cast<uint16_t>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1));
#else
			struct winsize w{};
			ioctl(fileno(get_standard_stream(stream)), TIOCGWINSZ, &w);
			size.set_width(w.ws_col);
			size.set_height(w.ws_row);
#endif
			return size;
		}
	}
}

#undef WIN_FRIENDLY
