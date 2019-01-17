/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/clambdacommon/maths.h"
#include <lambdacommon/maths.h>

double lc_maths_radians(double degrees)
{
	return lambdacommon::maths::radians(degrees);
}

float lc_maths_fradians(float degrees)
{
	return lambdacommon::maths::radians(degrees);
}

double lc_maths_degrees(double radians)
{
	return lambdacommon::maths::degrees(radians);
}

float lc_maths_fdegrees(float radians)
{
	return lambdacommon::maths::degrees(radians);
}
