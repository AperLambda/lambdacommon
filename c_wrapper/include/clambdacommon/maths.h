/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_MATHS_H
#define CLAMBDACOMMON_MATHS_H

#define LCOMMON_PI 3.14159265359
#define LCOMMON_TAU 6.28318530718

// Define every functions that include templates as preprocessor defines.
#define lc_maths_abs(number) (number < 0 ? -(number) : number)
#define lc_maths_min(a, b) (a < b ? a : b)
#define lc_maths_max(a, b) (a > b ? a : b)
#define lc_maths_clamp(number, min, max) lc_maths_min(lc_maths_max(number, min), max) //maths::min(maths::max(number, min), max)

#ifdef __cplusplus
extern "C"
{
#endif

double lc_maths_radians(double degrees);

float lc_maths_fradians(float degrees);

double lc_maths_degrees(double radians);

float lc_maths_fdegrees(float radians);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_MATHS_H
