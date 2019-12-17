/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_EXCEPTIONS_H
#define LAMBDACOMMON_EXCEPTIONS_H

#include <exception>
#include <string>
#include <utility>

namespace lambdacommon
{
    class ParseException : public std::exception
    {
    private:
        std::string _msg;

    public:
        explicit ParseException(std::string msg) : _msg(std::move(msg)) {}

        [[nodiscard]] const char* what() const noexcept override {
            return _msg.c_str();
        }
    };
}

#endif //LAMBDACOMMON_EXCEPTIONS_H
