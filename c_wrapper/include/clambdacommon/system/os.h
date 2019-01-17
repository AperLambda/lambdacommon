/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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
enum lc_sys_OS
{
	LC_OS_WINDOWS_32 = 1,
	LC_OS_WINDOWS_64 = 2,
	LC_OS_MAC_OSX = 3,
	LC_OS_LINUX = 5,
	LC_OS_FREEBSD = 6,
	LC_OS_NETBSD = 7,
	LC_OS_OPENBSD = 8,
	LC_OS_ANDROID = 100,
	LC_OS_CYGWIN = 11,
	LC_OS_OS_UNKNOWN = 0
};
typedef enum lc_sys_OS lc_sys_OS;

/*! @brief Gets the Operating system used.
 *
 * This function returns the Operating system used as an enum value of the enum OS.
 *
 * @return The OS used.
 */
lc_sys_OS lc_os_get_os();

const char *lc_os_get_os_name(lc_sys_OS os);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_OS_H
