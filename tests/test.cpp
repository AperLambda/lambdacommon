#include <lambdacommon/test.h>
#include <lambdacommon/graphics/color.h>
#include <lambdacommon/system/system.h>
#include <lambdacommon/resources.h>
#include <lambdacommon/system/uri.h>
#include <lambdacommon/exceptions/exceptions.h>
#include <lambdacommon/maths.h>
#include <lambdacommon/maths/geometry/geometry.h>
#include <functional>
#include <fstream>

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

LC_TEST_SECTION(String)
{
    LC_TEST(lstring_equals_ignore_case_char, "lstring::equals_ignore_case(const char a, const char b)")
    {
        REQUIRE(lstring::equals_ignore_case('a', 'a'));
        REQUIRE(lstring::equals_ignore_case('w', 'W'));
    }

    LC_TEST(lstring_equals, "lstring::equals(const std::string &a, const std::string &b)")
    {
        REQUIRE(lstring::equals("LambdAurora", "LambdAurora"));
        REQUIRE(!lstring::equals("lambdacommon", "LambdAcommOn"));
    }

    LC_TEST(lstring_equals_ignore_case, "lstring::equals_ignore_case(const std::string &a, const std::string &b)")
    {
        REQUIRE(lstring::equals_ignore_case("owo", "owo"));
        REQUIRE(lstring::equals_ignore_case("lambdacommon", "LambdaCommon"));
    }

    LC_TEST(lstring_to_lower, "lstring::to_lower_case(const std::string &input)")
    {
        REQUIRE(lstring::to_lower_case("OwO") == "owo");
        REQUIRE(lstring::to_lower_case("owo") == "owo");
    }

    LC_TEST(lstring_to_upper, "lstring::to_upper_case(const std::string &input)")
    {
        REQUIRE(lstring::to_upper_case("OwO") == "OWO");
        REQUIRE(lstring::to_upper_case("owo") == "OWO");
    }
}

LC_TEST_SECTION(FileSystem)
{
    LC_TEST(fs_fp_exists, "path::exists()")
    {
        REQUIRE(fs::current_path().exists());
        REQUIRE(!fs::path("404_non_existent").exists());
    }

    LC_TEST(fs_fp_is_dir, "path::is_directory()")
    {
        REQUIRE(fs::current_path().is_directory());
    }

    LC_TEST(fs_fp_op, "path / operator")
    {
        REQUIRE((fs::current_path() / "not_found_dir").get_filename().to_string() == "not_found_dir");
    }
}

LC_TEST_SECTION(URI)
{
    LC_TEST(uri_to_string, "URI::to_string()")
    {
        REQUIRE(URI("https",
                    "user", "pwd",
                    {"something.com"},
                    "test/file_or_folder",
                    {{"foo",               "bar"},
                     {"invalid query",     "but fixed"},
                     {"queryWithoutValue", ""}},
                    "bar").to_string() == u8"https://user:pwd@something.com/test/file_or_folder?foo=bar&invalid%20query=but%20fixed&queryWithoutValue#bar");
    }

    LC_TEST(uri_from_file_path, "URI::from_file_path(const fs::path &)")
    {
        REQUIRE(uri::from_file_path(std::move(fs::current_path())).to_string() == u8"file://" + fs::current_path().to_generic_string());
    }

    LC_TEST(uri_parsing, "URI parsing")
    {
        auto uri_string = u8"https://www.youtube.com/channel/UC2i7nj6wnh1Z2GQwvFeeKoA";
        REQUIRE(uri::from_string(uri_string).to_string() == uri_string);
    }
}

LC_TEST_SECTION(Maths)
{
    LC_TEST(maths_abs, "maths::abs(N a)")
    {
        REQUIRE(maths::abs((int) 42) == 42);
        REQUIRE(maths::abs(-64.f) == 64.f);
        REQUIRE(maths::abs(256.0) == 256.0);
    }

    LC_TEST(maths_min, "maths::min(N...)")
    {
        REQUIRE(maths::min(5, 2) == 2);
        REQUIRE(maths::min(0.957f, 0.998f) == 0.957f);
        REQUIRE(maths::min({-5.0, 2.0, 42.0, -56.0, 64.0}) == -56.0);
    }

    LC_TEST(maths_max, "maths::max(N...)")
    {
        REQUIRE(maths::max(5, 2) == 5);
        REQUIRE(maths::max(0.957f, 0.998f) == 0.998f);
        REQUIRE(maths::max({-5.0, 2.0, 42.0, -56.0, 64.0}) == 64.0);
    }

    LC_TEST(maths_clamp, "maths::clamp(N input, N min, N max)")
    {
        REQUIRE(maths::clamp(128, 0, 255) == 128);
        REQUIRE(maths::clamp(32.f, 0.f, 1.f) == 1.f);
    }
}

LC_TEST_SECTION(Size)
{
    LC_TEST(size2d_tostring, "Size2D::to_string()")
    {
        REQUIRE(Size2D_i16(-5, 6).to_string() == R"({"width":-5,"height":6})");
        REQUIRE(Size2D_u32(32, 2048).to_string() == R"({"width":32,"height":2048})");
    }

    LC_TEST(size3d_tostring, "Size3D::to_string()")
    {
        REQUIRE(Size3D_i16(-5, 6, -90).to_string() == R"({"width":-5,"height":6,"depth":-90})");
        REQUIRE(Size3D_u32(256, 128, 64).to_string() == R"({"width":256,"height":128,"depth":64})");
    }

    LC_TEST(sizes_op, "Size operators")
    {
        REQUIRE((Size2D_i32(5, 9) + Size2D_i32(5, 1)) == Size2D_i32(10, 10));
        REQUIRE(Size2D_u32(16, 32) * 2 == Size2D_u32(32, 64));
        REQUIRE(Size2D_u16(16, 32) / 2 == Size2D_u16(8, 16));
        REQUIRE(Size3D_u32(2048, 1024, 16) - Size2D_u32(2032, 1008) == Size3D_u32(16, 16, 16));
    }

    LC_TEST(size_null, "Size::is_null")
    {
        REQUIRE(Size2D_u32(0, 0).is_null());
        REQUIRE(!Size2D_i32(-1, 0).is_null());
        REQUIRE(Size3D_u32(0, 0, 0).is_null());
    }
}

LC_TEST_SECTION(ResourceNames)
{
    LC_TEST(rsc_name, "ResourceName(const std::string &resource_name)")
    {
        REQUIRE(ResourceName("tests:value/path").to_string() == "tests:value/path");
    }

    LC_TEST(rsc_operators, "ResourceName operators")
    {
        REQUIRE(ResourceName("tests:value/path") != ResourceName("tests", "value/OwO"));
        REQUIRE(ResourceName("tests:value/path") / "owo" == ResourceName("tests", "value/path/owo"));
    }
}

LC_TEST_SECTION(Color)
{
    LC_TEST(color_from_hex, "color::from_hex(uint64_t color, bool has_alpha)")
    {
        REQUIRE(color::from_hex(0xCE0031AA).to_string(false) == "rgba(206, 0, 49, 170)");
    }
}

int main()
{
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
    cout << " User directory: " << formats({LIGHT_BLUE, BOLD}) << system::get_user_directory().to_string() << RESET << endl;
    bool root = system::is_root();
    cout << " Is run as root: " << (root ? formats({LIGHT_GREEN, BOLD}) : formats({LIGHT_RED, BOLD})) << lstring::to_string(root) << RESET << endl;
    cout << " CPU: " << LIGHT_GREEN << system::get_cpu_name() << " (" << to_string(system::get_cpu_cores()) << " cores)" << RESET << endl;
    uint64_t total_mem = system::get_memory_total();
    uint64_t used_mem = system::get_memory_used();
    uint64_t available_mem = system::get_memory_available();
    cout << " OS Physical Memory: " << LIGHT_GREEN << to_string((total_mem / 1048576)) << "MB (" << to_string((total_mem / 1073741824.0)) << "GB)" << RESET << endl;
    cout << " OS Available RAM: " << LIGHT_GREEN << to_string((available_mem / 1048576)) << "MB (" << to_string((available_mem / 1073741824.0)) << "GB)" << RESET << endl;
    cout << " OS RAM used: " << LIGHT_GREEN << to_string((used_mem / 1048576)) << "MB (" << to_string((used_mem / 1073741824.0)) << "GB)" << RESET << endl;
    cout << endl;

    return lambdatest::LambdaTest::INSTANCE.launch();
}
