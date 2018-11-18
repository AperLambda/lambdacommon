/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
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

namespace lambdacommon
{
	namespace system
	{
		OS LAMBDACOMMON_API os::get_os()
		{
#ifdef LAMBDA_WINDOWS
#ifdef LAMBDA_WIN64
			return OS::WINDOWS_64;
#else
			return OS::WINDOWS_32;
#endif
#elif defined(LAMBDA_MAC_OSX)
			return OS::MAC_OSX;
#elif defined(LAMBDA_ANDROID)
			return OS::OS_ANDROID;
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

		std::string LAMBDACOMMON_API os::get_os_name(OS os)
		{
			std::string os_name = std::string();
			switch (os)
			{
				case WINDOWS_32:
					os_name = "Windows 32-bits";
					break;
				case WINDOWS_64:
					os_name = "Windows 64-bits";
					break;
				case MAC_OSX:
					os_name = "Mac OSX";
					break;
				case LINUX:
					os_name = "Linux";
					break;
				case FREEBSD:
					os_name = "FreeBSD";
					break;
				case NETBSD:
					os_name = "NetBSD";
					break;
				case OPENBSD:
					os_name = "OpenBSD";
					break;
				case CYGWIN:
					os_name = "Cygwin";
					break;
				case OS_ANDROID:
					os_name = "Android";
					break;
				case OS_UNKNOWN:
					os_name = "Unknown";
					break;
			}
			return os_name;
		}
	}
}