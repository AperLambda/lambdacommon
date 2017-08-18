/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
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

namespace lambdacommon
{
    namespace system
    {
        /*
         * Hardware
         */

        /*!
         * Returns the size of the physical memory (RAM) in bytes.
         * @return The physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API getPhysicalMemory();

        /*!
         * Returns the size of the available physical memory (RAM) in bytes.
         * @return The available physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API getAvailablePhysicalMemory();

        /*!
         * Returns the size of the used physical memory (RAM) in bytes.
         * @return The used physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API getUsedPhysicalMemory();

        /*
         * Computer
         */

        /*!
         * Returns the computer's name as a string.
         * @return The computer's name.
         */
        extern std::string LAMBDACOMMON_API getComputerName();

        /*
         * User
         */

        extern std::string LAMBDACOMMON_API getUserName();

        extern std::string LAMBDACOMMON_API getUserDirectoryStr();

        extern filesystem::FilePath LAMBDACOMMON_API getUserDirectory();
    }
}

#endif //LAMBDACOMMON_SYSTEM_H