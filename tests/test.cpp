#include <lambdacommon/graphics/color.h>
#include <lambdacommon/system/system.h>
#include <lambdacommon/resources.h>
#include <lambdacommon/system/uri.h>
#include <lambdacommon/exceptions/exceptions.h>
#include <lambdacommon/maths/complex.h>
#include <lambdacommon/maths/geometry/geometry.h>
#include <functional>

using namespace lambdacommon;
using namespace uri;
using namespace lstring::stream;
using namespace terminal;
using namespace std;

const ResourceName BASE_RESOURCENAME{"tests", "value/path"};

bool test(const string &test_name, const std::function<bool()> &func)
{
	if (test_name.find('"') != string::npos) {
		cout << "TESTING ";
		bool state = false;
		for (char c : test_name) {
			if (c == '"') {
				if (state)
					cout << c << RESET;
				else
					cout << MAGENTA << c;
				state = !state;
			} else
				cout << c;
		}
		cout << RESET << "...\n  RESULT: ";
	} else
		cout << "TESTING " << test_name << "...\n  RESULT: ";
	time_t start = time::get_time_millis();
	bool result = func();
	time_t end = time::get_time_millis();
	if (result)
		cout << LIGHT_GREEN << "OK." << RESET << " (in " << to_string(end - start) << "ms)" << endl;
	else
		cout << LIGHT_RED << "FAILED." << RESET << " (in " << to_string(end - start) << "ms)" << endl;
	return result;
}

int main()
{
	time_t start = time::get_time_millis();
	setup();
	set_title("λcommon - tests");
	cout << "Starting lambdacommon-tests with" << CYAN << " lambdacommon" << RESET << " v"
		 << lambdacommon::get_version()
		 << " (Compiled with " << LAMBDACOMMON_VERSION_MAJOR << '.' << LAMBDACOMMON_VERSION_MINOR << '.'
		 << LAMBDACOMMON_VERSION_PATCH << ")" << endl;
	cout << endl;
	cout << "OS running: " << LIGHT_YELLOW << system::get_os_name() << RESET << " (kernel: "
		 << system::get_kernel_version()
		 << ", arch: " << system::get_processor_arch_str() << " [" + system::get_processor_arch_enum_str() << "])"
		 << endl;
	cout << endl;

	cout << "Computer DATA:" << endl;
	cout << " Computer Name: " << LIGHT_YELLOW << system::get_host_name() << RESET << endl;
	cout << " User Name: " << LIGHT_YELLOW << system::get_user_name() << RESET << endl;
	cout << " User directory: " << vector<TermFormatting>{LIGHT_BLUE, BOLD} << system::get_user_directory().to_string()
		 << RESET << endl;
	bool root = system::is_root();
	cout << " Is run as root: "
		 << (root ? vector<TermFormatting>{LIGHT_GREEN, BOLD} : vector<TermFormatting>{LIGHT_RED, BOLD})
		 << lstring::to_string(root) << RESET << endl;
	cout << " CPU: " << LIGHT_GREEN << system::get_cpu_name() << " (" << to_string(system::get_cpu_cores())
		 << " cores)" << RESET << endl;
	uint64_t total_mem = system::get_memory_total();
	uint64_t used_mem = system::get_memory_used();
	uint64_t available_mem = system::get_memory_available();
	cout << " OS Physical Memory: " << LIGHT_GREEN << to_string((total_mem / 1048576)) << "MB ("
		 << to_string((total_mem / 1073741824.0)) << "GB)" << RESET << endl;
	cout << " OS Available RAM: " << LIGHT_GREEN << to_string((available_mem / 1048576)) << "MB ("
		 << to_string((available_mem / 1073741824.0)) << "GB)" << RESET << endl;
	cout << " OS RAM used: " << LIGHT_GREEN << to_string((used_mem / 1048576)) << "MB ("
		 << to_string((used_mem / 1073741824.0)) << "GB)" << RESET << endl;
	cout << endl;

	string expected;

	uint32_t tests_count = 0;
	uint32_t tests_passed = 0;

	cout << "===== STRING SECTION =====" << endl;
	tests_count++;
	if (test("lstring::equals_ignore_case((const char) 'w', (const char) 'W')", []() {
		return lstring::equals_ignore_case('w', 'W');
	}))
		tests_passed++;

	tests_count++;
	if (test("lstring::equals(const std::string &, const std::string &)", []() {
		return !lstring::equals("LambdAurora", "lAmbdaUrOrA");
	}))
		tests_passed++;

	tests_count++;
	if (test("lstring::equals_ignore_case(const std::string &, const std::string &)", []() {
		return lstring::equals_ignore_case("LambdAurora", "lAmbdaUrOrA");
	}))
		tests_passed++;

	cout << "Tests results: " + to_string(tests_passed) << '/' << to_string(tests_count) << endl;
	if (tests_passed != tests_count)
		return 1;

	cout << "===== FILESYSTEM SECTION =====" << endl;
	tests_count = 0;
	tests_passed = 0;

	tests_count++;
	if (test("FilePath::exists() with relative path \"404_non_existent\"", []() {
		fs::FilePath path{"404_non_existent"};
		return !path.exists();
	}))
		tests_passed++;

	fs::FilePath here_path = fs::get_current_working_directory();
	tests_count++;
	if (test("FilePath::exists() with absolute path \"" + here_path.to_string() + "\"", [here_path]() {
		return here_path.exists();
	}))
		tests_passed++;

	tests_count++;
	if (test("FilePath::is_directory() with absolute path \"" + here_path.to_string() + "\"", [here_path]() {
		return here_path.is_directory();
	}))
		tests_passed++;

	tests_count++;
	if (test("FilePath::operator/(const std::string&) with FilePath object \"" + here_path.to_string() +
			 R"(" and string "notFoundDir")", [here_path]() {
		auto h404re_path = here_path / "notFoundDir";
		return lstring::equals(h404re_path.get_filename(), "notFoundDir");
	}))
		tests_passed++;

	cout << "Tests results: " + to_string(tests_passed) << '/' << to_string(tests_count) << endl;
	if (tests_passed != tests_count)
		return 1;

	cout << "===== URI SECTION =====" << endl;
	tests_count = 0;
	tests_passed = 0;

	auto uri = URI("https", "user", "pwd", {"something.com"}, {"test", "file_or_folder"}, {{"foo",               "bar"},
																						   {"queryWithoutValue", ""},
																						   {"invalid query",     "but fixed"}},
				   "bar");
	string uri_to_string = uri.to_string();
	expected = "https://user:pwd@something.com/test/file_or_folder?foo=bar&queryWithoutValue&invalid%20query=but%20fixed#bar";
	tests_count++;
	if (test("URI.to_string(): expected: \"" + expected + "\" got: \"" + uri_to_string + "\"",
			 [uri_to_string, expected]() {
				 return lstring::equals(uri_to_string, expected);
			 }))
		tests_passed++;

	cout << "TESTING URI::from_file_path(FILESYSTEM::FILEPATH) WITH \"" << here_path.to_string(fs::COMMON)
		 << "\"...\nRESULT: ";
	auto file_uri = uri::from_file_path(here_path);
	expected = ("file://" + here_path.to_string(fs::COMMON));
	uri_to_string = file_uri.to_string();
	tests_count++;
	if (test("URI.from_file_path(const fs::FilePath&) (with \"" + here_path.to_string() + "\"): expected: \"" +
			 expected +
			 "\" got: \"" + uri_to_string + "\"", [uri_to_string, expected]() {
		return lstring::equals(uri_to_string, expected);
	}))
		tests_passed++;

	expected = "https://www.youtube.com/channel/UC2i7nj6wnh1Z2GQwvFeeKoA";
	cout << "TESTING URI::from_string(STD::STRING) WITH \"" << expected << "\"...\nRESULT: ";
	try {
		uri = uri::from_string("https://www.youtube.com/channel/UC2i7nj6wnh1Z2GQwvFeeKoA");
		uri_to_string = uri.to_string();
		if (lstring::equals(uri_to_string, expected))
			cout << LIGHT_GREEN << "OK. (" << uri_to_string << ")" << RESET << endl;
		else {
			cout << LIGHT_RED << "FAILED. (" << uri_to_string << ", expected: " + expected + ")"
				 << RESET << endl;
			return 1;
		}
	}
	catch (ParseException &e) {
		cout << LIGHT_RED << "FAILED. (ParseException{\"" << e.what()
			 << "\"}, expected: " + expected + ")"
			 << RESET << endl;
		return 1;
	}

	cout << "Tests results: " + to_string(tests_passed) << '/' << to_string(tests_count) << endl;
	if (tests_passed != tests_count)
		return 1;

	cout << "===== MATHS SECTION =====" << endl;
	tests_count = 0;
	tests_passed = 0;

	tests_count++;
	if (test("maths::abs((int) 42)", []() {
		return maths::abs((int) 42) == 42;
	}))
		tests_passed++;

	tests_count++;
	if (test("maths::abs((float) -64.0f)", []() {
		return maths::abs(-64.f) == 64.f;
	}))
		tests_passed++;

	tests_count++;
	if (test("maths::min(42, 64)", []() {
		return maths::min(42, 64) == 42;
	}))
		tests_passed++;

	tests_count++;
	if (test("maths::max(42.0f, 64.0f)", []() {
		return maths::max(42.f, 64.f) == 64.f;
	}))
		tests_passed++;

	tests_count++;
	if (test("maths::min({-5.0, 2.0, 42.0, -56.0, 64.0})", []() {
		return maths::min({-5.0, 2.0, 42.0, -56.0, 64.0}) == -56.0;
	}))
		tests_passed++;

	tests_count++;
	if (test("maths::max({-5, 2, 42, -56, 64})", []() {
		return maths::max({-5, 2, 42, -56, 64}) == 64;
	}))
		tests_passed++;

	tests_count++;
	if (test("maths::clamp(128, 0, 255)", []() {
		return maths::clamp(128, 0, 255) == 128;
	}))
		tests_passed++;

	tests_count++;
	if (test("maths::clamp(32.f, 0.f, 1.f)", []() {
		return maths::clamp(32.f, 0.f, 1.f) == 1.f;
	}))
		tests_passed++;

	cout << "Tests results: " + to_string(tests_passed) << '/' << to_string(tests_count) << endl;
	if (tests_passed != tests_count)
		return 1;

	cout << "===== SIZE SECTION =====" << endl;
	tests_count = 0;
	tests_passed = 0;

	tests_count++;
	if (test("Size3D::to_string() with {256, 128, 64}", []() {
		Size3D_u32 dimension{256, 128, 64};
		return lstring::equals(dimension.to_string(), R"({"width":256,"height":128,"depth":64})");
	}))
		tests_passed++;

	tests_count++;
	if (test("Size2D::operator*(T n) with {16, 32} ; 2", []() {
		Size2D_u32 dimension{16, 32};
		auto &[width, height] = (dimension * 2); // C++17 feature, it is nice!
		return width == 32 && height == 64;
	}))
		tests_passed++;

	tests_count++;
	if (test("Size3D::operator-(Size2D<T> dimension2D) with {2048;1024;16} ; {2032;1008}", []() {
		Size3D_u32 dimension{2048, 1024, 16};
		Size2D_u32 dimension2d{2032, 1008};
		auto &[width, height, depth] = dimension - dimension2d; // C++17 feature, it is nice!
		return width == 16 && height == 16 && depth == 16;
	}))
		tests_passed++;

	tests_count++;
	if (test("Size3D::is_null() with {0;0;0}", []() {
		Size3D_u32 dim{0, 0, 0};
		return dim.is_null();
	}))
		tests_passed++;

	cout << "Tests results: " + to_string(tests_passed) << '/' << to_string(tests_count) << endl;
	if (tests_passed != tests_count)
		return 1;

	cout << "===== ResourceNames SECTION =====" << endl;
	tests_count = 0;
	tests_passed = 0;

	tests_count++;
	if (test("RESOURCENAME(\"tests:value/path\")", []() {
		ResourceName base{"tests:value/path"};
		return base.get_domain() == "tests" && base.get_name() == "value/path";
	}))
		tests_passed++;

	tests_count++;
	if (test(R"(ResourceName::operator!=({"tests:value/OwO"}) with object {"tests:value/path"}, expect: true)", []() {
		auto base = BASE_RESOURCENAME;
		ResourceName owo{"tests:value/OwO"};
		return base != owo;
	}))
		tests_passed++;

	tests_count++;
	if (test(R"(ResourceName::operator=({"tests:value/OwO"}) with object {"tests:value/path"})", []() {
		auto new_res = BASE_RESOURCENAME;
		ResourceName owo{"tests", "value/OwO"};
		new_res = owo;
		return new_res.get_name() == "value/OwO";
	}))
		tests_passed++;

	tests_count++;
	if (test(R"(ResourceName::operator/ with base object {"tests:value/path"} and argument "owo")", []() {
		auto base = BASE_RESOURCENAME;
		auto new_res = base / "owo";
		return new_res.get_name() == "value/path/owo";
	}))
		tests_passed++;

	cout << "Tests results: " + to_string(tests_passed) << '/' << to_string(tests_count) << endl;
	if (tests_passed != tests_count)
		return 1;

	cout << "===== Color SECTION =====" << endl;
	tests_count = 0;
	tests_passed = 0;

	tests_count++;
	if (test("color::from_hex(0xCE0031AA) => rgba(206, 0, 49, 170)", []() {
		return color::from_hex(0xCE0031AA).to_string(false) == "rgba(206, 0, 49, 170)";
	}))
		tests_passed++;

	cout << "Tests results: " + to_string(tests_passed) << '/' << to_string(tests_count) << endl;
	if (tests_passed != tests_count)
		return 1;

	time_t end = time::get_time_millis();
	cout << endl << "Tests execution time: " << to_string(end - start) << "ms" << endl;

	return 0;
}
