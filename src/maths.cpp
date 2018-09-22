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

		double radians(double degrees)
		{
			return RADIANS(double, degrees);
		}

		float radians(float degrees)
		{
			return RADIANS(float, degrees);
		}

		double degrees(double radians)
		{
			return DEGREES(double, radians);
		}

		float degrees(float radians)
		{
			return DEGREES(float, radians);
		}
	}
}