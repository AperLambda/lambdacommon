/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
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
            UNKNOWN = 0
        };

        /*! @brief Enumeration of different Architectures of a CPU
         *
         * Enum of different CPU Architectures. Not complete.
         */
        enum OSArchitecture
        {
            ARM,
            ARM64,
            I386,
            X86_64
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

            extern std::string LAMBDACOMMON_API getOSName(OS os);

            extern OSArchitecture LAMBDACOMMON_API getOSArch();

            extern std::string LAMBDACOMMON_API getArchName(OSArchitecture arch);
        }
    }
}

#endif //LAMBDACOMMON_OS_H