#include <clambdacommon/graphics/color.h>
#include <clambdacommon/system/system.h>
#include <clambdacommon/lstring.h>
#include <clambdacommon/resources.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_with_color(const char *print, lc_TermFormatting formatting);

#define TEST(NAME, COUNT, INDEX, FUNC) printf("TESTING %s...\n  RESULT: ", NAME); \
COUNT++; \
if (FUNC) { \
    INDEX++; \
    print_with_color("OK.\n", LC_TERM_LIGHT_GREEN); \
} else \
    print_with_color("FAILED.\n", LC_TERM_LIGHT_RED); \


int main()
{
	time_t start = lc_time_get_time_millis();
	lc_term_setup();
	lc_term_set_terminal_title(u8"cλcommon - tests");

	printf("Starting clambdacommon-tests with ");
	print_with_color("lambdacommon", LC_TERM_CYAN);
	printf(" (C wrapper) v%s\n\n", lc_get_version());

	printf("OS running: ");
	print_with_color(lc_sys_get_os_name(), LC_TERM_LIGHT_YELLOW);
	printf("\n\n");

	printf("Computer DATA:\n");
	printf(" Computer Name: ");
	lc_term_set_format(LC_TERM_LIGHT_YELLOW);
	printf("%s\n", lc_sys_get_host_name());
	lc_term_set_format(LC_TERM_RESET);

	printf(" User Name: ");
	lc_term_set_format(LC_TERM_LIGHT_YELLOW);
	printf("%s\n", lc_sys_get_user_name());
	lc_term_set_format(LC_TERM_RESET);

	printf(" User Directory: ");
	lc_term_set_formats(2, LC_TERM_LIGHT_BLUE, LC_TERM_BOLD);
	printf("%s\n", lc_sys_get_user_directory_str());
	lc_term_set_format(LC_TERM_RESET);

	printf(" Is run as root: ");
	lc_term_set_format(LC_TERM_LIGHT_RED);
	printf("%s\n", lc_sys_is_root() ? "true" : "false");
	lc_term_set_format(LC_TERM_RESET);

	printf(" CPU: %s (%u cores)\n", lc_sys_get_cpu_name(), lc_sys_get_cpu_cores());

	uint64_t total_mem = lc_sys_get_memory_total();
	uint64_t used_mem = lc_sys_get_memory_used();
	uint64_t available_mem = lc_sys_get_memory_available();
	printf(" OS Physical Memory: ");
	lc_term_set_format(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n", (uint32_t) (total_mem / 1048576), total_mem / 1073741824.0);
	lc_term_set_format(LC_TERM_RESET);
	printf(" OS Available Memory: ");
	lc_term_set_format(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n", (uint32_t) (available_mem / 1048576), available_mem / 1073741824.0);
	lc_term_set_format(LC_TERM_RESET);
	printf(" OS Used Memory: ");
	lc_term_set_format(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n\n", (uint32_t) (used_mem / 1048576), used_mem / 1073741824.0);
	lc_term_set_format(LC_TERM_RESET);

	uint32_t tests_count = 0;
	uint32_t tests_passed = 0;

	printf("===== MATHS SECTION =====\n");

	TEST("lc_maths_abs((int) 42)", tests_count, tests_passed, lc_maths_abs((int) 42) == 42);

	TEST("lc_maths_abs((float) -64.0f)", tests_count, tests_passed, lc_maths_abs(-64.f) == 64.f);

	TEST("lc_maths_min(42, 64)", tests_count, tests_passed, lc_maths_min(42, 64) == 42);

	TEST("lc_maths_clamp(128, 0, 255)", tests_count, tests_passed, lc_maths_clamp(128, 0, 255) == 128);

	TEST("lc_maths_clamp(32.f, 0.f, 1.f)", tests_count, tests_passed, lc_maths_clamp(32.f, 0.f, 1.f) == 1.f);

	printf("Tests results: %u/%u\n", tests_passed, tests_count);
	if (tests_passed != tests_count)
		return 1;

	printf("===== COLOR SECTION =====\n");
	tests_count = 0;
	tests_passed = 0;

	lcColor *colorFromHex = lc_color_from_hex(0xCE0031AA, true);
	TEST("lc_color_from_hex(0xCE0031AA, true) => rgba(206, 0, 49, 170)", tests_count, tests_passed,
		 lc_str_equals(lc_Color_to_string(colorFromHex, false), "rgba(206, 0, 49, 170)"));
	lc_delete_Color(colorFromHex);

	printf("Tests results: %u/%u\n", tests_passed, tests_count);
	if (tests_passed != tests_count)
		return 1;

	printf("\nTests execution time: %lldms", (long long) (lc_time_get_time_millis() - start));

	return 0;
}

void print_with_color(const char *print, lc_TermFormatting formatting)
{
	lc_term_set_format(formatting);
	printf("%s", print);
	lc_term_set_format(LC_TERM_RESET);
}
