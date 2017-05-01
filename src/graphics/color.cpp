/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/apercommon/graphics/color.h"

#define MIN(A, B)    ((A) < (B) ? (A) : (B))
#define MAX(A, B)    ((A) > (B) ? (A) : (B))
#define CLAMP(X, min, max)    MIN(MAX(X, min), max)

namespace apercommon
{
    Color::Color(float red, float green, float blue, float alpha) : _red(CLAMP(red, 0.f, 1.f)),
                                                                    _green(CLAMP(green, 0.f, 1.f)),
                                                                    _blue(CLAMP(blue, 0.f, 1.f)),
                                                                    _alpha(CLAMP(alpha, 0.f, 1.f))
    {}

    float Color::red() const
    {
        return _red;
    }

    float Color::green() const
    {
        return _green;
    }

    float Color::blue() const
    {
        return _blue;
    }

    float Color::alpha() const
    {
        return _alpha;
    }

    uint8_t Color::redAsInt() const
    {
        return static_cast<uint8_t>(_red * 255);
    }

    uint8_t Color::greenAsInt() const
    {
        return static_cast<uint8_t>(_green * 255);
    }

    uint8_t Color::blueAsInt() const
    {
        return static_cast<uint8_t>(_blue * 255);
    }

    uint8_t Color::alphaAsInt() const
    {
        return static_cast<uint8_t>(_alpha * 255);
    }

    Color getColorByIntRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    {
        return Color((red / 255.f), (green / 255.f), (blue / 255.f), (alpha / 255.f));
    }
}