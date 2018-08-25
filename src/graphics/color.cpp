/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/graphics/color.h"
#include "../../include/lambdacommon/lstring.h"
#include "../../include/lambdacommon/maths.h"
#include <sstream>
#include <iomanip>

namespace lambdacommon
{
	Color::Color(float red, float green, float blue, float alpha) : _red(maths::clamp(red, 0.f, 1.f)),
	                                                                _green(maths::clamp(green, 0.f, 1.f)),
	                                                                _blue(maths::clamp(blue, 0.f, 1.f)),
	                                                                _alpha(maths::clamp(alpha, 0.f, 1.f))
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

	const Color Color::blend(const Color &bgColor) const
	{
		auto beta = bgColor.alpha() * (1 - _alpha);
		auto alpha = _alpha + beta;
		return Color((_red * _alpha + bgColor.red() * beta) / alpha, (_green * _alpha + bgColor.green() * beta) / alpha,
		             (_blue * _alpha + bgColor.blue() * beta) / alpha, alpha);
	}

	const Color Color::mix(const Color &b, float ratio) const
	{
		return color::mix(*this, b, ratio);
	}

	uint64_t Color::toHex() const
	{
		std::stringstream hexStr;
		hexStr << std::hex << std::setfill('0') << std::setw(2) << (int) redAsInt(); // Red
		hexStr << std::hex << std::setfill('0') << std::setw(2) << (int) greenAsInt(); // Green
		hexStr << std::hex << std::setfill('0') << std::setw(2) << (int) blueAsInt(); // Blue
		hexStr << std::hex << std::setfill('0') << std::setw(2) << (int) alphaAsInt(); // Alpha
		return static_cast<uint64_t>(lambdastring::parseLong(hexStr.str(), 16));
	}

	std::string Color::toString(bool hex) const
	{
		if (hex)
		{
			std::stringstream string;
			string << std::hex << std::right << std::setfill('0') << std::setw(8) << toHex();
			return "#" + lambdastring::toUpperCase(string.str());
		}
		return "rgba(" + std::to_string(redAsInt()) + ", " + std::to_string(greenAsInt()) + ", " +
		       std::to_string(blueAsInt()) + ", " + std::to_string(alphaAsInt()) + ")";
	}

	bool Color::operator==(const Color &other) const
	{
		return _red == other._red && _green == other._green && _blue == other._blue &&
		       _alpha == other._alpha;
	}

	bool Color::operator<(const Color &other) const
	{
		return std::tie(_red, _green, _blue, _alpha) < std::tie(other._red, other._green, other._blue, other._alpha);
	}

	const Color Color::operator+(const Color &other) const
	{
		return {_red + other._red, _green + other._green, _blue + other._blue, _alpha + other._alpha};
	}

	const Color Color::operator-(const Color &other) const
	{
		return {_red - other._red, _green - other._green, _blue - other._blue};
	}

	const Color Color::operator*(const Color &other) const
	{
		uint8_t red = maths::clamp(redAsInt() * other.redAsInt(), 0, 255);
		uint8_t green = maths::clamp(greenAsInt() * other.greenAsInt(), 0, 255);
		uint8_t blue = maths::clamp(blueAsInt() * other.blueAsInt(), 0, 255);
		uint8_t alpha = maths::clamp(alphaAsInt() * other.alphaAsInt(), 0, 255);
		return getColorByIntRGBA(red, green, blue, alpha);
	}

	const Color Color::operator*(float coefficient) const
	{
		return Color(_red * coefficient, _green * coefficient, _blue * coefficient, _alpha * coefficient);
	}

	Color &Color::operator+=(const Color &other)
	{
		auto newColor = operator+(other);
		_red = newColor._red;
		_green = newColor._green;
		_blue = newColor._blue;
		_alpha = newColor._alpha;
		return *this;
	}

	Color &Color::operator-=(const Color &other)
	{
		auto newColor = operator-(other);
		_red = newColor._red;
		_green = newColor._green;
		_blue = newColor._blue;
		_alpha = newColor._alpha;
		return *this;
	}

	Color &Color::operator*=(const Color &other)
	{
		auto newColor = operator*(other);
		_red = newColor._red;
		_green = newColor._green;
		_blue = newColor._blue;
		_alpha = newColor._alpha;
		return *this;
	}

	Color Color::COLOR_BLACK{0.f, 0.f, 0.f};

	Color Color::COLOR_WHITE{1.f, 1.f, 1.f};

	Color Color::COLOR_RED{1.f, 0.f, 0.f};

	Color Color::COLOR_GREEN{0.f, 1.f, 0.f};

	Color Color::COLOR_BLUE{0.f, 0.f, 1.f};

	namespace color
	{
		Color LAMBDACOMMON_API blend(const Color &fg, const Color &bg)
		{
			return fg.blend(bg);
		}

		Color LAMBDACOMMON_API mix(const Color &a, const Color &b, float ratio)
		{
			return {a.red() * (1 - ratio) + b.red() * ratio, a.green() * (1 - ratio) + b.green() * ratio,
			        a.blue() * (1 - ratio) + b.blue() * ratio, a.alpha() * (1 - ratio) + b.alpha() * ratio};
		}

		Color LAMBDACOMMON_API fromHex(uint64_t hexColor, bool hasAlpha)
		{
			std::stringstream string;
			string << std::hex << std::right << std::setfill('0');
			if (hasAlpha)
				string << std::setw(8);
			else
				string << std::setw(6);
			string << hexColor;
			return fromHex(string.str());
		}

		Color LAMBDACOMMON_API fromHex(const std::string &hexColor)
		{
			auto colorStr = lambdastring::replaceAll(hexColor, "#", "");
			if (colorStr.length() < 6 || colorStr.length() > 8)
				throw std::out_of_range("The hexadecimal color is invalid (Digits out of range).");
			auto redStr = colorStr.substr(0, 2);
			auto greenStr = colorStr.substr(2, 2);
			auto blueStr = colorStr.substr(4, 2);
			uint8_t alpha = 255;
			if (colorStr.length() > 6)
				alpha = static_cast<uint8_t>(lambdastring::parseInt(colorStr.substr(6, 2), 16));
			return getColorByIntRGBA(static_cast<uint8_t>(lambdastring::parseInt(redStr, 16)),
			                         static_cast<uint8_t>(lambdastring::parseInt(greenStr, 16)),
			                         static_cast<uint8_t>(lambdastring::parseInt(blueStr, 16)),
			                         alpha);
		}
	}

	Color getColorByIntRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		return {(red / 255.f), (green / 255.f), (blue / 255.f), (alpha / 255.f)};
	}
}