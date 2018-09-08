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
enum lcommon_SysArchitecture
{
	ARM,
	ARM64,
	I386,
	X86_64,
	UNKNOWN
};
typedef enum lcommon_SysArchitecture lcommon_SysArchitecture;

/*
 * Hardware
 */

/*!
 * Returns the name of the processor (CPU) as a string.
 * @return The CPU's name.
 */
const char *lcommon_sys_getProcessorName();

lcommon_SysArchitecture lcommon_sys_getProcessorArch();

const char *lcommon_sys_getProcessorArchEnumStr(lcommon_SysArchitecture arch);

const char *lcommon_sys_getProcessorArchStr();

/*!
 * Returns the count of the processor (CPU) cores as an int.
 * @return The CPU cores' count
 */
uint32_t lcommon_sys_getProcessorCores();

/*!
 * Returns the size of the physical memory (RAM) in bytes.
 * @return The physical memory in bytes.
 */
uint64_t lcommon_sys_getMemoryTotal();

/*!
 * Returns the size of the available physical memory (RAM) in bytes.
 * @return The available physical memory in bytes.
 */
uint64_t lcommon_sys_getMemoryAvailable();

/*!
 * Returns the size of the used physical memory (RAM) in bytes.
 * @return The used physical memory in bytes.
 */
uint64_t lcommon_sys_getMemoryUsed();

/*
 * Computer
 */

/*!
 * Returns the computer's name as a string.
 * @return The computer's name.
 */
const char *lcommon_sys_getHostName();

/*!
 * Returns the OS' name at runtime.
 * @return The OS' name at runtime.
 */
const char *lcommon_sys_getOSName();

/*!
 * Returns the Kernel' version at runtime.
 * @return The Kernel' version at runtime.
 */
const char *lcommon_sys_getKernelVersion();

/*
 * User
 */

/*!
 * Gets the user name of the user running the program.
 * @return The user name.
 */
const char *lcommon_sys_getUserName();

const char *lcommon_sys_getUserDirectoryStr();

/*
 * Others
 */

bool lcommon_sys_isProcessRunningAsRoot();

/*!
 * Sleeps the current thread for the specified time in milliseconds.
 * @param time Time to wait in milliseconds.
 */
void lcommon_sys_sleep(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_SYSTEM_H