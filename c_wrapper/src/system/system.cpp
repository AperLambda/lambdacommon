/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/clambdacommon/system/system.h"
#include <lambdacommon/system/system.h>

#ifdef CLAMBDA_WINDOWS
#  define INFO_BUFFER_SIZE 32767
#  define HOST_NAME_MAX INFO_BUFFER_SIZE
#else

#  include <climits>
#  include <cstring>

#endif

const char *lc_sys_getProcessorName()
{
	static char cpuBrandStr[0x40];
	lambdacommon::system::getProcessorName().copy(cpuBrandStr, 0x40);
	return cpuBrandStr;
}

lc_SysArchitecture lc_sys_getProcessorArch()
{
	return static_cast<lc_SysArchitecture>(lambdacommon::system::getProcessorArch());
}

const char *lc_sys_getProcessorArchEnumStr(lc_SysArchitecture arch)
{
	static char archStr[8];
	lambdacommon::system::getProcessorArchEnumStr(static_cast<lambdacommon::system::SysArchitecture>(arch)).copy(
			archStr, 8);
	return archStr;
}

const char *lc_sys_getProcessorArchStr()
{
	return lc_sys_getProcessorArchEnumStr(lc_sys_getProcessorArch());
}

uint32_t lc_sys_getProcessorCores()
{
	return lambdacommon::system::getProcessorCores();
}

uint64_t lc_sys_getMemoryTotal()
{
	return lambdacommon::system::getMemoryTotal();
}

uint64_t lc_sys_getMemoryAvailable()
{
	return lambdacommon::system::getMemoryAvailable();
}

uint64_t lc_sys_getMemoryUsed()
{
	return lambdacommon::system::getMemoryUsed();
}

const char *lc_sys_getHostName()
{
	static char hostName[HOST_NAME_MAX];
	lambdacommon::system::getHostName().copy(hostName, HOST_NAME_MAX);
	return hostName;
}

const char *lc_sys_getOSName()
{
	static char osName[256];
	lambdacommon::system::getOSName().copy(osName, 256);
	return osName;
}

const char *lc_sys_getKernelVersion()
{
	static char kernelVersion[64];
	lambdacommon::system::getKernelVersion().copy(kernelVersion, 64);
	return kernelVersion;
}

const char *lc_sys_getUserName()
{
	static char username[32];
	lambdacommon::system::getUserName().copy(username, 32);
	return username;
}

const char *lc_sys_getUserDirectoryStr()
{
	static char path[256];
	lambdacommon::system::getUserDirectoryStr().copy(path, 256);
	return path;
}

bool lc_sys_isProcessRunningAsRoot()
{
	return lambdacommon::system::isProcessRunningAsRoot();
}

void lc_sys_openURI(const char *uri)
{
	lambdacommon::system::openURI({uri});
}

void lc_sys_sleep(uint32_t time)
{
	lambdacommon::system::sleep(time);
}