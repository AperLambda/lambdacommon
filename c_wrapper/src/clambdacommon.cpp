/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/clambdacommon/clambdacommon.h"
#include <lambdacommon/lambdacommon.h>
#include <cstring>

const char* lcommon_getVersion()
{
	static char version[10];
	lambdacommon::getVersion().copy(version, 10);
	return version;
}