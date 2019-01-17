/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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
#include <tuple>

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

	uint8_t Color::red_as_int() const
	{
		return static_cast<uint8_t>(_red * 255);
	}

	uint8_t Color::green_as_int() const
	{
		return static_cast<uint8_t>(_green * 255);
	}

	uint8_t Color::blue_as_int() const
	{
		return static_cast<uint8_t>(_blue * 255);
	}

	uint8_t Color::alpha_as_int() const
	{
		return static_cast<uint8_t>(_alpha * 255);
	}

	const Color Color::blend(const Color &bg_color) const
	{
		auto beta = bg_color.alpha() * (1 - _alpha);
		auto alpha = _alpha + beta;
		return Color((_red * _alpha + bg_color.red() * beta) / alpha,
					 (_green * _alpha + bg_color.green() * beta) / alpha,
					 (_blue * _alpha + bg_color.blue() * beta) / alpha,
					 alpha);
	}

	const Color Color::mix(const Color &b, float ratio) const
	{
		return color::mix(*this, b, ratio);
	}

	/*uint64_t Color::to_hex() const
	{
		auto hex_str = lstring::replace_all(to_string(true), "#", "");
		return static_cast<uint64_t>(lstring::parse_long(hex_str, 16));
	}*/

	std::string Color::to_string(bool hex) const
	{
		if (hex) {
			std::stringstream string;
			string << std::hex << std::setfill('0') << std::setw(2) << (int) red_as_int(); // Red
			string << std::hex << std::setfill('0') << std::setw(2) << (int) green_as_int(); // Green
			string << std::hex << std::setfill('0') << std::setw(2) << (int) blue_as_int(); // Blue
			string << std::hex << std::setfill('0') << std::setw(2) << (int) alpha_as_int(); // Alpha
			return "#" + lstring::to_upper_case(string.str());
		}
		return "rgba(" + std::to_string(red_as_int()) + ", " + std::to_string(green_as_int()) + ", " + std::to_string(blue_as_int()) + ", " + std::to_string(alpha_as_int()) + ")";
	}

	std::string Color::to_string() const
	{
		return to_string(true);
	}

	bool Color::operator==(const Color &other) const
	{
		return red_as_int() == other.red_as_int() && green_as_int() == other.green_as_int() && blue_as_int() == other.blue_as_int() && alpha_as_int() == other.alpha_as_int();
	}

	bool Color::operator<(const Color &other) const
	{
		return std::tie(_red, _green, _blue, _alpha) < std::tie(other._red, other._green, other._blue, other._alpha);
	}

	Color &Color::operator+=(const Color &other)
	{
		_red = maths::clamp(_red + other._red, 0.f, 1.f);
		_green = maths::clamp(_green + other._green, 0.f, 1.f);
		_blue = maths::clamp(_blue + other._blue, 0.f, 1.f);
		_alpha = maths::clamp(_alpha + other._alpha, 0.f, 1.f);
		return *this;
	}

	Color &Color::operator-=(const Color &other)
	{
		_red = maths::clamp(_red - other._red, 0.f, 1.f);
		_green = maths::clamp(_green - other._green, 0.f, 1.f);
		_blue = maths::clamp(_blue - other._blue, 0.f, 1.f);
		//_alpha = maths::clamp(_alpha - other._alpha, 0.f, 1.f); We don't change the alpha, because if we remove the opacity it doesn't have any sense.
		return *this;
	}

	Color &Color::operator*=(const Color &other)
	{
		_red = maths::clamp(red_as_int() * other.red_as_int(), 0, 255) / 255.f;
		_green = maths::clamp(green_as_int() * other.green_as_int(), 0, 255) / 255.f;
		_blue = maths::clamp(blue_as_int() * other.blue_as_int(), 0, 255) / 255.f;
		_alpha = maths::clamp(alpha_as_int() * other.alpha_as_int(), 0, 255) / 255.f;
		return *this;
	}

	Color &Color::operator*=(float coefficient)
	{
		_red = maths::clamp(_red * coefficient, 0.f, 1.f);
		_green = maths::clamp(_green * coefficient, 0.f, 1.f);
		_blue = maths::clamp(_blue * coefficient, 0.f, 1.f);
		_alpha = maths::clamp(_alpha * coefficient, 0.f, 1.f);
		return *this;
	}

	const Color operator+(Color self, const Color &other)
	{
		self += other;
		return self;
	}

	const Color operator-(Color self, const Color &other)
	{
		self -= other;
		return self;
	}

	const Color operator*(Color self, const Color &other)
	{
		self *= other;
		return self;
	}

	const Color operator*(Color self, float coefficient)
	{
		self *= coefficient;
		return self;
	}

	Color Color::COLOR_BLACK{0.f, 0.f, 0.f};

	Color Color::COLOR_WHITE{1.f, 1.f, 1.f};

	Color Color::COLOR_RED{1.f, 0.f, 0.f};

	Color Color::COLOR_GREEN{0.f, 1.f, 0.f};

	Color Color::COLOR_BLUE{0.f, 0.f, 1.f};

	namespace color
	{
		Color LAMBDACOMMON_API blend(const Color &bg, const Color &fg)
		{
			return fg.blend(bg);
		}

		Color LAMBDACOMMON_API mix(const Color &a, const Color &b, float ratio)
		{
			return {a.red() * (1 - ratio) + b.red() * ratio, a.green() * (1 - ratio) + b.green() * ratio,
					a.blue() * (1 - ratio) + b.blue() * ratio, a.alpha() * (1 - ratio) + b.alpha() * ratio};
		}

		Color LAMBDACOMMON_API from_hex(uint64_t hex_color, bool has_alpha)
		{
			std::stringstream string;
			string << std::hex << std::right << std::setfill('0');
			if (has_alpha)
				string << std::setw(8);
			else
				string << std::setw(6);
			string << hex_color;
			return from_hex(string.str());
		}

		Color LAMBDACOMMON_API from_hex(const std::string &hex_color)
		{
			auto color_str = lstring::replace_all(lstring::replace_all(hex_color, "#", ""), "0x", "");
			if (color_str.length() < 6 || color_str.length() > 8)
				throw std::out_of_range("The hexadecimal color is invalid (Digits out of range).");
			auto red_str = color_str.substr(0, 2);
			auto green_str = color_str.substr(2, 2);
			auto blue_str = color_str.substr(4, 2);
			uint8_t alpha = 255;
			if (color_str.length() > 6)
				alpha = static_cast<uint8_t>(lstring::parse_int(color_str.substr(6, 2), 16).value_or(0));
			return color::from_int_rgba(static_cast<uint8_t>(lstring::parse_int(red_str, 16).value_or(0)),
										static_cast<uint8_t>(lstring::parse_int(green_str, 16).value_or(0)),
										static_cast<uint8_t>(lstring::parse_int(blue_str, 16).value_or(0)),
										alpha);
		}

		Color LAMBDACOMMON_API from_int_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			return {(red / 255.f), (green / 255.f), (blue / 255.f), (alpha / 255.f)};
		}
	}
}
