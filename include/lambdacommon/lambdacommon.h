/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_LIBRARY_H
#define LAMBDACOMMON_LIBRARY_H

#include "lambdacommon_version.h"
#include "lambdacommon_exports.h"
#include <string>

#ifdef _WIN32
#  ifdef _WIN64
#    define LAMBDA_WIN64
#  else
#    define LAMBDA_WIN32
#  endif
#  define LAMBDA_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#  define LAMBDA_MAC_OSX
#elif defined(__ANDROID__)
#  define LAMBDA_ANDROID
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#  define LAMBDA_BSD
#  ifdef __FreeBSD__
#    define LAMBDA_FREEBSD
#  elif defined(__NetBSD__)
#    define LAMBDA_NETBSD
#  elif defined(__OpenBSD__)
#    define LAMBDA_OPENBSD
#  elif defined(__DragonFly__)
#    define LAMBDA_DRAGONFLY
#  endif
#elif defined(sun) || defined(__sun)
#  define LAMBDA_SOLARIS
#endif
#ifdef __CYGWIN__
#  define LAMBDA_CYGWIN
#endif

#if defined(__arm__) || defined(_ARM) || defined(_M_ARM) || defined(__arm)
#  define LAMBDA_ARM
#elif defined(__aarch64__)
#  define LAMBDA_ARM64
#endif

// Deletes the pointer and sets the variable to null. It's just simpler to write like this.
#define LCOMMON_DELETE_POINTER(pointer) delete pointer; pointer = nullptr;

#define LCOMMON_DEFINE_TUPLE_ELEMENT(index, of, return_type) struct tuple_element<index, of> { using type = return_type; };

namespace lambdacommon
{
	class Nullable
	{
	public:
		/*!
		 * Checks whether the object is null.
		 * @return True if the object is null, else false.
		 */
		virtual bool is_null() const = 0;
	};

	/*!
	 * Gets the runtime version of lambdacommon as a string.
	 * @return The runtime version of lambdacommon.
	 */
	extern std::string LAMBDACOMMON_API get_version();
}

#endif //LAMBDACOMMON_LIBRARY_H
