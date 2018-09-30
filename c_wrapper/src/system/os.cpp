/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/clambdacommon/system/os.h"
#include <lambdacommon/system/os.h>

lc_sys_OS lc_os_getOS()
{
	return static_cast<lc_sys_OS>(lambdacommon::system::os::getOS());
}

const char *lc_os_getOSName(lc_sys_OS os)
{
	return lambdacommon::system::os::getOSName(static_cast<lambdacommon::system::OS>(os)).c_str();
}