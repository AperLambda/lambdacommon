/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/clambdacommon/system/os.h"
#include <lambdacommon/system/os.h>

lc_sys_OS lc_os_get_os()
{
    return static_cast<lc_sys_OS>(lambdacommon::system::os::get_os());
}

const char *lc_os_get_os_name(lc_sys_OS os)
{
    return lambdacommon::system::os::get_os_name(static_cast<lambdacommon::system::OS>(os)).c_str();
}
