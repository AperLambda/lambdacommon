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

const char *lcommon_sys_getProcessorName()
{
	static char cpuBrandStr[0x40];
	lambdacommon::system::getProcessorName().copy(cpuBrandStr, 0x40);
	return cpuBrandStr;
}

lcommon_SysArchitecture lcommon_sys_getProcessorArch()
{
	return static_cast<lcommon_SysArchitecture>(lambdacommon::system::getProcessorArch());
}

const char *lcommon_sys_getProcessorArchEnumStr(lcommon_SysArchitecture arch)
{
	static char archStr[8];
	lambdacommon::system::getProcessorArchEnumStr(static_cast<lambdacommon::system::SysArchitecture>(arch)).copy(
			archStr, 8);
	return archStr;
}

const char *lcommon_sys_getProcessorArchStr()
{
	return lcommon_sys_getProcessorArchEnumStr(lcommon_sys_getProcessorArch());
}

uint32_t lcommon_sys_getProcessorCores()
{
	return lambdacommon::system::getProcessorCores();
}

uint64_t lcommon_sys_getMemoryTotal()
{
	return lambdacommon::system::getMemoryTotal();
}

uint64_t lcommon_sys_getMemoryAvailable()
{
	return lambdacommon::system::getMemoryAvailable();
}

uint64_t lcommon_sys_getMemoryUsed()
{
	return lambdacommon::system::getMemoryUsed();
}

const char *lcommon_sys_getHostName()
{
	static char hostName[HOST_NAME_MAX];
	lambdacommon::system::getHostName().copy(hostName, HOST_NAME_MAX);
	return hostName;
}

const char *lcommon_sys_getOSName()
{
	static char osName[256];
	lambdacommon::system::getOSName().copy(osName, 256);
	return osName;
}

const char *lcommon_sys_getKernelVersion()
{
	static char kernelVersion[64];
	lambdacommon::system::getKernelVersion().copy(kernelVersion, 64);
	return kernelVersion;
}

const char *lcommon_sys_getUserName()
{
	static char username[32];
	lambdacommon::system::getUserName().copy(username, 32);
	return username;
}

const char *lcommon_sys_getUserDirectoryStr()
{
	static char path[256];
	lambdacommon::system::getUserDirectoryStr().copy(path, 256);
	return path;
}

bool lcommon_sys_isProcessRunningAsRoot()
{
	return lambdacommon::system::isProcessRunningAsRoot();
}

void lcommon_sys_sleep(uint32_t time)
{
	lambdacommon::system::sleep(time);
}