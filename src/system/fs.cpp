/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/system/fs.h"
#include <lambdacommon/lstring.h>
#include <stdexcept>

#ifdef LAMBDA_WINDOWS

#include <Windows.h>

#define STAT_STRUCT _stati64
#define STAT_METHOD _stati64

#elif defined(LAMBDA_MAC_OSX)

#include <mach/error.h>

#else

#include <unistd.h>
#include <cstring>
#include <climits>

#define STAT_STRUCT stat
#define STAT_METHOD stat

#endif

#include <sys/stat.h>

namespace lambdacommon
{
	namespace fs
	{
		std::wstring LAMBDACOMMON_API get_cwd_wstr()
		{
#ifdef LAMBDA_WINDOWS
			wchar_t temp[MAX_PATH];
			if (!_wgetcwd(temp, MAX_PATH))
				throw std::runtime_error("fs.cpp(" + std::to_string(__LINE__ - 1) + ")@lambdacommon::fs::get_cwd_wstr(): Internal error \"" + std::to_string(GetLastError())
										 + "\"");
			return {temp};
#else
			return lstring::convert_string_to_wstring(get_cwd_str());
#endif
		}

		std::string LAMBDACOMMON_API get_cwd_str()
		{
#ifdef LAMBDA_WINDOWS
			return lstring::convert_wstring_to_string(get_cwd_wstr());
#else
			char temp[PATH_MAX];
			if (getcwd(temp, PATH_MAX) == nullptr)
				throw std::runtime_error("fs.cpp(" + std::to_string(__LINE__ - 1) + ")@lambdacommon::fs::get_cwd_str(): Internal error \"" + strerror(errno) + "\"");
			return {temp};
#endif
		}
	}
}