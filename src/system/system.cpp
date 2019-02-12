/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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
#else
#  ifdef LAMBDA_MAC_OSX
#    include <mach/vm_statistics.h>
#    include <mach/mach_types.h>
#    include <mach/mach_init.h>
#    include <mach/mach_host.h>
#    include <CoreFoundation/CFBundle.h>
#    include <ApplicationServices/ApplicationServices.h>
#  else
#    include <sys/sysinfo.h>
#    ifdef LAMBDA_ANDROID
#      include <sys/system_properties.h>
#    elif defined(LAMBDA_CYGWIN)
#      include <windows.h>
#    endif
#  endif

#if __has_include(<sysconf.h>)
#  include <sysconf.h>
#elif __has_include(<sys/sysconf.h>)
#  include <sys/sysconf.h>
#elif __has_include(<sysctl.h>)
#  include <sysctl.h>
#elif __has_include(<sys/sysctl.h>)
#  include <sys/sysctl.h>
#elif __has_include(<linux/sysctl.h>)
#  include <linux/sysctl.h>
#elif !defined(LAMBDA_ANDROID) and !defined(LAMBDA_CYGWIN)
#  error "Cannot find any replacement for sys/sysctl.h"
#endif

#  include <unistd.h>
#  include <climits>
#  include <sys/types.h>
#  include <sys/utsname.h>
#  include <pwd.h>
#  include <fstream>

#  ifndef HOST_NAME_MAX
#    if defined(_POSIX_HOST_NAME_MAX)
#      define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
#    elif defined(MAXHOSTNAMELEN)
#      define HOST_NAME_MAX MAXHOSTNAMELEN
#    elif defined(LAMBDA_MAC_OSX)
#      define HOST_NAME_MAX 255
#    endif
#  endif /* HOST_NAME_MAX */

#endif

#include <thread>

namespace lambdacommon
{
	namespace system
	{
		std::string os_name;

		bool is_arch_from_arm_family(SysArchitecture arch)
		{
			return arch == ARM || arch == ARM64 || arch == ARMv7 || arch == ARMv8_64;
		}

		uint32_t LAMBDACOMMON_API get_cpu_cores()
		{
			return std::thread::hardware_concurrency();
		}

#ifdef LAMBDA_WINDOWS

		typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

		typedef NTSTATUS (WINAPI *LPFN_RtlGetVersion)(OSVERSIONINFOEXW *);

		typedef LONG (WINAPI *LPFN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW *, ULONG, ULONGLONG);

#define NT_INFORMATION(status) (status >= 0x40000000 && status <= 0x7FFFFFFF)
#define NT_SUCCESS(status) (status <= 0x3FFFFFFF || NT_INFORMATION(status))
#define NT_WARNING(status) (status >= 0x80000000 && status <= 0xBFFFFFFF)
#define NT_ERROR(status) (status >= 0xC0000000 && status <= 0xFFFFFFFF)

		class SysContext_Win
		{
		private:
			HMODULE ntdll;
			LPFN_ISWOW64PROCESS fn_IsWow64Process;
			LPFN_RtlGetVersion fn_RtlGetVersion;
			LPFN_RtlVerifyVersionInfo fn_RtlVerifyVersionInfo;

		public:
			~SysContext_Win()
			{
				if (ntdll)
					FreeLibrary(ntdll);
			}

			void init()
			{
				if (fn_IsWow64Process == nullptr)
					fn_IsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
				// Load ntdll and its functions to hack some limitations.
				if (ntdll = LoadLibraryA("ntdll.dll"); ntdll) {
					fn_RtlGetVersion = (LPFN_RtlGetVersion) GetProcAddress(ntdll, "RtlGetVersion");
					fn_RtlVerifyVersionInfo = (LPFN_RtlVerifyVersionInfo) GetProcAddress(ntdll, "RtlVerifyVersionInfo");
				}
			}

			bool is_wow64()
			{
				BOOL b_is_wow64 = FALSE;

				//IsWow64Process is not available on all supported versions of Windows.
				//Use GetModuleHandle to get a handle to the DLL that contains the function
				//and GetProcAddress to get a pointer to the function if available.

				if (fn_IsWow64Process != nullptr) {
					if (!fn_IsWow64Process(GetCurrentProcess(), &b_is_wow64)) {
						//handle error
					}
				}
				return static_cast<bool>(b_is_wow64);
			}

			bool is_windows_version_or_greater(WORD major, WORD minor, WORD sp)
			{
				if (!fn_RtlVerifyVersionInfo)
					return false;
				OSVERSIONINFOEXW osvi = {sizeof(osvi), major, minor, 0, 0, {0}, sp};
				DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR;
				ULONGLONG cond = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
				cond = VerSetConditionMask(cond, VER_MINORVERSION, VER_GREATER_EQUAL);
				cond = VerSetConditionMask(cond, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
				/* HACK: Use RtlVerifyVersionInfo instead of VerifyVersionInfoW as the
				 *       latter lies unless the user knew to embed a non-default manifest
				 *       announcing support for Windows 10 via supportedOS GUID.
				 */
				if (fn_RtlVerifyVersionInfo)
					return fn_RtlVerifyVersionInfo(&osvi, mask, cond) == 0;
				else {
#ifndef __MINGW32__
					return VerifyVersionInfoW(&osvi, mask, cond) == 0;
#else
					return false;
#endif
				}
			}

			NTSTATUS get_version(OSVERSIONINFOEXW *osvi)
			{
				if (fn_RtlGetVersion)
					return fn_RtlGetVersion(osvi);
				else {
#ifdef __MINGW32__
					OSVERSIONINFOW mingw_osvi;
					mingw_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
					if (!GetVersionExW(&mingw_osvi))
						// Return a dummy error value.
						return 0xC0000000;
					osvi->dwPlatformId = mingw_osvi.dwPlatformId;
					osvi->dwMajorVersion = mingw_osvi.dwMajorVersion;
					osvi->dwMinorVersion = mingw_osvi.dwMinorVersion;
					osvi->dwBuildNumber = mingw_osvi.dwBuildNumber;
					wcscpy_s(osvi->szCSDVersion, 128, mingw_osvi.szCSDVersion);
					return 0x40000000; // Dummy value.
#else
					// Return a dummy error value.
					return 0xC0000000;
#endif
				}
			}

			/*!
			 * Checks whether the running Windows is a Windows Server.
			 * @return True if the running Windows is a Windows Server, else false.
			 */
			bool is_windows_server()
			{
				OSVERSIONINFOEXW osvi = {sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0, 0, VER_NT_WORKSTATION};
				DWORDLONG const dwlConditionMask = VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL);

				if (fn_RtlVerifyVersionInfo)
					return static_cast<bool>(fn_RtlVerifyVersionInfo(&osvi, VER_PRODUCT_TYPE, dwlConditionMask));
				else {
#ifndef __MINGW32__
					return !VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);
#else
					return false;
#endif
				}
			}
		};

		/*!
		 * The Context of the System on Windows.
		 */
		std::unique_ptr<SysContext_Win> win_context;

		void syscontext_load()
		{
			if (win_context == nullptr) {
				win_context = std::make_unique<SysContext_Win>();
				win_context->init();
			}
		}

		bool is_wow64()
		{
			syscontext_load();
			return win_context->is_wow64();
		}

		bool is_windows_version_or_greater(WORD major, WORD minor, WORD sp)
		{
			syscontext_load();
			return win_context->is_windows_version_or_greater(major, minor, sp);
		}

		NTSTATUS get_version(OSVERSIONINFOEXW *osvi)
		{
			syscontext_load();
			return win_context->get_version(osvi);
		}

		bool is_windows_server()
		{
			syscontext_load();
			return win_context->is_windows_server();
		}

#define IsWindowsXPOrGreater() is_windows_version_or_greater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0)
#define IsWindowsVistaOrGreater() is_windows_version_or_greater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0)
#define IsWindowsVistaSP1OrGreater() is_windows_version_or_greater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 1)
#define IsWindowsVistaSP2OrGreater() is_windows_version_or_greater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 2)
#define IsWindows7OrGreater() is_windows_version_or_greater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0)
#define IsWindows7SP1OrGreater() is_windows_version_or_greater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 1)
#define IsWindows8OrGreater() is_windows_version_or_greater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0)
#define IsWindows8Point1OrGreater() is_windows_version_or_greater(HIBYTE(_WIN32_WINNT_WINBLUE), LOBYTE(_WIN32_WINNT_WINBLUE), 0)
#define IsWindows10OrGreater() is_windows_version_or_greater(HIBYTE(0x0A00), LOBYTE(0x0A00), 0)

		std::string LAMBDACOMMON_API get_cpu_name()
		{
#if defined(LAMBDA_ARM) || defined(LAMBDA_ARM64)
			return "";
#else
			int cpu_info[4] = {-1};
			char cpu_brand_str[0x40];
			__cpuid(cpu_info, 0x80000000);
			auto n_ex_ids = static_cast<unsigned int>(cpu_info[0]);

			memset(cpu_brand_str, 0, sizeof(cpu_brand_str));

			// Get the information associated with each extended ID.
			for (unsigned int i = 0x80000000; i <= n_ex_ids; ++i) {
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
#endif
		}

#ifdef __MINGW32__
#define PROCESSOR_ARCHITECTURE_ARM64 12
#endif

		SysArchitecture LAMBDACOMMON_API get_processor_arch()
		{
			SYSTEM_INFO sys_info;
			if (is_wow64())
				GetNativeSystemInfo(&sys_info);
			else
				GetSystemInfo(&sys_info);

			switch (sys_info.wProcessorArchitecture) {
				case PROCESSOR_ARCHITECTURE_AMD64:
					return SysArchitecture::X86_64;
				case PROCESSOR_ARCHITECTURE_INTEL:
					return SysArchitecture::I386;
				case PROCESSOR_ARCHITECTURE_ARM:
					return SysArchitecture::ARM;
				case PROCESSOR_ARCHITECTURE_ARM64:
					return SysArchitecture::ARM64;
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
#  ifdef LAMBDA_CYGWIN
			return "Windows with CYGWIN";
#  else
			// Holy shit, what the fuck is this shitty code?
			std::string win_name = "Windows ";

			if (is_windows_server())
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
			OSVERSIONINFOEXW osvi;
			osvi.dwOSVersionInfoSize = sizeof(osvi);
			if (!NT_SUCCESS(get_version(&osvi))) return "UNKNOWN";
			std::ostringstream string;
			string << std::to_string(osvi.dwMajorVersion) << '.' << std::to_string(osvi.dwMinorVersion) << " (Build " << std::to_string(osvi.dwBuildNumber & 0xFFFF);
			if (osvi.szCSDVersion[0]) string << ": " << osvi.szCSDVersion;
			string << ")";
			return string.str();
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
			if (FAILED(SHGetFolderPath(nullptr, CSIDL_PROFILE, nullptr, 0, path)))
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
#ifdef LAMBDA_MAC_OSX
			char buffer[128];
			size_t buffer_len = 128;
			sysctlbyname("machdep.cpu.brand_string", &buffer, &buffer_len, nullptr, 0);
			return {buffer};
#else
			std::ifstream cpuinfo;
			cpuinfo.open("/proc/cpuinfo", std::ios::in);
			std::string word;
			//bool record = false;
			std::string cpu;
			if (cpuinfo.is_open()) {
				for (std::string line; std::getline(cpuinfo, line);) {
#ifdef LAMBDA_ANDROID
					if (lstring::starts_with_ignore_case(line, "Hardware"))
#else
					if (lstring::starts_with_ignore_case(line, "model name"))
#endif
					{
						size_t separator_index = line.find_first_of(':');
						cpu = line.substr(separator_index + 2);
						break;
					}
				}
				cpuinfo.close();
			}
			return cpu;
#endif
		}

		SysArchitecture LAMBDACOMMON_API get_processor_arch()
		{
			if (lstring::equals(get_processor_arch_str(), "x86_64"))
				return SysArchitecture::X86_64;
			else if (lstring::equals(get_processor_arch_str(), "i386"))
				return SysArchitecture::I386;
			else if (lstring::equals_ignore_case(get_processor_arch_str().substr(0, 3), "ARM"))
				return SysArchitecture::ARM;
			else if (lstring::equals_ignore_case(get_processor_arch_str(), "armv7l"))
				return SysArchitecture::ARMv7;
			else if (lstring::equals_ignore_case(get_processor_arch_str(), "aarch32"))
				return SysArchitecture::ARMv8_32;
			else if (lstring::equals_ignore_case(get_processor_arch_str(), "aarch64"))
				return SysArchitecture::ARMv8_64;
			else if (lstring::equals_ignore_case(get_processor_arch_str(), "riscv32"))
				return SysArchitecture::RISCV32;
			else if (lstring::equals_ignore_case(get_processor_arch_str(), "riscv64"))
				return SysArchitecture::RISCV64;
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

		uint64_t LAMBDACOMMON_API get_memory_total()
		{
#  ifdef _SC_PHYS_PAGES
			long pages = sysconf(_SC_PHYS_PAGES);
			long page_size = sysconf(_SC_PAGESIZE);
			return static_cast<uint64_t>(pages * page_size);
#  else
			uint64_t mem;
			size_t len = sizeof(mem);
			sysctlbyname("hw.memsize", &mem, &len, nullptr, 0);
			return mem / sysconf(_SC_PAGE_SIZE);
#  endif
		}

		uint64_t LAMBDACOMMON_API get_memory_available()
		{
			uint64_t mem_free = 0;
#ifdef LAMBDA_MAC_OSX
			vm_size_t page_size;
			mach_port_t mach_port;
			mach_msg_type_number_t count;
			vm_statistics64_data_t vm_stats;

			mach_port = mach_host_self();
			count = sizeof(vm_stats) / sizeof(natural_t);
			if (KERN_SUCCESS == host_page_size(mach_port, &page_size) && KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO, (host_info64_t) &vm_stats, &count))
				mem_free = static_cast<uint64_t>(vm_stats.free_count + page_size);
#else
			std::ifstream meminfo;
			meminfo.open("/proc/meminfo", std::ios::in);

			if (meminfo.is_open()) {
				std::string last;

				while (meminfo >> last) {
					if (lstring::equals_ignore_case(last, "MemAvailable:")) {
						meminfo >> mem_free;
						// kB to B
						mem_free = mem_free * 1024;
						break;
					} else if (lstring::equals_ignore_case(last, "MemFree:")) {
						meminfo >> mem_free;
						// kB to B
						mem_free = mem_free * 1024;
						break;
					}
				}

				meminfo.close();
			}
			if (mem_free == 0)
				mem_free = get_memory_total() - get_memory_used();
#endif
			return mem_free;
		}

		uint64_t LAMBDACOMMON_API get_memory_used()
		{
			std::ifstream meminfo;
			meminfo.open("/proc/meminfo", std::ios::in);

			uint64_t mem_used = 0;

			if (meminfo.is_open()) {
				std::string last;

				while (meminfo >> last) {
					if (lstring::equals_ignore_case(last, "Active:")) {
						meminfo >> mem_used;
						// kB to B
						mem_used = mem_used * 1024;
						break;
					}
				}

				meminfo.close();
			}

			if (mem_used == 0)
				mem_used = get_memory_total() - get_memory_available();

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
#ifdef LAMBDA_ANDROID
			char os_version[PROP_VALUE_MAX + 1];
			size_t os_version_length = static_cast<size_t>(__system_property_get("ro.build.version.release", os_version));
			return "Android " + std::string(os_version, os_version_length);
#else
			if (!os_name.empty())
				return os_name;
			fs::FilePath etc_os_release{"/etc/os-release"};
			fs::FilePath lsb_release{"/etc/lsb-release"};
			struct utsname uts{};
			uname(&uts);
			if (!lsb_release.exists() && !etc_os_release.exists())
				os_name = uts.sysname;

			if (lsb_release.exists()) {
				std::ifstream lsb_release_in;
				lsb_release_in.open(lsb_release.to_string(), std::ios::in);
				if (lsb_release_in.is_open()) {
					std::string word;
					bool record = false;
					while (lsb_release_in >> word) {
						if (word.find("DISTRIB_DESCRIPTION") != std::string::npos) {
							size_t a = word.find_last_of("N=") + 1;
							os_name = word.substr(a + 1, word.length() - a);
							record = true;
						} else if (record)
							os_name += " " + lstring::replace_all(word, "\"", "");
					}
					lsb_release_in.close();
				}
			} else {
				std::ifstream in;
				in.open(etc_os_release.to_string(), std::ios::in);
				if (in.is_open()) {
					char line[256];
					while (in.getline(line, 256)) {
						std::string line_{line};
						if (line_.find("PRETTY_NAME=") != std::string::npos) {
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
#endif
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
			switch (arch) {
				case ARM:
					return "ARM";
				case ARM64:
					return "ARM64";
				case ARMv8_64:
					return "ARMv8 64bit";
				case I386:
					return "i386";
				case RISCV32:
					return "riscv32";
				case RISCV64:
					return "riscv64";
				case X86_64:
					return "x86_64";
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
			if (p_administrators_group) {
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
#elif defined(LAMBDA_CYGWIN)
			// Use of system is not recommended but none alternatives exist.
			if (::system(nullptr))
				::system((std::string("cygstart ") + uri).c_str());
#elif defined(LAMBDA_MAC_OSX)
			// Use the Apple's framework.
			CFURLRef url = CFURLCreateWithBytes (
				  nullptr,					// allocator
				  (UInt8*) uri.c_str(),		// URLBytes
				  uri.length(),				// length
				  kCFStringEncodingASCII,	// encoding
				  nullptr					// baseURL
			);
			LSOpenCFURLRef(url, nullptr);
			CFRelease(url);
#elif defined(LAMBDA_ANDROID)
			// Do nothing because it requires JNI calls.
#else
			// Use of system is not recommended but none alternatives exist.
			if (::system(nullptr))
				::system((std::string("xdg-open ") + uri).c_str());
#endif
		}

		void LAMBDACOMMON_API sleep(utime_t time)
		{
#ifdef LAMBDA_WINDOWS
			Sleep(static_cast<DWORD>(time));
#else
			auto goal = time::get_time_millis() + time;
			while (goal > time::get_time_millis());
#endif
		}
	}
}

#ifdef LAMBDA_WINDOWS
#  undef INFO_BUFFER_SIZE
#endif
