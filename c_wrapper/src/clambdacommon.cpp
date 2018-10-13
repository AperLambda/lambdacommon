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

const char* lc_getVersion()
{
	static char version[10];
	lambdacommon::getVersion().copy(version, 10);
	return version;
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-incomplete"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-incomplete"
#endif
void lc_deletePointer(void **pointer)
{
	delete *pointer;
	*pointer = nullptr;
}
#ifdef __clang__
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif