/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/system/system.h"
#include <sstream>

#ifdef LAMBDA_WINDOWS

#  define INFO_BUFFER_SIZE 32767

#include <intrin.h>
#include <ShlObj.h>

#  ifndef __MINGW32__
#    include <VersionHelpers.h>
#  endif

#else

#include <unistd.h>
#include <climits>
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
		std::string osName;

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

		typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

		LPFN_ISWOW64PROCESS fnIsWow64Process;

		BOOL isWow64()
		{
			BOOL bIsWow64 = FALSE;

			//IsWow64Process is not available on all supported versions of Windows.
			//Use GetModuleHandle to get a handle to the DLL that contains the function
			//and GetProcAddress to get a pointer to the function if available.

			fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
					GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

			if (NULL != fnIsWow64Process)
			{
				if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
				{
					//handle error
				}
			}
			return bIsWow64;
		}

		SysArchitecture LAMBDACOMMON_API getProcessorArch()
		{
			SYSTEM_INFO sysInfo;
			if (isWow64())
				GetNativeSystemInfo(&sysInfo);
			else
				GetSystemInfo(&sysInfo);

			switch (sysInfo.wProcessorArchitecture)
			{
				case PROCESSOR_ARCHITECTURE_AMD64:
					return SysArchitecture::X86_64;
				case PROCESSOR_ARCHITECTURE_INTEL:
					return SysArchitecture::I386;
				case PROCESSOR_ARCHITECTURE_ARM:
					return SysArchitecture::ARM;
				case PROCESSOR_ARCHITECTURE_UNKNOWN:
					return SysArchitecture::UNKNOWN;
				default:
					return SysArchitecture::UNKNOWN;
			}
		}

		string LAMBDACOMMON_API getProcessorArchStr()
		{
			return getProcessorArchEnumStr();
		}

		uint32_t LAMBDACOMMON_API getProcessorCores()
		{
			SYSTEM_INFO systemInfo;
			GetSystemInfo(&systemInfo);
			return systemInfo.dwNumberOfProcessors;
		}

		uint64_t LAMBDACOMMON_API getPhysicalMemory()
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof(statex);
			GlobalMemoryStatusEx(&statex);
			return statex.ullTotalPhys;
		}

		uint64_t LAMBDACOMMON_API getPhysicalMemoryFree()
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof(statex);
			GlobalMemoryStatusEx(&statex);
			return statex.ullAvailPhys;
		}

		uint64_t LAMBDACOMMON_API getPhysicalMemoryUsed()
		{
			return getPhysicalMemory() - getPhysicalMemoryFree();
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
#  ifdef __MINGW32__
			return "Windows with MinGW";
#  elif defined(LAMBDA_CYGWIN)
			return "Windows with CYGWIN";
#  else
			// Holy shit, what the fuck is this shitty code?
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
#  endif
		}

		string LAMBDACOMMON_API getKernelVersion()
		{
#if defined(__MINGW32__) || defined(LAMBDA_CYGWIN)
			OSVERSIONINFO vi;
			vi.dwOSVersionInfoSize = sizeof(vi);
			if (GetVersionEx(&vi) == 0) return "UNKNOWN";
			std::ostringstream str;
			str << vi.dwMajorVersion << "." << vi.dwMinorVersion << " (Build " << (vi.dwBuildNumber & 0xFFFF);
			if (vi.szCSDVersion[0]) str << ": " << vi.szCSDVersion;
			str << ")";
			return str.str();
#else
			// I don't know how to get it using Visual Studio compiler :/
			return "UNKNOWN";
#endif
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
			//bool record = false;
			string cpu;
			if (cpuinfo.is_open())
			{
				for (string line; getline(cpuinfo, line);)
				{
					if (lambdastring::equalsIgnoreCase(line.substr(0, 10), "model name"))
					{
						size_t separatorIndex = line.find_first_of(':');
						cpu = line.substr(separatorIndex + 2);
						break;
					}
				}
				cpuinfo.close();
			}
			return cpu;
		}

		SysArchitecture LAMBDACOMMON_API getProcessorArch()
		{
			if (lambdastring::equals(getProcessorArchStr(), "x86_64"))
				return SysArchitecture::X86_64;
			else if (lambdastring::equals(getProcessorArchStr(), "i386"))
				return SysArchitecture::I386;
			else if (lambdastring::equalsIgnoreCase(getProcessorArchStr().substr(0, 3), "ARM"))
				return SysArchitecture::ARM;
			else
				return SysArchitecture::UNKNOWN;
		}

		string LAMBDACOMMON_API getProcessorArchStr()
		{
			utsname uName{};
			if (uname(&uName) != 0)
				return "Unknown";
			return uName.machine;
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

		uint64_t LAMBDACOMMON_API getMemoryTotal()
		{
#  ifdef _SC_PHYS_PAGES
			long pages = sysconf(_SC_PHYS_PAGES);
			long page_size = sysconf(_SC_PAGESIZE);
			return static_cast<uint64_t>(pages * page_size);
#  else
			uint64_t mem;
			size_t len = sizeof(mem);
			sysctlbyname("hw.memsize", &mem, &len, NULL, 0);
			return mem / sysconf(_SC_PAGE_SIZE);
#  endif
		}

		uint64_t LAMBDACOMMON_API getMemoryAvailable()
		{
			ifstream meminfo;
			meminfo.open("/proc/meminfo", ios::in);

			uint64_t memFree = 0;

			if (meminfo.is_open())
			{
				string last;

				while (meminfo >> last)
				{
					if (lambdastring::equalsIgnoreCase(last, "MemAvailable:"))
					{
						meminfo >> memFree;
						// kB to B
						memFree = memFree * 1024;
						break;
					}
				}

				meminfo.close();
			}

			return memFree;
		}

		uint64_t LAMBDACOMMON_API getMemoryUsed()
		{
			ifstream meminfo;
			meminfo.open("/proc/meminfo", ios::in);

			uint64_t memUsed = 0;

			if (meminfo.is_open())
			{
				string last;

				while (meminfo >> last)
				{
					if (lambdastring::equalsIgnoreCase(last, "Active:"))
					{
						meminfo >> memUsed;
						// kB to B
						memUsed = memUsed * 1024;
						break;
					}
				}

				meminfo.close();
			}

			return memUsed;
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
			struct utsname uts{};
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
						} else if (record)
							osName += " " + lambdastring::replaceAll(word, "\"", "");
					}
					lsb_release_in.close();
				}
			} else
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
			struct utsname uts{};
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

		std::string LAMBDACOMMON_API getProcessorArchEnumStr(SysArchitecture arch)
		{
			switch (arch)
			{
				case SysArchitecture::ARM:
					return "ARM";
				case SysArchitecture::ARM64:
					return "ARM64";
				case SysArchitecture::I386:
					return "i386";
				case SysArchitecture::X86_64:
					return "x86_64";
				case SysArchitecture::UNKNOWN:
					return "Unknown";
				default:
					return "Unknown";
			}
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