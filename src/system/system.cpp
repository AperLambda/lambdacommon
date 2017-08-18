/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/system/system.h"
#include "../../include/lambdacommon/string.h"

#ifdef LAMBDA_WINDOWS

#  define INFO_BUFFER_SIZE 32767

#include <windows.h>
#include <ShlObj.h>

#else

#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <pwd.h>

#endif

using namespace std;

namespace lambdacommon
{
    namespace system
    {
        uint64_t LAMBDACOMMON_API getPhysicalMemory()
        {
#ifdef LAMBDA_WINDOWS
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatusEx(&statex);
            return statex.ullTotalPhys;
#else
#  ifdef _SC_PHYS_PAGES
            long pages = sysconf(_SC_PHYS_PAGES);
            long page_size = sysconf(_SC_PAGE_SIZE);
            return pages * page_size;
#  else
            uint64_t mem;
            size_t len = sizeof(mem);
            sysctlbyname("hw.memsize", &mem, &len, NULL, 0);
            return mem / sysconf(_SC_PAGE_SIZE);
#  endif
#endif
        }

        uint64_t LAMBDACOMMON_API getAvailablePhysicalMemory()
        {
#ifdef LAMBDA_WINDOWS
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatusEx(&statex);
            return statex.ullAvailPhys;
#else
            struct sysinfo memInfo;

            sysinfo(&memInfo);

            return (memInfo.freeram * memInfo.mem_unit);
#endif
        }

        uint64_t LAMBDACOMMON_API getUsedPhysicalMemory()
        {
#ifdef LAMBDA_WINDOWS
            return getPhysicalMemory() - getAvailablePhysicalMemory();
#else
            struct sysinfo memInfo;

            sysinfo(&memInfo);

            return ((memInfo.totalram - memInfo.freeram) * memInfo.mem_unit);
#endif
        }

        std::string LAMBDACOMMON_API getComputerName()
        {
#ifdef LAMBDA_WINDOWS
            char infoBuf[INFO_BUFFER_SIZE];
            DWORD bufCharCount = INFO_BUFFER_SIZE;

            if (!GetComputerName(infoBuf, &bufCharCount))
                return "";

            return string(infoBuf);
#else
            char hostname[HOST_NAME_MAX];
            gethostname(hostname, HOST_NAME_MAX);
            return string(hostname);
#endif
        }

        std::string LAMBDACOMMON_API getUserName()
        {
#ifdef LAMBDA_WINDOWS
            char infoBuf[INFO_BUFFER_SIZE];
            DWORD bufCharCount = INFO_BUFFER_SIZE;

            if (!GetUserName(infoBuf, &bufCharCount))
                return "";

            return string(infoBuf);
#else
            struct passwd *userInfo;
            userInfo = getpwuid(getuid());
            return string(userInfo->pw_name);
#endif
        }

        std::string LAMBDACOMMON_API getUserDirectoryStr()
        {
#ifdef LAMBDA_WINDOWS
            PWSTR szPath;
            if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Profile, KF_FLAG_NO_ALIAS, NULL, &szPath)))
                return "";
            return string(lambdastring::convertWStringToString(wstring(szPath)));
#else
            struct passwd *userInfo;
            userInfo = getpwuid(getuid());
            return string(userInfo->pw_dir);
#endif
        }

        filesystem::FilePath LAMBDACOMMON_API getUserDirectory()
        {
            return {getUserDirectoryStr()};
        }
    }
}

#ifdef LAMBDA_WINDOWS
#  undef INFO_BUFFER_SIZE
#endif