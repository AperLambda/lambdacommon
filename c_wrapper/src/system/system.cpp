/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
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

const char* lc_sys_get_cpu_name() {
    static char cpu_brand_str[0x40];
    lambdacommon::system::get_cpu_name().copy(cpu_brand_str, 0x40);
    return cpu_brand_str;
}

lc_SysArchitecture lc_sys_get_processor_arch() {
    return static_cast<lc_SysArchitecture>(lambdacommon::system::get_processor_arch());
}

const char* lc_sys_get_processor_arch_enum_str(lc_SysArchitecture arch) {
    static char arch_str[8];
    lambdacommon::system::get_processor_arch_enum_str(static_cast<lambdacommon::system::SysArchitecture>(arch)).copy(
            arch_str, 8);
    return arch_str;
}

const char* lc_sys_get_processor_arch_str() {
    return lc_sys_get_processor_arch_enum_str(lc_sys_get_processor_arch());
}

uint32_t lc_sys_get_cpu_cores() {
    return lambdacommon::system::get_cpu_cores();
}

uint64_t lc_sys_get_memory_total() {
    return lambdacommon::system::get_memory_total();
}

uint64_t lc_sys_get_memory_available() {
    return lambdacommon::system::get_memory_available();
}

uint64_t lc_sys_get_memory_used() {
    return lambdacommon::system::get_memory_used();
}

const char* lc_sys_get_host_name() {
    static char hostname[HOST_NAME_MAX];
    lambdacommon::system::get_host_name().copy(hostname, HOST_NAME_MAX);
    return hostname;
}

const char* lc_sys_get_os_name() {
    static char os_name[256];
    lambdacommon::system::get_os_name().copy(os_name, 256);
    return os_name;
}

const char* lc_sys_get_kernel_version() {
    static char kernel_version[64];
    lambdacommon::system::get_kernel_version().copy(kernel_version, 64);
    return kernel_version;
}

const char* lc_sys_get_user_name() {
    static char username[32];
    lambdacommon::system::get_user_name().copy(username, 32);
    return username;
}

const char* lc_sys_get_user_directory_str() {
    static char path[256];
    lambdacommon::system::get_user_directory_str().copy(path, 256);
    return path;
}

bool lc_sys_is_root() {
    return lambdacommon::system::is_root();
}

void lc_sys_open_uri(const char* uri) {
    lambdacommon::system::open_uri({uri});
}

void lc_sys_get_error_message(int error, char* msg, size_t msg_length) {
    lambdacommon::system::get_error_message(error).copy(msg, msg_length);
}

void lc_sys_sleep(uint32_t time) {
    lambdacommon::system::sleep(time);
}
