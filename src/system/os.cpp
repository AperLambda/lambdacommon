/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/apercommon/system/os.h"

#ifdef __arm__
#define APER_ARM
#elif _ARM || _M_ARM || __arm
#define APER_ARM
#endif

using namespace std;

namespace apercommon
{
    OS os::getOS()
    {
#ifdef APER_WINDOWS
#ifdef APER_WIN64
        return OS::WINDOWS_64;
#else
        return OS::WINDOWS_32;
#endif
#elif APER_MAC_OSX
        return OS::MAC_OSX;
#elif __linux__
        return OS::LINUX;
#elif __FreeBSD__
        return OS::FREEBSD;
#else
        return OS::UNKNOWN;
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
            case UNKNOWN:
                osName = "Unknown";
                break;
        }
        return osName;
    }

    OSArchitecture os::getOSArch()
    {
#ifdef APER_ARM
        return OSArchitecture::ARM;
#elif __aarch64__
        return OSArchitecture::ARM64;
#elif i386 || __i386 || __i386__ || _M_IX86 || __X86__ || _X86_
        return OSArchitecture::I386;
#elif __amd64__ || __amd64 || __x86_64__ || __x86_64 || _M_X64 || _M_AMD64
        return OSArchitecture::X86_64;
#endif
    }

    std::string os::getArchName(OSArchitecture arch)
    {
        string archName;
        switch (arch)
        {
            case ARM:
                archName = "ARM";
            case ARM64:
                archName = "ARM 64bits";
            case I386:
                archName = "i386";
            case X86_64:
                archName = "x86_64";
        }
        return archName;
    }
}
