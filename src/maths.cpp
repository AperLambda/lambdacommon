/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/maths.h"

namespace lambdacommon
{
	namespace maths
	{
		/*
		 * Trigonometric functions
		 */

#define RADIANS(TYPE, VALUE) VALUE * (static_cast<TYPE>(LCOMMON_PI) / static_cast<TYPE>(180.0))
#define DEGREES(TYPE, VALUE) VALUE * (static_cast<TYPE>(180.0) / static_cast<TYPE>(LCOMMON_PI))

		double LAMBDACOMMON_API radians(double degrees)
		{
			return RADIANS(double, degrees);
		}

		float LAMBDACOMMON_API radians(float degrees)
		{
			return RADIANS(float, degrees);
		}

		double LAMBDACOMMON_API degrees(double radians)
		{
			return DEGREES(double, radians);
		}

		float LAMBDACOMMON_API degrees(float radians)
		{
			return DEGREES(float, radians);
		}
	}
}