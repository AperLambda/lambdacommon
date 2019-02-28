/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/object.h"
#include "../include/lambdacommon/lstring.h"

namespace lambdacommon
{
    std::wstring Object::to_wstring() const
    {
        return std::move(lstring::from_utf8_to_wstring(this->to_string()));
    }
}
