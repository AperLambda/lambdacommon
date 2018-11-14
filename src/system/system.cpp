/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
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

#  include <Windows.h>
#  include <WinBase.h>
#  include <intrin.h>
#  include <ShlObj.h>

#  ifndef __MINGW32__

#    include <VersionHelpers.h>

#  endif
#else
#  ifdef LAMBDA_MAC_OSX

#    include <CoreFoundation/CFBundle.h>
#    include <ApplicationServices/ApplicationServices.h>

#  elif defined(LAMBDA_CYGWIN)

#    include <windows.h>

#  endif

#  include <unistd.h>
#  include <climits>
#  include <sys/types.h>
#  include <sys/sysinfo.h>
#  include <sys/utsname.h>
#  include <pwd.h>
#  include <fstream>

#endif

namespace lambdacommon
{
	namespace system
	{
		std::string os_name;

#ifdef LAMBDA_WINDOWS

		std::string LAMBDACOMMON_API get_cpu_name()
		{
			int cpu_info[4] = {-1};
			char cpu_brand_str[0x40];
			__cpuid(cpu_info, 0x80000000);
			auto n_ex_ids = static_cast<unsigned int>(cpu_info[0]);

			memset(cpu_brand_str, 0, sizeof(cpu_brand_str));

			// Get the information associated with each extended ID.
			for (unsigned int i = 0x80000000; i <= n_ex_ids; ++i)
			{
				__cpuid(cpu_info, i);
				// Interpret CPU brand string.
				if (i == 0x80000002)
					memcpy(cpu_brand_str, cpu_info, sizeof(cpu_info));
				else if (i == 0x80000003)
					memcpy(cpu_brand_str + 16, cpu_info, sizeof(cpu_info));
				else if (i == 0x80000004)
					memcpy(cpu_brand_str + 32, cpu_info, sizeof(cpu_info));
			}
			return {cpu_brand_str};
		}

		typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

		LPFN_ISWOW64PROCESS fn_IsWow64Process;

		BOOL is_wow64()
		{
			BOOL b_is_wow64 = FALSE;

			//IsWow64Process is not available on all supported versions of Windows.
			//Use GetModuleHandle to get a handle to the DLL that contains the function
			//and GetProcAddress to get a pointer to the function if available.

			fn_IsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
					GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

			if (nullptr != fn_IsWow64Process)
			{
				if (!fn_IsWow64Process(GetCurrentProcess(), &b_is_wow64))
				{
					//handle error
				}
			}
			return b_is_wow64;
		}

		SysArchitecture LAMBDACOMMON_API get_processor_arch()
		{
			SYSTEM_INFO sys_info;
			if (is_wow64())
				GetNativeSystemInfo(&sys_info);
			else
				GetSystemInfo(&sys_info);

			switch (sys_info.wProcessorArchitecture)
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

		std::string LAMBDACOMMON_API get_processor_arch_str()
		{
			return get_processor_arch_enum_str();
		}

		uint32_t LAMBDACOMMON_API get_cpu_cores()
		{
			SYSTEM_INFO system_info;
			GetSystemInfo(&system_info);
			return system_info.dwNumberOfProcessors;
		}

		uint64_t LAMBDACOMMON_API get_memory_total()
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof(statex);
			GlobalMemoryStatusEx(&statex);
			return statex.ullTotalPhys;
		}

		uint64_t LAMBDACOMMON_API get_memory_available()
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof(statex);
			GlobalMemoryStatusEx(&statex);
			return statex.ullAvailPhys;
		}

		uint64_t LAMBDACOMMON_API get_memory_used()
		{
			return get_memory_total() - get_memory_available();
		}

		std::string LAMBDACOMMON_API get_host_name()
		{
			char info_buf[INFO_BUFFER_SIZE];
			DWORD buf_char_count = INFO_BUFFER_SIZE;

			if (!GetComputerName(info_buf, &buf_char_count))
				return "";

			return {info_buf, buf_char_count};
		}

		std::string LAMBDACOMMON_API get_os_name()
		{
#  ifdef __MINGW32__
			return "Windows with MinGW";
#  elif defined(LAMBDA_CYGWIN)
			return "Windows with CYGWIN";
#  else
			// Holy shit, what the fuck is this shitty code?
			std::string win_name = "Windows ";

			if (IsWindowsServer())
				win_name += "Server ";

			if (IsWindows10OrGreater())
				win_name += "10";
			else if (IsWindows8Point1OrGreater())
				win_name += "8.1";
			else if (IsWindows8OrGreater())
				win_name += "8";
			else if (IsWindows7SP1OrGreater())
				win_name += "7 SP1";
			else if (IsWindows7OrGreater())
				win_name += "7";
			else if (IsWindowsVistaSP2OrGreater())
				win_name += "Vista SP2";
			else if (IsWindowsVistaSP1OrGreater())
				win_name += "Vista SP1";
			else if (IsWindowsVistaOrGreater())
				win_name += "Vista";
			else if (IsWindowsXPOrGreater())
				win_name += "XP";
			return win_name;
#  endif
		}

		std::string LAMBDACOMMON_API get_kernel_version()
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

		std::string LAMBDACOMMON_API get_user_name()
		{
			char info_buf[INFO_BUFFER_SIZE];
			DWORD buf_char_count = INFO_BUFFER_SIZE;

			if (!GetUserName(info_buf, &buf_char_count))
				return "";

			return {info_buf, buf_char_count};
		}

		std::string LAMBDACOMMON_API get_user_directory_str()
		{
#  ifdef __MINGW32__
			TCHAR path[MAX_PATH];
			if (FAILED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, path)))
				return "";
			return std::string(path);
#  else // __MINGW32__
			PWSTR sz_path;
			if (FAILED(SHGetKnownFolderPath(FOLDERID_Profile, KF_FLAG_NO_ALIAS, nullptr, &sz_path)))
				return "";
			return std::string(lstring::convert_wstring_to_string(std::wstring(sz_path)));
#  endif // __MINGW32__
		}

#else

		std::string LAMBDACOMMON_API get_cpu_name()
		{
			std::ifstream cpuinfo;
			cpuinfo.open("/proc/cpuinfo", std::ios::in);
			std::string word;
			//bool record = false;
			std::string cpu;
			if (cpuinfo.is_open())
			{
				for (std::string line; std::getline(cpuinfo, line);)
				{
					if (lstring::equals_ignore_case(line.substr(0, 10), "model name"))
					{
						size_t sperator_index = line.find_first_of(':');
						cpu = line.substr(sperator_index + 2);
						break;
					}
				}
				cpuinfo.close();
			}
			return cpu;
		}

		SysArchitecture LAMBDACOMMON_API get_processor_arch()
		{
			if (lstring::equals(get_processor_arch_str(), "x86_64"))
				return SysArchitecture::X86_64;
			else if (lstring::equals(get_processor_arch_str(), "i386"))
				return SysArchitecture::I386;
			else if (lstring::equals_ignore_case(get_processor_arch_str().substr(0, 3), "ARM"))
				return SysArchitecture::ARM;
			else
				return SysArchitecture::UNKNOWN;
		}

		std::string LAMBDACOMMON_API get_processor_arch_str()
		{
			utsname u_name{};
			if (uname(&u_name) != 0)
				return "Unknown";
			return u_name.machine;
		}

		uint32_t LAMBDACOMMON_API get_cpu_cores()
		{
			std::ifstream cpuinfo;
			cpuinfo.open("/proc/cpuinfo", std::ios::in);
			std::string word;
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

		uint64_t LAMBDACOMMON_API get_memory_total()
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

		uint64_t LAMBDACOMMON_API get_memory_available()
		{
			std::ifstream meminfo;
			meminfo.open("/proc/meminfo", std::ios::in);

			uint64_t mem_free = 0;

			if (meminfo.is_open())
			{
				std::string last;

				while (meminfo >> last)
				{
					if (lstring::equals_ignore_case(last, "MemAvailable:"))
					{
						meminfo >> mem_free;
						// kB to B
						mem_free = mem_free * 1024;
						break;
					}
				}

				meminfo.close();
			}

			return mem_free;
		}

		uint64_t LAMBDACOMMON_API get_memory_used()
		{
			std::ifstream meminfo;
			meminfo.open("/proc/meminfo", std::ios::in);

			uint64_t mem_used = 0;

			if (meminfo.is_open())
			{
				std::string last;

				while (meminfo >> last)
				{
					if (lstring::equals_ignore_case(last, "Active:"))
					{
						meminfo >> mem_used;
						// kB to B
						mem_used = mem_used * 1024;
						break;
					}
				}

				meminfo.close();
			}

			return mem_used;
		}

		std::string LAMBDACOMMON_API get_host_name()
		{
			char hostname[HOST_NAME_MAX];
			gethostname(hostname, HOST_NAME_MAX);
			return {hostname};
		}

		std::string LAMBDACOMMON_API get_os_name()
		{
			if (!os_name.empty())
				return os_name;
			fs::FilePath etc_os_release{"/etc/os-release"};
			fs::FilePath lsb_release{"/etc/lsb-release"};
			struct utsname uts{};
			uname(&uts);
			if (!lsb_release.exists() && !etc_os_release.exists())
				os_name = uts.sysname;

			if (lsb_release.exists())
			{
				std::ifstream lsb_release_in;
				lsb_release_in.open(lsb_release.to_string(), std::ios::in);
				if (lsb_release_in.is_open())
				{
					std::string word;
					bool record = false;
					while (lsb_release_in >> word)
					{
						if (word.find("DISTRIB_DESCRIPTION") != std::string::npos)
						{
							size_t a = word.find_last_of("N=") + 1;
							os_name = word.substr(a + 1, word.length() - a);
							record = true;
						}
						else if (record)
							os_name += " " + lstring::replace_all(word, "\"", "");
					}
					lsb_release_in.close();
				}
			}
			else
			{
				std::ifstream in;
				in.open(etc_os_release.to_string(), std::ios::in);
				if (in.is_open())
				{
					char line[256];
					while (in.getline(line, 256))
					{
						std::string line_{line};
						if (line_.find("PRETTY_NAME=") != std::string::npos)
						{
							size_t equal_sign = line_.find_first_of('"');
							os_name = line_.substr(equal_sign + 1, line_.length() - equal_sign - 2);
							break;
						}
					}
					in.close();
				}
			}

			if (std::string(uts.release).find("Microsoft") != std::string::npos)
				os_name += " on Windows";
			return os_name;
		}

		std::string LAMBDACOMMON_API get_kernel_version()
		{
			struct utsname uts{};
			uname(&uts);
			return uts.release;
		}

		std::string LAMBDACOMMON_API get_user_name()
		{
			struct passwd *user_info;
			user_info = getpwuid(getuid());
			return user_info->pw_name;
		}

		std::string LAMBDACOMMON_API get_user_directory_str()
		{
			struct passwd *user_info;
			user_info = getpwuid(getuid());
			return user_info->pw_dir;
		}

#endif

		std::string LAMBDACOMMON_API get_processor_arch_enum_str(SysArchitecture arch)
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

		fs::FilePath LAMBDACOMMON_API get_user_directory()
		{
			return {get_user_directory_str()};
		}

		bool LAMBDACOMMON_API is_root()
		{
#ifdef LAMBDA_WINDOWS
			BOOL f_is_run_as_admin = FALSE;
			PSID p_administrators_group = nullptr;

			// Allocate and initialize a SID of the administrators group.
			SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
			if (!AllocateAndInitializeSid(
					&nt_authority,
					2,
					SECURITY_BUILTIN_DOMAIN_RID,
					DOMAIN_ALIAS_RID_ADMINS,
					0, 0, 0, 0, 0, 0,
					&p_administrators_group))
				goto cleanup;

			// Determine whether the SID of administrators group is enabled in
			// the primary access token of the process.
			if (!CheckTokenMembership(nullptr, p_administrators_group, &f_is_run_as_admin))
				goto cleanup;

			cleanup:
			// Centralized cleanup for all allocated resources.
			if (p_administrators_group)
			{
				FreeSid(p_administrators_group);
				p_administrators_group = nullptr;
			}

			return f_is_run_as_admin == TRUE;
#else
			return getuid() == 0;
#endif
		}

		void LAMBDACOMMON_API open_uri(const std::string &uri)
		{
#ifdef LAMBDA_WINDOWS
			// Use the Windows API.
			ShellExecuteA(nullptr, "open", uri.c_str(), nullptr, nullptr, SW_SHOW);
#elif defined(LAMBDA_MAC_OSX)
			// Use the Apple's framework.
			CFURLRef url = CFURLCreateWithBytes (
				  nullptr,                        // allocator
				  (UInt8*) uri.c_str(),     		// URLBytes
				  url_str.length(),            	// length
				  kCFStringEncodingASCII,      	// encoding
				  nullptr                         // baseURL
			);
			LSOpenCFURLRef(url, nullptr);
			CFRelease(url);
#elif defined(LAMBDA_CYGWIN)
			// Use of system is not recommended but none alternatives exist.
			if (::system(nullptr))
				::system((std::string("cygstart ") + uri).c_str());
#else
			// Use of system is not recommended but none alternatives exist.
			if (::system(nullptr))
				::system((std::string("xdg-open ") + uri).c_str());
#endif
		}

		void LAMBDACOMMON_API sleep(uint32_t time)
		{
#ifdef LAMBDA_WINDOWS
			Sleep(time);
#else
			auto goal = time / 1000 * CLOCKS_PER_SEC + clock();
			while (goal > clock());
#endif
		}
	}
}

#ifdef LAMBDA_WINDOWS
#  undef INFO_BUFFER_SIZE
#endif