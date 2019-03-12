/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_TYPES_H
#define LAMBDACOMMON_TYPES_H

#include "sizes.h"
#include "maths/geometry/point.h"
#include <chrono>

namespace lambdacommon
{
    /*
     * Sizes
     */
    typedef Size2D<uint16_t> Size2D_u16;
    typedef Size2D<int16_t>  Size2D_i16;
    typedef Size2D<uint32_t> Size2D_u32;
    typedef Size2D<int32_t>  Size2D_i32;

    typedef Size3D<uint16_t> Size3D_u16;
    typedef Size3D<int16_t>  Size3D_i16;
    typedef Size3D<uint32_t> Size3D_u32;
    typedef Size3D<int32_t>  Size3D_i32;

    /*
     * Points
     */
    typedef Point2D<uint16_t> Point2D_u16;
    typedef Point2D<int16_t>  Point2D_i16;
    typedef Point2D<uint32_t> Point2D_u32;
    typedef Point2D<int32_t>  Point2D_i32;

    /*
     * Network
     */
    typedef uint16_t port_t;

    /*
     * Time
     */
    typedef uint64_t utime_t;
#if __cplusplus > 201703L
    using file_time_type = std::chrono::time_point<std::chrono::file_clock>;
#else
    using file_time_type = std::chrono::system_clock::time_point;
#endif

    /*
     * Dumb types
     */
    typedef std::string *pstring;
}

#endif //LAMBDACOMMON_TYPES_H
