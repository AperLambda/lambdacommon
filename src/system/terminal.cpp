/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/system/terminal.h"

#if defined(LAMBDA_WINDOWS)

#include <io.h>
#include <windows.h>

#elif defined(__linux__) || defined(LAMBDA_MAC_OSX)
#include <unistd.h>
#endif

using namespace std;

namespace lambdacommon
{
    namespace terminal
    {
        /*
         * INTERNAL
         */
        inline
        FILE *get_standard_stream(const ostream &stream)
        {
            if (&stream == &cout)
                return stdout;
            else if ((&stream == &cerr) || (&stream == &clog))
                return stderr;

            return nullptr;
        }

        inline
        bool is_atty(const ostream &stream)
        {
            FILE *std_stream = get_standard_stream(stream);

#if defined(LAMBDA_MAC_OSX) || defined(__linux__)
            return ::isatty(fileno(std_stream));
#elif defined(LAMBDA_WINDOWS)
            return ::_isatty(_fileno(std_stream)) != 0;
#endif
        }

#if defined(LAMBDA_WINDOWS)

        inline void win_change_attributes(ostream &stream, int foreground, int background)
        {
            static WORD defaultAttributes = 0;

            // Get terminal handle
            HANDLE hTerminal = INVALID_HANDLE_VALUE;
            if (&stream == &cout)
                hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
            else if (&stream == &cerr)
                hTerminal = GetStdHandle(STD_ERROR_HANDLE);

            // Save default terminal attributes if it unsaved
            if (!defaultAttributes)
            {
                CONSOLE_SCREEN_BUFFER_INFO info;
                if (!GetConsoleScreenBufferInfo(hTerminal, &info))
                    return;
                defaultAttributes = info.wAttributes;
            }

            // Restore all default settings
            if (foreground == -1 && background == -1)
            {
                SetConsoleTextAttribute(hTerminal, defaultAttributes);
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

        ostream &operator<<(ostream &stream, TermFormatting termFormatting)
        {
            vector<TermFormatting> formats = {termFormatting};
            return stream << formats;
        }

        ostream &operator<<(ostream &stream, vector<TermFormatting> termFormatting)
        {
            if (is_atty(stream))
            {
#if defined(LAMBDA_WINDOWS)
                for (size_t i = 0; i < termFormatting.size(); i++)
                {
                    switch (termFormatting[i])
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
                            win_change_attributes(stream, 0x3, -1);
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
                            win_change_attributes(stream, 0x9, -1);
                            break;
                        case LIGHT_GREEN:
                            win_change_attributes(stream, 0xA, -1);
                            break;
                        case LIGHT_CYAN:
                            win_change_attributes(stream, 0xB, -1);
                            break;
                        case LIGHT_RED:
                            win_change_attributes(stream, 0xC, -1);
                            break;
                        case LIGHT_MAGENTA:
                            win_change_attributes(stream, 0xD, -1);
                            break;
                        case LIGHT_YELLOW:
                            win_change_attributes(stream, 0xE, -1);
                            break;
                        case WHITE:
                            win_change_attributes(stream, 0xF, -1);
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
                            win_change_attributes(stream, -1, (0x3 << 4));
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
#else
                string ansiSequence = "\033[";
                auto formattings = termFormatting.size();
                for (int i = 0; i < formattings; i++)
                {
                    string str = to_string(static_cast<int>(termFormatting[i]));
                    if (i != formattings - 1)
                        ansiSequence += (str + ";");
                    else
                        ansiSequence += str;
                }
                ansiSequence += "m";
                stream << ansiSequence;
#endif
            }
            return stream;
        }

        ostream &eraseCurrentLine(ostream &stream)
        {
#if defined(LAMBDA_WINDOWS)
#elif defined(__linux__) || defined(LAMBDA_MAC_OSX)
            stream << string("\33[2K");
#endif
            return stream;
        }

        ostream &carriageReturn(ostream &stream)
        {
            stream << string("\r");
            return stream;
        }

        void LAMBDACOMMON_API useUTF8()
        {
#ifdef LAMBDA_WINDOWS
            SetConsoleCP(65001);
            SetConsoleOutputCP(65001);
#endif
        }

        string LAMBDACOMMON_API getTerminalTitle()
        {
#ifdef LAMBDA_WINDOWS
            TCHAR currentTitle[MAX_PATH];
            if (GetConsoleTitle(currentTitle, MAX_PATH))
                return string(currentTitle);
            else
                return "";
#else
            return "";
#endif
        }

        bool LAMBDACOMMON_API setTerminalTitle(string title, ostream &stream)
        {
#ifdef LAMBDA_WINDOWS
            if (is_atty(stream))
                return (bool) SetConsoleTitle(TEXT(title.c_str()));
            else
                goto writeANSI;
#else
            goto writeANSI;
#endif
            writeANSI:
            stream << ("\033]0;" + title + "\007");
            return true;
        }

    }
}