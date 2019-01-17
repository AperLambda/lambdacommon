/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_TIME_H
#define LAMBDACOMMON_TIME_H

#include "../lambdacommon.h"
#include <chrono>

namespace lambdacommon
{
	namespace time
	{
		/*!
		 * Gets the current time in milliseconds.
		 * @return The difference, measured in milliseconds, between the current time and midnight, January 1, 1970 UTC.
		 */
		extern time_t LAMBDACOMMON_API get_time_millis();
	}
}

#endif //LAMBDACOMMON_TIME_H
