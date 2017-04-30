#include <apercommon/graphics/color.h>
#include <apercommon/system/terminal.h>
#include <iostream>
#include <apercommon/string.h>

using namespace apercommon;
using namespace terminal;
using namespace std;

int main()
{
    cout << "Starting AperCommon test with AperCommon v" << apercommon::getVersion() << endl;
    cout << endl;
    cout << "OS running: " << TermFormatting::LIGHT_YELLOW << os::getOSName(os::getOS()) << TermFormatting::RESET
         << " (arch: " << os::getArchName(os::getOSArch()) << ")" << endl;
    cout << endl;

    char aChar = 'w';
    char bChar = 'W';
    string aString = "wAperClem";
    string bString = "wApErClEm";

    cout << "TESTING APERSTRING::EQUALSIGNORECASE(CONST CHAR, CONST CHAR)..." << endl << "RESULT: ";
    if (aperstring::equalsIgnoreCase(aChar, bChar))
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING APERSTRING::EQUALS(CONST STRING, CONST STRING)..." << endl << "RESULT: ";
    if (!aperstring::equals(aString, bString))
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING APERSTRING::EQUALSIGNORECASE(CONST STRING, CONST STRING)..." << endl << "RESULT: ";
    if (aperstring::equalsIgnoreCase(aString, bString))
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }
    return 0;
}
