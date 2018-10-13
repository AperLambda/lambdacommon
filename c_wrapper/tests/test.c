#include <clambdacommon/graphics/color.h>
#include <clambdacommon/system/system.h>
#include <clambdacommon/lstring.h>
#include <clambdacommon/resources.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printWithColor(const char *print, lc_TermFormatting formatting);

#define TEST(NAME, COUNT, INDEX, FUNC) printf("TESTING %s...\n  RESULT: ", NAME); \
COUNT++; \
if (FUNC) \
{ \
	INDEX++; \
    printWithColor("OK.\n", LC_TERM_LIGHT_GREEN); \
} \
else \
	printWithColor("FAILED.\n", LC_TERM_LIGHT_RED); \

int main()
{
	time_t start = lc_time_getTimeMillis();
	lc_term_setup();
	lc_term_setTerminalTitle(u8"cλcommon - tests");

	printf("Starting clambdacommon-tests with ");
	printWithColor("lambdacommon", LC_TERM_CYAN);
	printf(" (C wrapper) v%s\n\n", lc_getVersion());

	printf("OS running: ");
	printWithColor(lc_sys_getOSName(), LC_TERM_LIGHT_YELLOW);
	printf("\n\n");

	printf("Computer DATA:\n");
	printf(" Computer Name: ");
	lc_term_setFormat(LC_TERM_LIGHT_YELLOW);
	printf("%s\n", lc_sys_getHostName());
	lc_term_setFormat(LC_TERM_RESET);

	printf(" User Name: ");
	lc_term_setFormat(LC_TERM_LIGHT_YELLOW);
	printf("%s\n", lc_sys_getUserName());
	lc_term_setFormat(LC_TERM_RESET);

	printf(" User Directory: ");
	lc_term_setFormats(2, LC_TERM_LIGHT_BLUE, LC_TERM_BOLD);
	printf("%s\n", lc_sys_getUserDirectoryStr());
	lc_term_setFormat(LC_TERM_RESET);

	printf(" Is run as root: ");
	lc_term_setFormat(LC_TERM_LIGHT_RED);
	printf("%s\n", lc_sys_isProcessRunningAsRoot() ? "true" : "false");
	lc_term_setFormat(LC_TERM_RESET);

	printf(" CPU: %s (%u cores)\n", lc_sys_getProcessorName(), lc_sys_getProcessorCores());

	uint64_t totalMem = lc_sys_getMemoryTotal();
	uint64_t usedMem = lc_sys_getMemoryUsed();
	uint64_t availableMem = lc_sys_getMemoryAvailable();
	printf(" OS Physical Memory: ");
	lc_term_setFormat(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n", (uint32_t) (totalMem / 1048576), totalMem / 1073741824.0);
	lc_term_setFormat(LC_TERM_RESET);
	printf(" OS Available Memory: ");
	lc_term_setFormat(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n", (uint32_t) (availableMem / 1048576), availableMem / 1073741824.0);
	lc_term_setFormat(LC_TERM_RESET);
	printf(" OS Used Memory: ");
	lc_term_setFormat(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n\n", (uint32_t) (usedMem / 1048576), usedMem / 1073741824.0);
	lc_term_setFormat(LC_TERM_RESET);

	uint32_t testsCount = 0;
	uint32_t testsPassed = 0;

	printf("===== MATHS SECTION =====\n");

	TEST("lc_maths_abs((int) 42)", testsCount, testsPassed, lc_maths_abs((int) 42) == 42);

	TEST("lc_maths_abs((float) -64.0f)", testsCount, testsPassed, lc_maths_abs(-64.f) == 64.f);

	TEST("lc_maths_min(42, 64)", testsCount, testsPassed, lc_maths_min(42, 64) == 42);

	TEST("lc_maths_clamp(128, 0, 255)", testsCount, testsPassed, lc_maths_clamp(128, 0, 255) == 128);

	TEST("lc_maths_clamp(32.f, 0.f, 1.f)", testsCount, testsPassed, lc_maths_clamp(32.f, 0.f, 1.f) == 1.f);

	printf("Tests results: %u/%u\n", testsPassed, testsCount);
	if (testsPassed != testsCount)
		return 1;

	printf("===== COLOR SECTION =====\n");
	testsCount = 0;
	testsPassed = 0;

	lcColor *colorFromHex = lc_color_fromHex(0xCE0031AA, true);
	TEST("lc_color_fromHex(0xCE0031AA, true) => rgba(206, 0, 49, 170)", testsCount, testsPassed, lc_str_equals(lc_Color_toString(colorFromHex, false), "rgba(206, 0, 49, 170)"));
	lc_deleteColor(colorFromHex);

	printf("Tests results: %u/%u\n", testsPassed, testsCount);
	if (testsPassed != testsCount)
		return 1;

	printf("\nTests execution time: %lldms", (long long) (lc_time_getTimeMillis() - start));

	return 0;
}

void printWithColor(const char *print, lc_TermFormatting formatting)
{
	lc_term_setFormat(formatting);
	printf("%s", print);
	lc_term_setFormat(LC_TERM_RESET);
}