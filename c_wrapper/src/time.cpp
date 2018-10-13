/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/clambdacommon/system/time.h"
#include <lambdacommon/system/time.h>

time_t lc_time_getTimeMillis()
{
	return lambdacommon::time::getTimeMillis();
}