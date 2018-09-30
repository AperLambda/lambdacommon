/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_SYSTEM_H
#define CLAMBDACOMMON_SYSTEM_H

#include "os.h"
#include "terminal.h"

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Enumeration of different Architectures of a CPU
 *
 * Enum of different CPU Architectures. Not complete.
 */
enum lc_SysArchitecture
{
	LC_ARCH_ARM,
	LC_ARCH_ARM64,
	LC_ARCH_I386,
	LC_ARCH_X86_64,
	LC_ARCH_UNKNOWN
};
typedef enum lc_SysArchitecture lc_SysArchitecture;

/*
 * Hardware
 */

/*!
 * Returns the name of the processor (CPU) as a string.
 * @return The CPU's name.
 */
const char *lc_sys_getProcessorName();

lc_SysArchitecture lc_sys_getProcessorArch();

const char *lc_sys_getProcessorArchEnumStr(lc_SysArchitecture arch);

const char *lc_sys_getProcessorArchStr();

/*!
 * Returns the count of the processor (CPU) cores as an int.
 * @return The CPU cores' count
 */
uint32_t lc_sys_getProcessorCores();

/*!
 * Returns the size of the physical memory (RAM) in bytes.
 * @return The physical memory in bytes.
 */
uint64_t lc_sys_getMemoryTotal();

/*!
 * Returns the size of the available physical memory (RAM) in bytes.
 * @return The available physical memory in bytes.
 */
uint64_t lc_sys_getMemoryAvailable();

/*!
 * Returns the size of the used physical memory (RAM) in bytes.
 * @return The used physical memory in bytes.
 */
uint64_t lc_sys_getMemoryUsed();

/*
 * Computer
 */

/*!
 * Returns the computer's name as a string.
 * @return The computer's name.
 */
const char *lc_sys_getHostName();

/*!
 * Returns the OS' name at runtime.
 * @return The OS' name at runtime.
 */
const char *lc_sys_getOSName();

/*!
 * Returns the Kernel' version at runtime.
 * @return The Kernel' version at runtime.
 */
const char *lc_sys_getKernelVersion();

/*
 * User
 */

/*!
 * Gets the user name of the user running the program.
 * @return The user name.
 */
const char *lc_sys_getUserName();

const char *lc_sys_getUserDirectoryStr();

/*
 * Others
 */

bool lc_sys_isProcessRunningAsRoot();

/*!
 * Opens the specified URI with the default application assigned to.
 * @param uri The URI to open.
 */
void lc_sys_openURI(const char *uri);

/*!
 * Sleeps the current thread for the specified time in milliseconds.
 * @param time Time to wait in milliseconds.
 */
void lc_sys_sleep(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_SYSTEM_H