#include <lambdacommon/graphics/color.h>
#include <lambdacommon/system/terminal.h>
#include <lambdacommon/system/filesystem/filesystem.h>
#include <iostream>
#include <lambdacommon/string.h>
#include <lambdacommon/connection/url.h>
#include <lambdacommon/exceptions/exceptions.h>

using namespace lambdacommon;
using namespace terminal;
using namespace std;

int main()
{
#ifdef LAMBDA_WINDOWS
    string lambdacommon = "lambdacommon";
#else
    string lambdacommon = "λcommon";
#endif

    cout << "Starting " + lambdacommon + " test with " + lambdacommon + " v" << lambdacommon::getVersion()
         << " (Compiled with "
         << LAMBDACOMMON_VERSION_MAJOR << LAMBDACOMMON_VERSION_MINOR << LAMBDACOMMON_VERSION_BUILD << ")" << endl;
    cout << endl;
    cout << "OS running: " << TermFormatting::LIGHT_YELLOW << os::getOSName(os::getOS()) << TermFormatting::RESET
         << " (arch: " << os::getArchName(os::getOSArch()) << ")" << endl;
    cout << endl;

    string expected;

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

    filesystem::FilePath path{"404_non_existent"};
    cout << "TESTING FILEPATH::EXISTS() WITH RELATIVE PATH \"" << path.toString() << "\"...\nRESULT: ";
    if (!path.exists())
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    filesystem::FilePath herePath = filesystem::getCurrentWorkingDirectory();
    cout << "TESTING FILEPATH::EXISTS() WITH ABSOLUTE PATH \"" << herePath.toString() << "\"...\nRESULT: ";
    if (herePath.exists())
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING FILEPATH::ISDIRECTORY() WITH ABSOLUTE PATH \"" << herePath.toString() << "\"...\nRESULT: ";
    if (herePath.isDirectory())
        cout << TermFormatting::LIGHT_GREEN << "OK." << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED." << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING FILEPATH::OPERATOR/(STD::STRING) WITH \"" << herePath.toString() << "\"...\nRESULT: ";
    auto h404rePath = herePath / "notFoundDir";
    if (lambdastring::equals(h404rePath.getFileName(), "notFoundDir"))
        cout << TermFormatting::LIGHT_GREEN << "OK. (" << h404rePath.toString() << ")" << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED. (" << h404rePath.toString() << ")" << TermFormatting::RESET
             << endl;
        return 1;
    }

    cout
            << "TESTING URL (WRITER) ...\nRESULT: ";

    auto url = URL::URL("https", "user", "pwd", {"something.com"}, {"test", "file_or_folder"}, {{"foo",               "bar"},
                                                                                                {"queryWithoutValue", ""},
                                                                                                {"invalid query",     "but fixed"}},
                        "bar");
    string url_toString = url.toString();
    expected = "https://user:pwd@something.com/test/file_or_folder?foo=bar&queryWithoutValue&invalid%20query=but%20fixed#bar";
    if (lambdastring::equals(url_toString, expected))
        cout << TermFormatting::LIGHT_GREEN << "OK. (" << url_toString << ")" << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED. (" << url_toString << ", expected: " + expected + ")"
             << TermFormatting::RESET << endl;
        return 1;
    }

    cout << "TESTING URL::fromFilePath(FILESYSTEM::FILEPATH) WITH \"" << herePath.toString(filesystem::COMMON)
         << "\"...\nRESULT: ";
    auto fileURL = URL::fromFilePath(herePath);
    expected = ("file://" + herePath.toString(filesystem::COMMON));
    url_toString = fileURL.toString();
    if (lambdastring::equals(url_toString, expected))
        cout << TermFormatting::LIGHT_GREEN << "OK. (" << url_toString << ")" << TermFormatting::RESET << endl;
    else
    {
        cout << TermFormatting::LIGHT_RED << "FAILED. (" << url_toString << ", expected: " + expected + ")"
             << TermFormatting::RESET << endl;
        return 1;
    }

    expected = "https://www.youtube.com/watch?v=wh10k2LPZiI";
    cout << "TESTING URL::fromString(STD::STRING) WITH \"" << expected << "\"...\nRESULT: ";
    try
    {
        url = URL::fromString("https://www.youtube.com/watch?v=wh10k2LPZiI");
        url_toString = url.toString();
        if (lambdastring::equals(url_toString, expected))
            cout << TermFormatting::LIGHT_GREEN << "OK. (" << url_toString << ")" << TermFormatting::RESET << endl;
        else
        {
            cout << TermFormatting::LIGHT_RED << "FAILED. (" << url_toString << ", expected: " + expected + ")"
                 << TermFormatting::RESET << endl;
            return 1;
        }
    }
    catch (ParseException e)
    {
        cout << TermFormatting::LIGHT_RED << "FAILED. (ParseException{\"" << e.what()
             << "\"}, expected: " + expected + ")"
             << TermFormatting::RESET << endl;
        return 1;
    }
    return 0;
}
