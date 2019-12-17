/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/maths.h"

namespace lambdacommon::maths
{
    /*
     * Trigonometric functions
     */

#define RADIANS(TYPE, VALUE) VALUE * (static_cast<TYPE>(LCOMMON_PI) / static_cast<TYPE>(180.0))
#define DEGREES(TYPE, VALUE) VALUE * (static_cast<TYPE>(180.0) / static_cast<TYPE>(LCOMMON_PI))

    f64 LAMBDACOMMON_API radians(f64 degrees) {
        return RADIANS(f64, degrees);
    }

    f32 LAMBDACOMMON_API radians(f32 degrees) {
        return RADIANS(f32, degrees);
    }

    f64 LAMBDACOMMON_API degrees(f64 radians) {
        return DEGREES(f64, radians);
    }

    f32 LAMBDACOMMON_API degrees(f32 radians) {
        return DEGREES(f32, radians);
    }
}
