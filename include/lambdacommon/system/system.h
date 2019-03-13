/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_SYSTEM_H
#define LAMBDACOMMON_SYSTEM_H

#include "fs.h"
#include "devices.h"
#include "os.h"
#include "terminal.h"
#include "time.h"
#include "../lstring.h"

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
            ARMv7,
            ARMv8_32,
            ARMv8_64,
            I386,
            RISCV32,
            RISCV64,
            X86_64,
            UNKNOWN
        };

        bool is_arch_from_arm_family(SysArchitecture arch);

        /*
         * Hardware
         */

        /*!
         * Returns the name of the processor (CPU) as a string.
         * @return The CPU's name.
         */
        extern std::string LAMBDACOMMON_API get_cpu_name();

        extern SysArchitecture LAMBDACOMMON_API get_processor_arch();

        extern std::string LAMBDACOMMON_API get_processor_arch_enum_str(SysArchitecture arch = get_processor_arch());

        extern std::string LAMBDACOMMON_API get_processor_arch_str();

        /*!
         * Returns the count of the processor (CPU) cores as an int.
         * @return The CPU cores' count
         */
        extern uint32_t LAMBDACOMMON_API get_cpu_cores();

        /*!
         * Returns the size of the physical memory (RAM) in bytes.
         * @return The physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API get_memory_total();

        /*!
         * Returns the size of the available physical memory (RAM) in bytes.
         * @return The available physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API get_memory_available();

        /*!
         * Returns the size of the used physical memory (RAM) in bytes.
         * @return The used physical memory in bytes.
         */
        extern uint64_t LAMBDACOMMON_API get_memory_used();

        /*
         * Computer
         */

        /*!
         * Returns the computer's name as a string.
         * @return The computer's name.
         */
        extern std::string LAMBDACOMMON_API get_host_name();

        /*!
         * Returns the OS' name at runtime.
         * @return The OS' name at runtime.
         */
        extern std::string LAMBDACOMMON_API get_os_name();

        /*!
         * Returns the Kernel' version at runtime.
         * @return The Kernel' version at runtime.
         */
        extern std::string LAMBDACOMMON_API get_kernel_version();

        /*
         * User
         */

        /*!
         * Gets the user name of the user running the program.
         * @return The user name.
         */
        extern std::string LAMBDACOMMON_API get_user_name();

        extern std::string LAMBDACOMMON_API get_user_directory_str();

        extern fs::path LAMBDACOMMON_API get_user_directory();

        /*
         * Others
         */

        /*!
         * Checks whether the current process is running as root.
         * @return True if the the current process is running as root, else false.
         */
        extern bool LAMBDACOMMON_API is_root();

        /*!
         * Opens the specified URI with the default application assigned to.
         * @param uri The URI to open.
         */
        extern void LAMBDACOMMON_API open_uri(const std::string &uri);

        /*!
         * Gets the message of the specified error code.
         * If 0 is given as a parameter get the latest error code and returns its message.
         * @param error The error code.
         * @return The message of the error.
         */
        extern std::string LAMBDACOMMON_API get_error_message(int error = 0);

        /*!
         * Sleeps the current thread for the specified time in milliseconds.
         * @param time Time to wait in milliseconds.
         */
        extern void LAMBDACOMMON_API sleep(utime_t time);
    }
}

#endif //LAMBDACOMMON_SYSTEM_H
