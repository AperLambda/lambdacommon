/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_LEXCEPTION_H
#define LAMBDACOMMON_LEXCEPTION_H

#include <exception>
#include <string>

namespace lambdacommon
{
    class unsupported_operation_error : public std::exception
    {
    private:
        std::string _msg;

    public:
        unsupported_operation_error(std::string message) : _msg(std::move(message)) {}

        unsupported_operation_error(const char* message) : _msg(message) {}

        const char* what() const noexcept override {
            return _msg.c_str();
        }
    };
}

#endif //LAMBDACOMMON_LEXCEPTION_H
