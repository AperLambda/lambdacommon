/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include <lambdacommon/system/system.h>

namespace sys = lambdacommon::system;
namespace term = lambdacommon::terminal;

int main(int argc, char **argv)
{
    term::setup();
    if (argc > 1) {
        term::set_title("λcommon_info - Help");
        std::cout << "> " << term::YELLOW << "lambdacommon_info" << "\n";
        std::cout << "\n";
        std::cout << term::CYAN << "  lambdacommon_info" << term::RESET << ", a software to get information about " << term::CYAN << "lambdacommon" << term::RESET << ".\n";
        std::cout << "\n" << std::endl;
        return EXIT_FAILURE;
    }
    term::set_title("λcommon_info");
    std::cout << "Now running " << term::CYAN;
    if (term::has_utf8()) std::cout << "λcommon"; else std::cout << "lambdacommon";
    std::cout << term::RESET << " v" << term::MAGENTA << lambdacommon::get_version() << term::RESET;
    std::cout << " on " << term::YELLOW << sys::get_os_name() << term::RESET << " (arch: " << term::YELLOW << sys::get_processor_arch_str() << term::RESET << ")." << std::endl;
    return EXIT_SUCCESS;
}
