#include <lambdacommon/graphics/color.h>
#include <lambdacommon/system/system.h>
#include <lambdacommon/string.h>
#include <lambdacommon/connection/url.h>
#include <lambdacommon/exceptions/exceptions.h>

using namespace lambdacommon;
using namespace terminal;
using namespace std;

int main()
{
    useUTF8();
    setTerminalTitle("λcommon - tests");

    cout << "Starting λcommon test with " << CYAN << "λcommon" << RESET << " v" << lambdacommon::getVersion()
         << " (Compiled with "
         << LAMBDACOMMON_VERSION_MAJOR << LAMBDACOMMON_VERSION_MINOR << LAMBDACOMMON_VERSION_BUILD << ")" << endl;
    cout << endl;
    cout << "OS running: " << LIGHT_YELLOW << system::os::getOSName(system::os::getOS()) << RESET << " (arch: "
         << system::os::getArchName(system::os::getOSArch()) << ")" << endl;
    cout << endl;

    cout << "Computer DATA:" << endl;
    cout << " Computer Name: " << LIGHT_YELLOW << system::getComputerName() << RESET << endl;
    cout << " User Name: " << LIGHT_YELLOW << system::getUserName() << RESET << endl;
    cout << " User directory: " << vector<TermFormatting>{LIGHT_BLUE, BOLD} << system::getUserDirectory().toString()
         << RESET << endl;
    cout << " OS Physical Memory: " << LIGHT_GREEN << to_string((system::getPhysicalMemory() / (1024 * 1024))) << "MB"
         << RESET << endl;
    cout << " OS Available RAM: " << LIGHT_GREEN << to_string((system::getAvailablePhysicalMemory() / (1024 * 1024)))
         << "MB" << RESET << endl;
    cout << " OS used RAM: " << LIGHT_GREEN << to_string((system::getUsedPhysicalMemory() / (1024 * 1024))) << "MB"
         << RESET << endl;
    cout << endl;

    string expected;

    char aChar = 'w';
    char bChar = 'W';
    string aString = "wAperClem";
    string bString = "wApErClEm";

    cout << "TESTING LAMBDASTRING::EQUALSIGNORECASE(CONST CHAR, CONST CHAR)...\nRESULT: ";
    if (lambdastring::equalsIgnoreCase(aChar, bChar))
        cout << LIGHT_GREEN << "OK." << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED." << RESET << endl;
        return 1;
    }

    cout << "TESTING LAMBDASTRING::EQUALS(CONST STRING, CONST STRING)...\nRESULT: ";
    if (!lambdastring::equals(aString, bString))
        cout << LIGHT_GREEN << "OK." << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED." << RESET << endl;
        return 1;
    }

    cout << "TESTING LAMBDASTRING::EQUALSIGNORECASE(CONST STRING, CONST STRING)...\nRESULT: ";
    if (lambdastring::equalsIgnoreCase(aString, bString))
        cout << LIGHT_GREEN << "OK." << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED." << RESET << endl;
        return 1;
    }

    filesystem::FilePath path{"404_non_existent"};
    cout << "TESTING FILEPATH::EXISTS() WITH RELATIVE PATH \"" << path.toString() << "\"...\nRESULT: ";
    if (!path.exists())
        cout << LIGHT_GREEN << "OK." << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED." << RESET << endl;
        return 1;
    }

    filesystem::FilePath herePath = filesystem::getCurrentWorkingDirectory();
    cout << "TESTING FILEPATH::EXISTS() WITH ABSOLUTE PATH \"" << herePath.toString() << "\"...\nRESULT: ";
    if (herePath.exists())
        cout << LIGHT_GREEN << "OK." << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED." << RESET << endl;
        return 1;
    }

    cout << "TESTING FILEPATH::ISDIRECTORY() WITH ABSOLUTE PATH \"" << herePath.toString() << "\"...\nRESULT: ";
    if (herePath.isDirectory())
        cout << LIGHT_GREEN << "OK." << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED." << RESET << endl;
        return 1;
    }

    cout << "TESTING FILEPATH::OPERATOR/(STD::STRING) WITH \"" << herePath.toString() << "\"...\nRESULT: ";
    auto h404rePath = herePath / "notFoundDir";
    if (lambdastring::equals(h404rePath.getFileName(), "notFoundDir"))
        cout << LIGHT_GREEN << "OK. (" << h404rePath.toString() << ")" << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED. (" << h404rePath.toString() << ")" << RESET
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
        cout << LIGHT_GREEN << "OK. (" << url_toString << ")" << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED. (" << url_toString << ", expected: " + expected + ")"
             << RESET << endl;
        return 1;
    }

    cout << "TESTING URL::fromFilePath(FILESYSTEM::FILEPATH) WITH \"" << herePath.toString(filesystem::COMMON)
         << "\"...\nRESULT: ";
    auto fileURL = URL::fromFilePath(herePath);
    expected = ("file://" + herePath.toString(filesystem::COMMON));
    url_toString = fileURL.toString();
    if (lambdastring::equals(url_toString, expected))
        cout << LIGHT_GREEN << "OK. (" << url_toString << ")" << RESET << endl;
    else
    {
        cout << LIGHT_RED << "FAILED. (" << url_toString << ", expected: " + expected + ")"
             << RESET << endl;
        return 1;
    }

    expected = "https://www.youtube.com/watch?v=wh10k2LPZiI";
    cout << "TESTING URL::fromString(STD::STRING) WITH \"" << expected << "\"...\nRESULT: ";
    try
    {
        url = URL::fromString("https://www.youtube.com/watch?v=wh10k2LPZiI");
        url_toString = url.toString();
        if (lambdastring::equals(url_toString, expected))
            cout << LIGHT_GREEN << "OK. (" << url_toString << ")" << RESET << endl;
        else
        {
            cout << LIGHT_RED << "FAILED. (" << url_toString << ", expected: " + expected + ")"
                 << RESET << endl;
            return 1;
        }
    }
    catch (ParseException e)
    {
        cout << LIGHT_RED << "FAILED. (ParseException{\"" << e.what()
             << "\"}, expected: " + expected + ")"
             << RESET << endl;
        return 1;
    }
    return 0;
}
