/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/lambdacommon.h"

using namespace std;

namespace lambdacommon
{
    string getVersion()
    {
        return to_string(LAMBDACOMMON_VERSION_HIGH_MAJOR) + "." + to_string(LAMBDACOMMON_VERSION_MAJOR) + "." +
               to_string(LAMBDACOMMON_VERSION_MINOR) +
               (LAMBDACOMMON_VERSION_BUILD != 0 ? "." + to_string(LAMBDACOMMON_VERSION_BUILD) : "");
    }
}
