/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_SYSTEM_H
#define LAMBDACOMMON_SYSTEM_H

#include "filesystem/filesystem.h"
#include "devices.h"
#include "os.h"
#include "terminal.h"
#include "../string.h"

namespace lambdacommon
{
    namespace system
    {
        /*! @brief Enumeration of different Architectures of a CPU
         *
         * Enum of different CPU Architectures. Not complete.
         */
        enum SysArchitecture
        {
            ARM,
            ARM64,
            I386,
            X86_64,
            UNKNOWN
        };

        /*
         * Hardware
         */

        /*!
         * Returns the name of the processor (CPU) as a string.
         * @return The CPU's name.
         */
        extern std::string LAMBDACOMMON_API getProcessorName();

        extern SysArchitecture LAMBDACOMMON_API getProcessorArch();

        extern std::string LAMBDACOMMON_API getProcessorArchEnumStr(SysArchitecture arch = getProcessorArch());

        extern std::string LAMBDACOMMON_API getProcessorArchStr();

        /*!
         * Returns the count of the processor (CPU) cores as an int.
         * @return The CPU cores' count
         */
        extern uint32_t LAMBDACOMMON_API getProcessorCores();

        /*!
         * Returns the size of the physical memory (RAM) in bytes.
         * @return The physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API getPhysicalMemory();

        /*!
         * Returns the size of the available physical memory (RAM) in bytes.
         * @return The available physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API getPhysicalMemoryFree();

        /*!
         * Returns the size of the used physical memory (RAM) in bytes.
         * @return The used physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API getPhysicalMemoryUsed();

        /*
         * Computer
         */

        /*!
         * Returns the computer's name as a string.
         * @return The computer's name.
         */
        extern std::string LAMBDACOMMON_API getComputerName();

        /*!
         * Returns the OS' name at runtime.
         * @return The OS' name at runtime.
         */
        extern std::string LAMBDACOMMON_API getOSName();

        /*!
         * Returns the Kernel' version at runtime.
         * @return The Kernel' version at runtime.
         */
        extern std::string LAMBDACOMMON_API getKernelVersion();

        /*
         * User
         */

        extern std::string LAMBDACOMMON_API getUserName();

        extern std::string LAMBDACOMMON_API getUserDirectoryStr();

        extern filesystem::FilePath LAMBDACOMMON_API getUserDirectory();
    }
}

#endif //LAMBDACOMMON_SYSTEM_H