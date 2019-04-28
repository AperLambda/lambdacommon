/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/lambdacommon.h"
#include "../include/lambdacommon/lstring.h"

namespace lambdacommon
{
    std::string LAMBDACOMMON_API get_version() {
        std::string result(LAMBDACOMMON_VERSION_STR);
        if (std::string type{LAMBDACOMMON_VERSION_TYPE}; type != "Release") {
            if (lstring::starts_with(type, "pre")) result = result + '-' + type; else result = type + result;
        }
        return result;
    }
}
