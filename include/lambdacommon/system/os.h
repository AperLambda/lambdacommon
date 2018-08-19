/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_OS_H
#define LAMBDACOMMON_OS_H

#include "../lambdacommon.h"

namespace lambdacommon
{
	namespace system
	{
		/*! @brief Enumeration of different Operating system.
		 *
		 * Enum of different Operating system. Not complete.
		 */
		enum OS
		{
			WINDOWS_32 = 1,
			WINDOWS_64 = 2,
			MAC_OSX = 3,
			LINUX = 5,
			FREEBSD = 6,
			NETBSD = 7,
			OPENBSD = 8,
			CYGWIN = 11,
			OS_UNKNOWN = 0
		};

		namespace os
		{
			/*! @brief Gets the Operating system used.
			 *
			 * This function returns the Operating system used as an enum value of the enum OS.
			 *
			 * @return The OS used.
			 */
			extern OS LAMBDACOMMON_API getOS();

			extern std::string LAMBDACOMMON_API getOSName(OS os = getOS());
		}
	}
}

#endif //LAMBDACOMMON_OS_H