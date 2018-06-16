#include <lambdacommon/graphics/color.h>
#include <lambdacommon/system/system.h>
#include <lambdacommon/resources.h>
#include <lambdacommon/system/uri.h>
#include <lambdacommon/exceptions/exceptions.h>

using namespace lambdacommon;
using namespace uri;
using namespace terminal;
using namespace std;

const ResourceName BASE_RESOURCENAME{"tests", "value/path"};

bool test(const string &testName, bool (*func)())
{
	cout << "TESTING " << testName << "...\n  RESULT: ";
	bool result = func();
	if (result)
		cout << LIGHT_GREEN << "OK." << RESET << endl;
	else
		cout << LIGHT_RED << "FAILED." << RESET << endl;
	return result;
}

int main()
{
	setup();
	setTerminalTitle("λcommon - tests");

	cout << "Starting lambdacommon-tests with" << CYAN << " lambdacommon" << RESET << " v" << lambdacommon::getVersion()
	     << " (Compiled with " << LAMBDACOMMON_VERSION_MAJOR << '.' << LAMBDACOMMON_VERSION_MINOR << '.'
	     << LAMBDACOMMON_VERSION_PATCH << ")" << endl;
	cout << endl;
	cout << "OS running: " << LIGHT_YELLOW << system::getOSName() << RESET << " (kernel: " << system::getKernelVersion()
	     << ", arch: " << system::getProcessorArchStr() << " [" + system::getProcessorArchEnumStr() << "])" << endl;
	cout << endl;

	cout << "Computer DATA:" << endl;
	cout << " Computer Name: " << LIGHT_YELLOW << system::getComputerName() << RESET << endl;
	cout << " User Name: " << LIGHT_YELLOW << system::getUserName() << RESET << endl;
	cout << " User directory: " << vector<TermFormatting>{LIGHT_BLUE, BOLD} << system::getUserDirectory().toString()
	     << RESET << endl;
	cout << " CPU: " << LIGHT_GREEN << system::getProcessorName() << " (" << to_string(system::getProcessorCores())
	     << " cores)" << RESET << endl;
	uint64_t totalMem = system::getMemoryTotal();
	uint64_t usedMem = system::getMemoryUsed();
	uint64_t availableMem = system::getMemoryAvailable();
	cout << " OS Physical Memory: " << LIGHT_GREEN << to_string((totalMem / 1048576)) << "MB ("
	     << to_string((totalMem / 1073741824.0)) << "GB)" << RESET << endl;
	cout << " OS Available RAM: " << LIGHT_GREEN << to_string((availableMem / 1048576)) << "MB ("
	     << to_string((availableMem / 1073741824.0)) << "GB)" << RESET << endl;
	cout << " OS RAM used: " << LIGHT_GREEN << to_string((usedMem / 1048576)) << "MB ("
	     << to_string((usedMem / 1073741824.0)) << "GB)" << RESET << endl;
	cout << endl;

	string expected;

	char aChar = 'w';
	char bChar = 'W';
	string aString = "wAperClem";
	string bString = "wApErClEm";

	uint32_t testsCount = 0;
	uint32_t testsPassed = 0;

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

	fs::FilePath path{"404_non_existent"};
	cout << "TESTING FILEPATH::EXISTS() WITH RELATIVE PATH \"" << path.toString() << "\"...\nRESULT: ";
	if (!path.exists())
		cout << LIGHT_GREEN << "OK." << RESET << endl;
	else
	{
		cout << LIGHT_RED << "FAILED." << RESET << endl;
		return 1;
	}

	fs::FilePath herePath = fs::getCurrentWorkingDirectory();
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

	cout << "TESTING URI (WRITER) ...\nRESULT: ";

	auto uri = URI("https", "user", "pwd", {"something.com"}, {"test", "file_or_folder"}, {{"foo",               "bar"},
	                                                                                       {"queryWithoutValue", ""},
	                                                                                       {"invalid query",     "but fixed"}},
	               "bar");
	string uri_toString = uri.toString();
	expected = "https://user:pwd@something.com/test/file_or_folder?foo=bar&queryWithoutValue&invalid%20query=but%20fixed#bar";
	if (lambdastring::equals(uri_toString, expected))
		cout << LIGHT_GREEN << "OK. (" << uri_toString << ")" << RESET << endl;
	else
	{
		cout << LIGHT_RED << "FAILED. (" << uri_toString << ", expected: " + expected + ")"
		     << RESET << endl;
		return 1;
	}

	cout << "TESTING URI::fromFilePath(FILESYSTEM::FILEPATH) WITH \"" << herePath.toString(fs::COMMON)
	     << "\"...\nRESULT: ";
	auto fileURL = uri::fromFilePath(herePath);
	expected = ("file://" + herePath.toString(fs::COMMON));
	uri_toString = fileURL.toString();
	if (lambdastring::equals(uri_toString, expected))
		cout << LIGHT_GREEN << "OK. (" << uri_toString << ")" << RESET << endl;
	else
	{
		cout << LIGHT_RED << "FAILED. (" << uri_toString << ", expected: " + expected + ")"
		     << RESET << endl;
		return 1;
	}

	expected = "https://www.youtube.com/watch?v=wh10k2LPZiI";
	cout << "TESTING URI::fromString(STD::STRING) WITH \"" << expected << "\"...\nRESULT: ";
	try
	{
		uri = uri::fromString("https://www.youtube.com/watch?v=wh10k2LPZiI");
		uri_toString = uri.toString();
		if (lambdastring::equals(uri_toString, expected))
			cout << LIGHT_GREEN << "OK. (" << uri_toString << ")" << RESET << endl;
		else
		{
			cout << LIGHT_RED << "FAILED. (" << uri_toString << ", expected: " + expected + ")"
			     << RESET << endl;
			return 1;
		}
	}
	catch (ParseException &e)
	{
		cout << LIGHT_RED << "FAILED. (ParseException{\"" << e.what()
		     << "\"}, expected: " + expected + ")"
		     << RESET << endl;
		return 1;
	}

	cout << "===== ResourceNames SECTION =====" << endl;
	testsCount = 0;
	testsPassed = 0;

	testsCount++;
	if (test("RESOURCENAME(\"tests:value/path\")", []()
		{
			ResourceName base{"tests:value/path"};
			return base.getDomain() == "tests" && base.getName() == "value/path";
		}))
		testsPassed++;

	testsCount++;
	if (test(R"(ResourceName::operator=({"tests:value/OwO"}) with object {"tests:value/path"})", []()
		{
			auto newRes = BASE_RESOURCENAME;
			ResourceName owo{"tests", "value/OwO"};
			newRes = owo;
			return newRes.getName() == "value/OwO";
		}))
		testsPassed++;

	testsCount++;
	if (test(R"(ResourceName::operator/ with base object {"tests:value/path"} and argument "owo")", []()
		{
			auto base = BASE_RESOURCENAME;
			auto newRes = base / "owo";
			return newRes.getName() == "value/path/owo";
		}))
		testsPassed++;

	cout << "Tests results: " + to_string(testsPassed) << '/' << to_string(testsCount) << endl;
	if (testsPassed != testsCount)
		return 1;

	return 0;
}