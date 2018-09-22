#include <clambdacommon/system/system.h>
#include <clambdacommon/lstring.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printWithColor(const char *print, lcommon_TermFormatting formatting);

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
	lcommon_term_setup();
	lcommon_term_setTerminalTitle(u8"cλcommon - tests");

	printf("Starting clambdacommon-tests with ");
	printWithColor("lambdacommon", LC_TERM_CYAN);
	printf(" (C wrapper) v%s\n\n", lcommon_getVersion());

	printf("OS running: ");
	printWithColor(lcommon_sys_getOSName(), LC_TERM_LIGHT_YELLOW);
	printf("\n\n");

	printf("Computer DATA:\n");
	printf(" Computer Name: ");
	lcommon_term_setFormat(LC_TERM_LIGHT_YELLOW);
	printf("%s\n", lcommon_sys_getHostName());
	lcommon_term_setFormat(LC_TERM_RESET);

	printf(" User Name: ");
	lcommon_term_setFormat(LC_TERM_LIGHT_YELLOW);
	printf("%s\n", lcommon_sys_getUserName());
	lcommon_term_setFormat(LC_TERM_RESET);

	printf(" User Directory: ");
	lcommon_term_setFormats(2, LC_TERM_LIGHT_BLUE, LC_TERM_BOLD);
	printf("%s\n", lcommon_sys_getUserDirectoryStr());
	lcommon_term_setFormat(LC_TERM_RESET);

	printf(" Is run as root: ");
	lcommon_term_setFormat(LC_TERM_LIGHT_RED);
	printf("%s\n", lcommon_sys_isProcessRunningAsRoot() ? "true" : "false");
	lcommon_term_setFormat(LC_TERM_RESET);

	printf(" CPU: %s (%u cores)\n", lcommon_sys_getProcessorName(), lcommon_sys_getProcessorCores());

	uint64_t totalMem = lcommon_sys_getMemoryTotal();
	uint64_t usedMem = lcommon_sys_getMemoryUsed();
	uint64_t availableMem = lcommon_sys_getMemoryAvailable();
	printf(" OS Physical Memory: ");
	lcommon_term_setFormat(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n", (uint32_t) (totalMem / 1048576), totalMem / 1073741824.0);
	lcommon_term_setFormat(LC_TERM_RESET);
	printf(" OS Available Memory: ");
	lcommon_term_setFormat(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n", (uint32_t) (availableMem / 1048576), availableMem / 1073741824.0);
	lcommon_term_setFormat(LC_TERM_RESET);
	printf(" OS Used Memory: ");
	lcommon_term_setFormat(LC_TERM_LIGHT_GREEN);
	printf("%uMB (%lfGB)\n\n", (uint32_t) (usedMem / 1048576), usedMem / 1073741824.0);
	lcommon_term_setFormat(LC_TERM_RESET);

	uint32_t testsCount = 0;
	uint32_t testsPassed = 0;

	printf("===== MATHS SECTION =====\n");

	TEST("lcommon_maths_abs((int) 42)", testsCount, testsPassed, lcommon_maths_abs((int) 42) == 42);

	TEST("lcommon_maths_abs((float) -64.0f)", testsCount, testsPassed, lcommon_maths_abs(-64.f) == 64.f);

	TEST("lcommon_maths_min(42, 64)", testsCount, testsPassed, lcommon_maths_min(42, 64) == 42);

	TEST("lcommon_maths_clamp(128, 0, 255)", testsCount, testsPassed, lcommon_maths_clamp(128, 0, 255) == 128);

	TEST("lcommon_maths_clamp(32.f, 0.f, 1.f)", testsCount, testsPassed, lcommon_maths_clamp(32.f, 0.f, 1.f) == 1.f);

	printf("Tests results: %u/%u\n", testsPassed, testsCount);
	if (testsPassed != testsCount)
		return 1;

	return 0;
}

void printWithColor(const char *print, lcommon_TermFormatting formatting)
{
	lcommon_term_setFormat(formatting);
	printf("%s", print);
	lcommon_term_setFormat(LC_TERM_RESET);
}