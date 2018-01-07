/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/system/os.h"

#ifdef __arm__
#  define LAMBDA_ARM
#elif _ARM || _M_ARM || __arm
#  define LAMBDA_ARM
#endif

using namespace std;

namespace lambdacommon
{
    namespace system
    {
        OS os::getOS()
        {
#ifdef LAMBDA_WINDOWS
#ifdef LAMBDA_WIN64
            return OS::WINDOWS_64;
#else
            return OS::WINDOWS_32;
#endif
#elif defined(LAMBDA_MAC_OSX)
            return OS::MAC_OSX;
#elif defined(__linux__)
            return OS::LINUX;
#elif defined(LAMBDA_FREEBSD)
            return OS::FREEBSD;
#elif defined(LAMBDA_CYGWIN)
            return OS::CYGWIN;
#else
            return OS::OS_UNKNOWN;
#endif
        }

        string os::getOSName(OS os)
        {
            string osName = string();
            switch (os)
            {
                case WINDOWS_32:
                    osName = "Windows 32-bits";
                    break;
                case WINDOWS_64:
                    osName = "Windows 64-bits";
                    break;
                case MAC_OSX:
                    osName = "Mac OSX";
                    break;
                case LINUX:
                    osName = "Linux";
                    break;
                case FREEBSD:
                    osName = "FreeBSD";
                    break;
                case NETBSD:
                    osName = "NetBSD";
                    break;
                case OPENBSD:
                    osName = "OpenBSD";
                    break;
                case CYGWIN:
                    osName = "Cygwin";
                    break;
                case OS_UNKNOWN:
                    osName = "Unknown";
                    break;
            }
            return osName;
        }
    }
}