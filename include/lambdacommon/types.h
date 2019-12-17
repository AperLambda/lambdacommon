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
     * Integers
     */
    typedef int8_t   i8;
    typedef int16_t  i16;
    typedef int32_t  i32;
    typedef int64_t  i64;
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    /*
     * Floats
     */
    typedef float  f32;
    typedef double f64;

    /*
     * Sizes
     */
    typedef Size2D<u16> Size2D_u16;
    typedef Size2D<i16> Size2D_i16;
    typedef Size2D<u32> Size2D_u32;
    typedef Size2D<i32> Size2D_i32;

    typedef Size3D<u16> Size3D_u16;
    typedef Size3D<i16> Size3D_i16;
    typedef Size3D<u32> Size3D_u32;
    typedef Size3D<i32> Size3D_i32;

    /*
     * Points
     */
    typedef Point2D<u16> Point2D_u16;
    typedef Point2D<i16> Point2D_i16;
    typedef Point2D<u32> Point2D_u32;
    typedef Point2D<i32> Point2D_i32;

    /*
     * Network
     */
    typedef u16 port_t;

    /*
     * Time
     */
    typedef u64 utime_t;
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
