/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_COLOR_H
#define LAMBDACOMMON_COLOR_H

#include "../lambdacommon.h"
#include <utility>

namespace lambdacommon
{
	using namespace std::rel_ops;

	class LAMBDACOMMON_API Color
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

		/*!
		 * Blends this color with a background color.
		 * @param bgColor The background color.
		 * @return The blended color.
		 */
		const Color blend(const Color &bgColor) const;

		/*!
		 * Mixes this color with another color.
		 * @param b The other color.
		 * @param ratio The mix ratio.
		 * @return The mixed color.
		 */
		const Color mix(const Color &b, float ratio) const;

		/*!
		 * Gets the color as an hexadecimal color.
		 * @return The hexadecimal color.
		 */
		uint64_t toHex() const;

		/*!
		 * Gets the color as a string.
		 * @return The color as a string.
		 */
		std::string toString(bool hex = true) const;

		bool operator==(const Color &other) const;

		bool operator<(const Color &other) const;

		/*!
		 * Adds the specified color to the current color.
		 * @param other The color to add.
		 * @return The added color.
		 */
		const Color operator+(const Color &other) const;

		/*!
		 * Subtracts the specified color to the current color. Alpha is not subtracted.
		 * @param other The color to subtract.
		 * @return The subtracted color.
		 */
		const Color operator-(const Color &other) const;

		/*!
		 * Multiplies the specified color to the current color.
		 * @param other The color to multiply.
		 * @return The multiplied color.
		 */
		const Color operator*(const Color &other) const;

		/*!
		 * Multiplies the current color with a specified coefficient.
		 * @param coefficient The multiplication coefficient.
		 * @return The multiplied color.
		 */
		const Color operator*(float coefficient) const;

		Color &operator+=(const Color &other);

		Color &operator-=(const Color &other);

		Color &operator*=(const Color &other);

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
		extern Color LAMBDACOMMON_API blend(const Color &bg, const Color &fg);

		/*!
		 * Mixes two different colors with a ratio.
		 * @param a The first color to mix.
		 * @param b The second color to mix.
		 * @param ratio The mix ratio.
		 * @return The mixed color.
		 */
		extern Color LAMBDACOMMON_API mix(const Color &a, const Color &b, float ratio);

		extern Color LAMBDACOMMON_API fromHex(uint64_t hexColor, bool hasAlpha = true);

		extern Color LAMBDACOMMON_API fromHex(const std::string &hexColor);

		extern Color LAMBDACOMMON_API fromIntRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	}
}

#endif //LAMBDACOMMON_COLOR_H