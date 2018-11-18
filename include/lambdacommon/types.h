/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_TYPES_H
#define LAMBDACOMMON_TYPES_H

#include "sizes.h"

namespace lambdacommon
{
	/*
	 * Sizes
	 */
	typedef Size2D<uint16_t> Size2D_u16;
	typedef Size2D<int16_t> Size2D_i16;
	typedef Size2D<uint32_t> Size2D_u32;
	typedef Size2D<int32_t> Size2D_i32;

	typedef Size3D<uint16_t> Size3D_u16;
	typedef Size3D<int16_t> Size3D_i16;
	typedef Size3D<uint32_t> Size3D_u32;
	typedef Size3D<int32_t> Size3D_i32;

	/*
	 * Network
	 */
	typedef uint16_t port_t;

	/*
	 * Time
	 */
	typedef uint64_t utime_t;

	/*
	 * Dumb types
	 */
	typedef std::string *pstring;
}

#endif //LAMBDACOMMON_TYPES_H