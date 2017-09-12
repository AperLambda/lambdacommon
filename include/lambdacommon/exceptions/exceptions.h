/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
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

namespace lambdacommon
{
    class IllegalArgumentException : public std::exception
    {
    private:
        std::string _msg;

    public:
        IllegalArgumentException(std::string msg) : _msg(msg)
        {}

        const char *what() const noexcept override
        {
            return _msg.c_str();
        }
    };

    class ParseException : public std::exception
    {
    private:
        std::string _msg;

    public:
        ParseException(std::string msg) : _msg(msg)
        {}

        const char *what() const noexcept override
        {
            return _msg.c_str();
        }
    };
}

#endif //LAMBDACOMMON_EXCEPTIONS_H