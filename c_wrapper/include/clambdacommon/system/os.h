/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_OS_H
#define CLAMBDACOMMON_OS_H

#include "../clambdacommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Enumeration of different Operating system.
 *
 * Enum of different Operating system. Not complete.
 */
enum lcommon_sys_OS
{
	WINDOWS_32 = 1,
	WINDOWS_64 = 2,
	MAC_OSX = 3,
	LINUX = 5,
	FREEBSD = 6,
	NETBSD = 7,
	OPENBSD = 8,
	CYGWIN = 11,
	OS_UNKNOWN = 0
};
typedef enum lcommon_sys_OS lcommon_sys_OS;

/*! @brief Gets the Operating system used.
 *
 * This function returns the Operating system used as an enum value of the enum OS.
 *
 * @return The OS used.
 */
lcommon_sys_OS lcommon_os_getOS();

const char *lcommon_os_getOSName(lcommon_sys_OS os);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_OS_H