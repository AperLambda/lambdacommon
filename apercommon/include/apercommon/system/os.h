/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef APERCOMMON_OS_H
#define APERCOMMON_OS_H

#include "../apercommon.h"

namespace apercommon
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
        extern OS APERCOMMON_EXPORTS getOS();

        extern std::string APERCOMMON_EXPORTS getOSName(OS os);

        extern OSArchitecture APERCOMMON_EXPORTS getOSArch();

        extern std::string APERCOMMON_EXPORTS getArchName(OSArchitecture arch);
    }
}

#endif //APERCOMMON_OS_H
