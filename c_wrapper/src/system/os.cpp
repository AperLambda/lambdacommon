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

lcommon_sys_OS lcommon_os_getOS()
{
	return static_cast<lcommon_sys_OS>(lambdacommon::system::os::getOS());
}

const char *lcommon_os_getOSName(lcommon_sys_OS os)
{
	return lambdacommon::system::os::getOSName(static_cast<lambdacommon::system::OS>(os)).c_str();
}