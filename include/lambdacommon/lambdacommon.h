/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_LIBRARY_H
#define LAMBDACOMMON_LIBRARY_H

#include "lambdacommon_exports.h"
#include <string>

#ifdef _WIN32
#   ifdef _WIN64
#       define LAMBDA_WIN64
#   else
#       define LAMBDA_WIN32
#   endif
#   define LAMBDA_WINDOWS
#elif __APPLE__ || __MACH__
#   define LAMBDA_MAC_OSX
#endif

#define LAMBDACOMMON_VERSION_MAJOR 1
#define LAMBDACOMMON_VERSION_MINOR 5
#define LAMBDACOMMON_VERSION_BUILD 3

namespace lambdacommon
{
    extern std::string LAMBDACOMMON_API getVersion();
}

#endif
