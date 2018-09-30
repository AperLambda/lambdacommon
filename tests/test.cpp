#include <lambdacommon/graphics/color.h>
#include <lambdacommon/system/system.h>
#include <lambdacommon/resources.h>
#include <lambdacommon/system/uri.h>
#include <lambdacommon/exceptions/exceptions.h>
#include <lambdacommon/maths.h>
#include <functional>

using namespace lambdacommon;
using namespace uri;
using namespace terminal;
using namespace std;

const ResourceName BASE_RESOURCENAME{"tests", "value/path"};

bool test(const string &testName, std::function<bool()> func)
{
	if (testName.find('"') != string::npos)
	{
		cout << "TESTING ";
		bool state = false;
		for (char c : testName)
		{
			if (c == '"')
			{
				if (state)
					cout << c << RESET;
				else
					cout << MAGENTA << c;
				state = !state;
			}
			else
				cout << c;
		}
		cout << RESET << "...\n  RESULT: ";
	}
	else
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
	cout << " Computer Name: " << LIGHT_YELLOW << system::getHostName() << RESET << endl;
	cout << " User Name: " << LIGHT_YELLOW << system::getUserName() << RESET << endl;
	cout << " User directory: " << vector<TermFormatting>{LIGHT_BLUE, BOLD} << system::getUserDirectory().toString()
		 << RESET << endl;
	bool root = system::isProcessRunningAsRoot();
	cout << " Is run as root: "
		 << (root ? vector<TermFormatting>{LIGHT_GREEN, BOLD} : vector<TermFormatting>{LIGHT_RED, BOLD})
		 << lstring::to_string(root) << RESET << endl;
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
	string aString = "LambdAurora";
	string bString = "lAmbdaUrOrA";

	uint32_t testsCount = 0;
	uint32_t testsPassed = 0;

	cout << "===== STRING SECTION =====" << endl;
	testsCount++;
	if (test("lstring::equalsIgnoreCase((const char) 'w', (const char) 'W')", []()
	{
		return lstring::equalsIgnoreCase('w', 'W');
	}))
		testsPassed++;

	testsCount++;
	if (test("lstring::equals(const std::string &, const std::string &)", []()
	{
		return !lstring::equals("LambdAurora", "lAmbdaUrOrA");
	}))
		testsPassed++;

	testsCount++;
	if (test("lstring::equalsIgnoreCase(const std::string &, const std::string &)", []()
	{
		return lstring::equalsIgnoreCase("LambdAurora", "lAmbdaUrOrA");
	}))
		testsPassed++;

	cout << "Tests results: " + to_string(testsPassed) << '/' << to_string(testsCount) << endl;
	if (testsPassed != testsCount)
		return 1;

	cout << "===== FILESYSTEM SECTION =====" << endl;
	testsCount = 0;
	testsPassed = 0;

	testsCount++;
	if (test("FilePath::exists() with relative path \"404_non_existent\"", []()
	{
		fs::FilePath path{"404_non_existent"};
		return !path.exists();
	}))
		testsPassed++;

	fs::FilePath herePath = fs::getCurrentWorkingDirectory();
	testsCount++;
	if (test("FilePath::exists() with absolute path \"" + herePath.toString() + "\"", [herePath]()
	{
		return herePath.exists();
	}))
		testsPassed++;

	testsCount++;
	if (test("FilePath::isDirectory() with absolute path \"" + herePath.toString() + "\"", [herePath]()
	{
		return herePath.isDirectory();
	}))
		testsPassed++;

	testsCount++;
	if (test("FilePath::operator/(const std::string&) with FilePath object \"" + herePath.toString() +
			 "\" and string \"notFoundDir\"", [herePath]()
			 {
				 auto h404rePath = herePath / "notFoundDir";
				 return lstring::equals(h404rePath.getFileName(), "notFoundDir");
			 }))
		testsPassed++;

	cout << "Tests results: " + to_string(testsPassed) << '/' << to_string(testsCount) << endl;
	if (testsPassed != testsCount)
		return 1;

	cout << "===== URI SECTION =====" << endl;
	testsCount = 0;
	testsPassed = 0;

	cout << "TESTING URI (WRITER) ...\nRESULT: ";

	auto uri = URI("https", "user", "pwd", {"something.com"}, {"test", "file_or_folder"}, {{"foo",               "bar"},
																						   {"queryWithoutValue", ""},
																						   {"invalid query",     "but fixed"}},
				   "bar");
	string uri_toString = uri.toString();
	expected = "https://user:pwd@something.com/test/file_or_folder?foo=bar&queryWithoutValue&invalid%20query=but%20fixed#bar";
	testsCount++;
	if (test("URI.toString(): expected: \"" + expected + "\" got: \"" + uri_toString + "\"", [uri_toString, expected]()
	{
		return lstring::equals(uri_toString, expected);
	}))
		testsPassed++;

	cout << "TESTING URI::fromFilePath(FILESYSTEM::FILEPATH) WITH \"" << herePath.toString(fs::COMMON)
		 << "\"...\nRESULT: ";
	auto fileURL = uri::fromFilePath(herePath);
	expected = ("file://" + herePath.toString(fs::COMMON));
	uri_toString = fileURL.toString();
	testsCount++;
	if (test("URI.fromFilePath(const fs::FilePath&) (with \"" + herePath.toString() + "\"): expected: \"" + expected +
			 "\" got: \"" + uri_toString + "\"", [uri_toString, expected]()
			 {
				 return lstring::equals(uri_toString, expected);
			 }))
		testsPassed++;

	expected = "https://www.youtube.com/channel/UC2i7nj6wnh1Z2GQwvFeeKoA";
	cout << "TESTING URI::fromString(STD::STRING) WITH \"" << expected << "\"...\nRESULT: ";
	try
	{
		uri = uri::fromString("https://www.youtube.com/channel/UC2i7nj6wnh1Z2GQwvFeeKoA");
		uri_toString = uri.toString();
		if (lstring::equals(uri_toString, expected))
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

	cout << "Tests results: " + to_string(testsPassed) << '/' << to_string(testsCount) << endl;
	if (testsPassed != testsCount)
		return 1;

	cout << "===== MATHS SECTION =====" << endl;
	testsCount = 0;
	testsPassed = 0;

	testsCount++;
	if (test("maths::abs((int) 42)", []()
	{
		return maths::abs((int) 42) == 42;
	}))
		testsPassed++;

	testsCount++;
	if (test("maths::abs((float) -64.0f)", []()
	{
		return maths::abs(-64.f) == 64.f;
	}))
		testsPassed++;

	testsCount++;
	if (test("maths::min(42, 64)", []()
	{
		return maths::min(42, 64) == 42;
	}))
		testsPassed++;

	testsCount++;
	if (test("maths::max(42.0f, 64.0f)", []()
	{
		return maths::max(42.f, 64.f) == 64.f;
	}))
		testsPassed++;

	testsCount++;
	if (test("maths::min({-5.0, 2.0, 42.0, -56.0, 64.0})", []()
	{
		return maths::min({-5.0, 2.0, 42.0, -56.0, 64.0}) == -56.0;
	}))
		testsPassed++;

	testsCount++;
	if (test("maths::max({-5, 2, 42, -56, 64})", []()
	{
		return maths::max({-5, 2, 42, -56, 64}) == 64;
	}))
		testsPassed++;

	testsCount++;
	if (test("maths::clamp(128, 0, 255)", []()
	{
		return maths::clamp(128, 0, 255) == 128;
	}))
		testsPassed++;

	testsCount++;
	if (test("maths::clamp(32.f, 0.f, 1.f)", []()
	{
		return maths::clamp(32.f, 0.f, 1.f) == 1.f;
	}))
		testsPassed++;

	cout << "Tests results: " + to_string(testsPassed) << '/' << to_string(testsCount) << endl;
	if (testsPassed != testsCount)
		return 1;

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
	if (test(R"(ResourceName::operator!=({"tests:value/OwO"}) with object {"tests:value/path"}, expect: true)", []()
	{
		auto base = BASE_RESOURCENAME;
		ResourceName owo{"tests:value/OwO"};
		return base != owo;
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

	cout << "===== Color SECTION =====" << endl;
	testsCount = 0;
	testsPassed = 0;

	testsCount++;
	if (test("color::fromHex(0xCE0031AA) => rgba(206, 0, 49, 170)", []()
	{
		return color::fromHex(0xCE0031AA).toString(false) == "rgba(206, 0, 49, 170)";
	}))
		testsPassed++;

	cout << "Tests results: " + to_string(testsPassed) << '/' << to_string(testsCount) << endl;
	if (testsPassed != testsCount)
		return 1;

	return 0;
}