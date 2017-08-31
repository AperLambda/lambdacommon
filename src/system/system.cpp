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
#include <sstream>

#ifdef LAMBDA_WINDOWS

#  define INFO_BUFFER_SIZE 32767

#include <intrin.h>
#include <ShlObj.h>
#include <VersionHelpers.h>

#else

#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <fstream>

#endif

using namespace std;

namespace lambdacommon
{
    namespace system
    {
        std::string osName = "";

#ifdef LAMBDA_WINDOWS

        string LAMBDACOMMON_API getProcessorName()
        {
            int cpuInfo[4] = {-1};
            char cpuBrandStr[0x40];
            __cpuid(cpuInfo, 0x80000000);
            unsigned int nExIds = static_cast<unsigned int>(cpuInfo[0]);

            memset(cpuBrandStr, 0, sizeof(cpuBrandStr));

            // Get the information associated with each extended ID.
            for (unsigned int i = 0x80000000; i <= nExIds; ++i)
            {
                __cpuid(cpuInfo, i);
                // Interpret CPU brand string.
                if (i == 0x80000002)
                    memcpy(cpuBrandStr, cpuInfo, sizeof(cpuInfo));
                else if (i == 0x80000003)
                    memcpy(cpuBrandStr + 16, cpuInfo, sizeof(cpuInfo));
                else if (i == 0x80000004)
                    memcpy(cpuBrandStr + 32, cpuInfo, sizeof(cpuInfo));
            }
            string cpuName{cpuBrandStr};
            return cpuName;
        }

        uint32_t LAMBDACOMMON_API getProcessorCores()
        {
            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            return sysinfo.dwNumberOfProcessors;
        }

        uint64_t LAMBDACOMMON_API getPhysicalMemory()
        {
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatusEx(&statex);
            return statex.ullTotalPhys;
        }

        uint64_t LAMBDACOMMON_API getAvailablePhysicalMemory()
        {
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatusEx(&statex);
            return statex.ullAvailPhys;
        }

        uint64_t LAMBDACOMMON_API getUsedPhysicalMemory()
        {
            return getPhysicalMemory() - getAvailablePhysicalMemory();
        }

        string LAMBDACOMMON_API getComputerName()
        {
            char infoBuf[INFO_BUFFER_SIZE];
            DWORD bufCharCount = INFO_BUFFER_SIZE;

            if (!GetComputerName(infoBuf, &bufCharCount))
                return "";

            return string(infoBuf);
        }

        string LAMBDACOMMON_API getOSName()
        {
            // Holy shit, what the fuck is this shitty code?
            // Fuck you Microsoft!
            string winName = "Windows ";

            if (IsWindowsServer())
                winName += "Server ";

            if (IsWindows10OrGreater())
                winName += "10";
            else if (IsWindows8Point1OrGreater())
                winName += "8.1";
            else if (IsWindows8OrGreater())
                winName += "8";
            else if (IsWindows7SP1OrGreater())
                winName += "7 SP1";
            else if (IsWindows7OrGreater())
                winName += "7";
            else if (IsWindowsVistaSP2OrGreater())
                winName += "Vista SP2";
            else if (IsWindowsVistaSP1OrGreater())
                winName += "Vista SP1";
            else if (IsWindowsVistaOrGreater())
                winName += "Vista";
            else if (IsWindowsXPOrGreater())
                winName += "XP";
            return winName;
        }

        string LAMBDACOMMON_API getKernelVersion()
        {
            OSVERSIONINFO vi;
            vi.dwOSVersionInfoSize = sizeof(vi);
            if (GetVersionEx(&vi) == 0) return "UNKNOWN";
            std::ostringstream str;
            str << vi.dwMajorVersion << "." << vi.dwMinorVersion << " (Build " << (vi.dwBuildNumber & 0xFFFF);
            if (vi.szCSDVersion[0]) str << ": " << vi.szCSDVersion;
            str << ")";
            return str.str();
        }

        string LAMBDACOMMON_API getUserName()
        {
            char infoBuf[INFO_BUFFER_SIZE];
            DWORD bufCharCount = INFO_BUFFER_SIZE;

            if (!GetUserName(infoBuf, &bufCharCount))
                return "";

            return string(infoBuf);
        }

        string LAMBDACOMMON_API getUserDirectoryStr()
        {
#  ifdef __MINGW32__
            TCHAR path[MAX_PATH];
            if (FAILED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, path)))
                return "";
            return string(path);
#  else // __MINGW32__
            PWSTR szPath;
            if (FAILED(SHGetKnownFolderPath(FOLDERID_Profile, KF_FLAG_NO_ALIAS, NULL, &szPath)))
                return "";
            return string(lambdastring::convertWStringToString(wstring(szPath)));
#  endif // __MINGW32__
        }

#else

        string LAMBDACOMMON_API getProcessorName()
        {
            ifstream cpuinfo;
            cpuinfo.open("/proc/cpuinfo", ios::in);
            string word;
            bool record = false;
            string cpu;
            if (cpuinfo.is_open())
            {
                while (cpuinfo >> word)
                {
                    if (word == "model" && cpuinfo >> word && word == "name" && cpuinfo >> word && cpuinfo >> word)
                    {
                        cpu = word;
                        record = true;
                    }
                    else if (record)
                    {
                        if (word == "stepping")
                            break;
                        else
                            cpu += (" " + word);
                    }
                }
                cpuinfo.close();
            }
            return cpu;
        }

        uint32_t LAMBDACOMMON_API getProcessorCores()
        {
            ifstream cpuinfo;
            cpuinfo.open("/proc/cpuinfo", ios::in);
            string word;
            uint32_t cpucount = 0;
            if (cpuinfo.is_open())
            {
                while (cpuinfo >> word)
                {
                    if (word == "processor")
                        cpucount++;
                }
                cpuinfo.close();
            }
            return cpucount;
        }

        uint64_t LAMBDACOMMON_API getPhysicalMemory()
        {
#  ifdef _SC_PHYS_PAGES
            long pages = sysconf(_SC_PHYS_PAGES);
            long page_size = sysconf(_SC_PAGE_SIZE);
            return static_cast<uint64_t>(pages * page_size);
#  else
            uint64_t mem;
            size_t len = sizeof(mem);
            sysctlbyname("hw.memsize", &mem, &len, NULL, 0);
            return mem / sysconf(_SC_PAGE_SIZE);
#  endif
        }

        uint64_t LAMBDACOMMON_API getAvailablePhysicalMemory()
        {
            struct sysinfo memInfo;

            sysinfo(&memInfo);

            return (memInfo.freeram * memInfo.mem_unit);
        }

        uint64_t LAMBDACOMMON_API getUsedPhysicalMemory()
        {
            struct sysinfo memInfo;

            sysinfo(&memInfo);

            return ((memInfo.totalram - memInfo.freeram) * memInfo.mem_unit);
        }

        string LAMBDACOMMON_API getComputerName()
        {
            char hostname[HOST_NAME_MAX];
            gethostname(hostname, HOST_NAME_MAX);
            return string(hostname);
        }

        string LAMBDACOMMON_API getOSName()
        {
            if (!osName.empty())
                return osName;
            filesystem::FilePath etc_os_release{"/etc/os-release"};
            filesystem::FilePath lsb_release{"/etc/lsb-release"};
            struct utsname uts;
            uname(&uts);
            if (!lsb_release.exists() && !etc_os_release.exists())
                osName = uts.sysname;

            if (lsb_release.exists())
            {
                ifstream lsb_release_in;
                lsb_release_in.open(lsb_release.toString(), ios::in);
                string word;
                bool record = false;
                if (lsb_release_in.is_open())
                {
                    while (lsb_release_in >> word)
                    {
                        if (word.find("DISTRIB_DESCRIPTION") != string::npos)
                        {
                            size_t a = word.find_last_of("N=") + 1;
                            osName = word.substr(a + 1, word.length() - a);
                            record = true;
                        }
                        else if (record)
                            osName += " " + lambdastring::replaceAll(word, "\"", "");
                    }
                    lsb_release_in.close();
                }
            }
            else
            {
                ifstream in;
                in.open(etc_os_release.toString(), ios::in);
                char line[256];
                if (in.is_open())
                {
                    while (in.getline(line, 256))
                    {
                        string line_{line};
                        if (line_.find("PRETTY_NAME=") != string::npos)
                        {
                            size_t equalSign = line_.find_first_of('"');
                            osName = line_.substr(equalSign + 1, line_.length() - equalSign - 2);
                            break;
                        }
                    }
                    in.close();
                }
            }

            if (string(uts.release).find("Microsoft") != string::npos)
                osName += " on Windows";
            return osName;
        }

        string LAMBDACOMMON_API getKernelVersion()
        {
            struct utsname uts;
            uname(&uts);
            return uts.release;
        }

        string LAMBDACOMMON_API getUserName()
        {
            struct passwd *userInfo;
            userInfo = getpwuid(getuid());
            return userInfo->pw_name;
        }

        string LAMBDACOMMON_API getUserDirectoryStr()
        {
            struct passwd *userInfo;
            userInfo = getpwuid(getuid());
            return userInfo->pw_dir;
        }

#endif

        filesystem::FilePath LAMBDACOMMON_API getUserDirectory()
        {
            return {getUserDirectoryStr()};
        }
    }
}

#ifdef LAMBDA_WINDOWS
#  undef INFO_BUFFER_SIZE
#endif