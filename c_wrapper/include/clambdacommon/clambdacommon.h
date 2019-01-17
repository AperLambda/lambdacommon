/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_H
#define CLAMBDACOMMON_H

#ifdef _WIN32
#  ifdef _WIN64
#    define CLAMBDA_WIN64
#  else
#    define CLAMBDA_WIN32
#  endif
#  define CLAMBDA_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#  define CLAMBDA_MAC_OSX
#elif defined(__ANDROID__)
#  define CLAMBDA_ANDROID
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#  define CLAMBDA_BSD
#  ifdef __FreeBSD__
#    define CLAMBDA_FREEBSD
#  elif defined(__NetBSD__)
#    define CLAMBDA_NETBSD
#  elif defined(__OpenBSD__)
#    define CLAMBDA_OPENBSD
#  endif
#elif defined(sun) || defined(__sun)
#  define CLAMBDA_SOLARIS
#endif
#ifdef __CYGWIN__
#  define CLAMBDA_CYGWIN
#endif

#include "maths.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/*!
 * Gets the runtime version of lambdacommon as a string.
 * @param version The runtime version of lambdacommon.
 */
const char *lc_get_version();

void lc_delete_pointer(void **pointer);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_H
