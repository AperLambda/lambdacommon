#include <lambdacommon/graphics/color.h>
#include <lambdacommon/system/terminal.h>
#include <lambdacommon/system/filesystem/filesystem.h>
#include <iostream>
#include <lambdacommon/string.h>

using namespace lambdacommon;
using namespace terminal;
using namespace std;

int main()
{
    cout << "Starting lambdacommon test with lambdacommon v" << lambdacommon::getVersion() << " (Compiled with "
         << LAMBDACOMMON_VERSION_MAJOR << LAMBDACOMMON_VERSION_MINOR << LAMBDACOMMON_VERSION_BUILD << ")" << endl;
    cout << endl;
    cout << "OS running: " << TermFormatting::LIGHT_YELLOW << os::getOSName(os::getOS()) << TermFormatting::RESET
         << " (arch: " << os::getArchName(os::getOSArch()) << ")" << endl;
    cout << endl;

    char aChar = 'w';
    char bChar = 'W';
    string aString = "wAperClem";
    string bString = "wApErClEm";

    cout << "TESTING LAMBDASTRING::EQUALSIGNORECASE(CONST CHAR, CONST CHAR)...\nRESULT: ";
    if (lambdastring::equalsIgnoreCase(aChar, bChar))
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING LAMBDASTRING::EQUALS(CONST STRING, CONST STRING)...\nRESULT: ";
    if (!lambdastring::equals(aString, bString))
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING LAMBDASTRING::EQUALSIGNORECASE(CONST STRING, CONST STRING)...\nRESULT: ";
    if (lambdastring::equalsIgnoreCase(aString, bString))
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    filesystem::Path path{"404_non_existent"};
    cout << "TESTING PATH::EXISTS() WITH RELATIVE PATH \"" << path.toString() << "\"...\nRESULT: ";
    if (!path.exists())
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    filesystem::Path herePath = filesystem::getCurrentWorkingDirectory();
    cout << "TESTING PATH::EXISTS() WITH ABSOLUTE PATH \"" << herePath.toString() << "\"...\nRESULT: ";
    if (herePath.exists())
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING PATH::ISDIRECTORY() WITH ABSOLUTE PATH \"" << herePath.toString() << "\"...\nRESULT: ";
    if (herePath.isDirectory())
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING PATH::OPERATOR/(STD::STRING) WITH \"" << herePath.toString() << "\"...\nRESULT: ";
    auto h404rePath = herePath / "notFoundDir";
    if (lambdastring::equals(h404rePath.getFileName(), "notFoundDir"))
        cout << TermFormatting::LIGHT_GREEN << "OK. (" << h404rePath.toString() << ")" << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED. (" << h404rePath.toString() << ")" << TermFormatting::RESET
             << endl;
        return 1;
    }

    return 0;
}
