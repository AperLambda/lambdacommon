/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/apercommon/apercommon.h"

using namespace std;

namespace apercommon
{
    string getVersion()
    {
        return to_string(APERCOMMON_VERSION_MAJOR) + "." + to_string(APERCOMMON_VERSION_MINOR) + "." +
               to_string(APERCOMMON_VERSION_BUILD);
    }
}
