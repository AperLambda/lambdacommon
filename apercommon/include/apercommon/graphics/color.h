/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef APERCOMMON_COLOR_H
#define APERCOMMON_COLOR_H

#include "../apercommon.h"

namespace apercommon
{
    class APERCOMMON_EXPORTS Color
    {
    private:
        float _red;
        float _green;
        float _blue;
        float _alpha;

    public:
        Color(float red, float green, float blue, float alpha = 1.f);

        /*! @brief Gets the red value of the color.
         *
         * This function returns the red value (between 0 and 1) of the color.
         *
         * @return The red value (between 0 and 1).
         */
        float red() const;

        /*! @brief Gets the green value of the color.
         *
         * This function returns the green value (between 0 and 1) of the color.
         *
         * @return The green value (between 0 and 1).
         */
        float green() const;

        /*! @brief Gets the blue value of the color.
         *
         * This function returns the red value (between 0 and 1) of the color.
         *
         * @return The blue value (between 0 and 1).
         */
        float blue() const;

        /*! @brief Gets the alpha value of the color.
         *
         * This function returns the alpha value (between 0 and 1) of the color.
         *
         * @return The alpha value (between 0 and 1).
         */
        float alpha() const;

        /*! @brief Gets the red value of the color.
         *
         * This function returns the red value (between 0 and 255) of the color.
         *
         * @return The red value (between 0 and 255).
         */
        uint8_t redAsInt() const;

        /*! @brief Gets the green value of the color.
         *
         * This function returns the green value (between 0 and 255) of the color.
         *
         * @return The green value (between 0 and 255).
         */
        uint8_t greenAsInt() const;

        /*! @brief Gets the blue value of the color.
         *
         * This function returns the red value (between 0 and 255) of the color.
         *
         * @return The blue value (between 0 and 255).
         */
        uint8_t blueAsInt() const;

        /*! @brief Gets the alpha value of the color.
         *
         * This function returns the alpha value (between 0 and 255) of the color.
         *
         * @return The alpha value (between 0 and 255).
         */
        uint8_t alphaAsInt() const;
    };

    extern Color APERCOMMON_EXPORTS getColorByIntRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
}

#endif //APERCOMMON_COLOR_H
