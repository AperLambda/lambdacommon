/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_MATHS_H
#define CLAMBDACOMMON_MATHS_H

#define LCOMMON_PI 3.14159265359

// Define every functions that include templates as preprocessor defines.
#define lcommon_maths_abs(number) (number < 0 ? -(number) : number)
#define lcommon_maths_min(a, b) (a < b ? a : b)
#define lcommon_maths_max(a, b) (a > b ? a : b)
#define lcommon_maths_clamp(number, min, max) lcommon_maths_min(lcommon_maths_max(number, min), max) //maths::min(maths::max(number, min), max)

#ifdef __cplusplus
extern "C"
{
#endif

double lcommon_maths_radians(double degrees);

float lcommon_maths_fradians(float degrees);

double lcommon_maths_degrees(double radians);

float lcommon_maths_fdegrees(float radians);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_MATHS_H