/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/serializable.h"

namespace lambdacommon
{
    namespace serializable
    {
        std::vector<std::string> LAMBDACOMMON_API tokenize(const std::string& _string, const std::string& delim) {
            std::string::size_type last_pos = 0;
            std::string::size_type pos = _string.find_first_of(delim, last_pos);
            std::vector<std::string> tokens;

            while (last_pos != std::string::npos) {
                if (pos != last_pos)
                    tokens.push_back(_string.substr(last_pos, pos - last_pos));
                last_pos = pos;
                if (last_pos == std::string::npos || last_pos + 1 == _string.length())
                    break;
                pos = _string.find_first_of(delim, ++last_pos);
            }

            return tokens;
        }
    }
}
