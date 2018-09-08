#include <clambdacommon/system/system.h>
#include <clambdacommon/lstring.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printWithColor(const char *print, lcommon_TermFormatting formatting);

int main()
{
	lcommon_term_setup();
	lcommon_term_setTerminalTitle(u8"cλcommon - tests");

	printf("Starting clambdacommon-tests with ");
	printWithColor("lambdacommon", CYAN);
	printf(" (C wrapper) v%s\n\n", lcommon_getVersion());

	printf("OS running: ");
	printWithColor(lcommon_sys_getOSName(), LIGHT_YELLOW);
	printf("\n\n");

	printf("Computer DATA:\n");
	printf(" Computer Name: ");
	lcommon_term_setFormat(LIGHT_YELLOW);
	printf("%s\n", lcommon_sys_getHostName());
	lcommon_term_setFormat(RESET);

	printf(" User Name: ");
	lcommon_term_setFormat(LIGHT_YELLOW);
	printf("%s\n", lcommon_sys_getUserName());
	lcommon_term_setFormat(RESET);

	printf(" User Directory: ");
	lcommon_term_setFormats(2, LIGHT_BLUE, BOLD);
	printf("%s\n", lcommon_sys_getUserDirectoryStr());
	lcommon_term_setFormat(RESET);

	printf(" Is run as root: ");
	lcommon_term_setFormat(LIGHT_RED);
	printf("%s\n", lcommon_sys_isProcessRunningAsRoot() ? "true" : "false");
	lcommon_term_setFormat(RESET);

	printf(" CPU: %s (%u cores)\n", lcommon_sys_getProcessorName(), lcommon_sys_getProcessorCores());

	uint64_t totalMem = lcommon_sys_getMemoryTotal();
	uint64_t usedMem = lcommon_sys_getMemoryUsed();
	uint64_t availableMem = lcommon_sys_getMemoryAvailable();
	printf(" OS Physical Memory: ");
	lcommon_term_setFormat(LIGHT_GREEN);
	printf("%uMB (%lfGB)\n", (uint32_t) (totalMem / 1048576), totalMem / 1073741824.0);
	lcommon_term_setFormat(RESET);
	printf(" OS Available Memory: ");
	lcommon_term_setFormat(LIGHT_GREEN);
	printf("%uMB (%lfGB)\n", (uint32_t) (availableMem / 1048576), availableMem / 1073741824.0);
	lcommon_term_setFormat(RESET);
	printf(" OS Used Memory: ");
	lcommon_term_setFormat(LIGHT_GREEN);
	printf("%uMB (%lfGB)\n\n", (uint32_t) (usedMem / 1048576), usedMem / 1073741824.0);
	lcommon_term_setFormat(RESET);

	return 0;
}

void printWithColor(const char *print, lcommon_TermFormatting formatting)
{
	lcommon_term_setFormat(formatting);
	printf("%s", print);
	lcommon_term_setFormat(RESET);
}