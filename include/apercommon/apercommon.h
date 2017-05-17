/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef APERCOMMON_LIBRARY_H
#define APERCOMMON_LIBRARY_H

#include "apercommon_exports.h"
#include <string>

#ifdef _WIN32
#   ifdef _WIN64
#       define APER_WIN64
#   else
#       define APER_WIN32
#   endif
#   define APER_WINDOWS
#elif __APPLE__ || __MACH__
#   define APER_MAC_OSX
#endif

#define APERCOMMON_VERSION_MAJOR 1
#define APERCOMMON_VERSION_MINOR 2
#define APERCOMMON_VERSION_BUILD 3

namespace apercommon
{
    extern std::string APERCOMMON_API getVersion();
}

#endif
