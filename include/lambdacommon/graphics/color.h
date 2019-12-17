/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_COLOR_H
#define LAMBDACOMMON_COLOR_H

#include "../types.h"
#include <utility>

namespace lambdacommon
{
    using namespace std::rel_ops;

    class LAMBDACOMMON_API Color : public Object
    {
    private:
        f32 _red;
        f32 _green;
        f32 _blue;
        f32 _alpha;

    public:
        Color(f32 red, f32 green, f32 blue, f32 alpha = 1.f);

        /*! @brief Gets the red value of the color.
         *
         * This function returns the red value (between 0 and 1) of the color.
         *
         * @return The red value (between 0 and 1).
         */
        f32 red() const;

        /*! @brief Gets the green value of the color.
         *
         * This function returns the green value (between 0 and 1) of the color.
         *
         * @return The green value (between 0 and 1).
         */
        f32 green() const;

        /*! @brief Gets the blue value of the color.
         *
         * This function returns the red value (between 0 and 1) of the color.
         *
         * @return The blue value (between 0 and 1).
         */
        f32 blue() const;

        /*! @brief Gets the alpha value of the color.
         *
         * This function returns the alpha value (between 0 and 1) of the color.
         *
         * @return The alpha value (between 0 and 1).
         */
        f32 alpha() const;

        /*! @brief Gets the red value of the color.
         *
         * This function returns the red value (between 0 and 255) of the color.
         *
         * @return The red value (between 0 and 255).
         */
        u8 red_as_int() const;

        /*! @brief Gets the green value of the color.
         *
         * This function returns the green value (between 0 and 255) of the color.
         *
         * @return The green value (between 0 and 255).
         */
        u8 green_as_int() const;

        /*! @brief Gets the blue value of the color.
         *
         * This function returns the red value (between 0 and 255) of the color.
         *
         * @return The blue value (between 0 and 255).
         */
        u8 blue_as_int() const;

        /*! @brief Gets the alpha value of the color.
         *
         * This function returns the alpha value (between 0 and 255) of the color.
         *
         * @return The alpha value (between 0 and 255).
         */
        u8 alpha_as_int() const;

        /*!
         * Blends this color with a background color.
         * @param bg_color The background color.
         * @return The blended color.
         */
        const Color blend(const Color& bg_color) const;

        /*!
         * Mixes this color with another color.
         * @param b The other color.
         * @param ratio The mix ratio.
         * @return The mixed color.
         */
        const Color mix(const Color& b, f32 ratio) const;

        /*!
         * Gets the color as an hexadecimal color.
         * @return The hexadecimal color.
         */
        //u64 to_hex() const;

        /*!
         * Gets the color as a string.
         * @return The color as a string.
         */
        std::string to_string(bool hex) const;

        std::string to_string() const override;

        template<std::size_t N>
        decltype(auto) get() const {
            if constexpr (N == 0) return this->_red;
            else if constexpr (N == 1) return this->_green;
            else if constexpr (N == 2) return this->_blue;
            else if constexpr (N == 3) return this->_alpha;
        }

        bool operator==(const Color& other) const;

        bool operator<(const Color& other) const;

        Color& operator+=(const Color& other);

        Color& operator-=(const Color& other);

        Color& operator*=(const Color& other);

        Color& operator*=(f32 coefficient);

        /*!
         * Adds the specified color to the current color.
         * @param self The current color.
         * @param other The color to add.
         * @return The added color.
         */
        friend const Color operator+(Color self, const Color& other);

        /*!
         * Subtracts the specified color to the current color. Alpha is not subtracted.
         * @param other The color to subtract.
         * @return The subtracted color.
         */
        friend const Color operator-(Color self, const Color& other);

        /*!
         * Multiplies the specified color to the current color.
         * @param other The color to multiply.
         * @return The multiplied color.
         */
        friend const Color operator*(Color self, const Color& other);

        /*!
         * Multiplies the current color with a specified coefficient.
         * @param coefficient The multiplication coefficient.
         * @return The multiplied color.
         */
        friend const Color operator*(Color self, f32 coefficient);

        /**
         * Represents the black color.
         */
        static Color COLOR_BLACK;

        /**
         * Represents the white color.
         */
        static Color COLOR_WHITE;

        /**
         * Represents the red color.
         */
        static Color COLOR_RED;

        /**
         * Represents the green color.
         */
        static Color COLOR_GREEN;

        /**
         * Represents the blue color.
         */
        static Color COLOR_BLUE;
    };

    namespace color
    {
        /*!
         * Blends two colors.
         * @param bg The background color.
         * @param fg The foreground color.
         * @return The blended color.
         */
        extern Color LAMBDACOMMON_API blend(const Color& bg, const Color& fg);

        /*!
         * Mixes two different colors with a ratio.
         * @param a The first color to mix.
         * @param b The second color to mix.
         * @param ratio The mix ratio.
         * @return The mixed color.
         */
        extern Color LAMBDACOMMON_API mix(const Color& a, const Color& b, f32 ratio);

        /*!
         * Makes a new Color instance from the given hexadecimal color value.
         * @param hex_color The hexadecimal color value.
         * @param has_alpha True if the hexadecimal color value includes the alpha channel, else false.
         * @return A new Color instance.
         */
        extern Color LAMBDACOMMON_API from_hex(u64 hex_color, bool has_alpha = true);

        /*!
         * Makes a new Color instance from the given hexadecimal color value string.
         * @param hex_color The hexadecimal color value as a string.
         * @return A new Color instance.
         */
        extern Color LAMBDACOMMON_API from_hex(const std::string& hex_color);

        /*!
         * Makes a new Color instance from a RGB value.
         * @param red Red channel.
         * @param green Green channel.
         * @param blue Blue channel.
         * @param alpha Alpha channel.
         * @return A new Color instance.
         */
        extern Color LAMBDACOMMON_API from_int_rgba(u8 red, u8 green, u8 blue, u8 alpha = 255);
    }
}

// Structured bindings for lambdacommon::Color.
namespace std
{
    template<>
    struct tuple_size<lambdacommon::Color> : std::integral_constant<std::size_t, 4>
    {
    };

    template<std::size_t N>
    struct tuple_element<N, lambdacommon::Color>
    {
        using type = decltype(std::declval<lambdacommon::Color>().get<N>());
    };
}

#endif //LAMBDACOMMON_COLOR_H
