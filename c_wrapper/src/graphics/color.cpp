/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/clambdacommon/graphics/color.h"
#include <lambdacommon/graphics/color.h>

#define COLOR_CPP(COLOR) reinterpret_cast<lambdacommon::Color *>(COLOR)
#define CONST_COLOR_CPP(COLOR) reinterpret_cast<const lambdacommon::Color *>(COLOR)

lcColor *lc_new_Color(float red, float green, float blue, float alpha)
{
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(red, green, blue, alpha));
}

void lc_delete_Color(lcColor *color)
{
	delete COLOR_CPP(color);
}

float lc_Color_get_red(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->red();
}

float lc_Color_get_green(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->green();
}

float lc_Color_get_blue(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->blue();
}

float lc_Color_get_alpha(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->alpha();
}

uint8_t lc_Color_get_red_as_int(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->red_as_int();
}

uint8_t lc_Color_get_green_as_int(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->green_as_int();
}

uint8_t lc_Color_get_blue_as_int(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->blue_as_int();
}

uint8_t lc_Color_get_alpha_as_int(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->alpha_as_int();
}

const char *lc_Color_to_string(const lcColor *color, bool hex)
{
	if (hex) {
		static char to_string[9];
		CONST_COLOR_CPP(color)->to_string(hex).copy(to_string, 9);
		return to_string;
	} else {
		static char to_string[24];
		CONST_COLOR_CPP(color)->to_string(hex).copy(to_string, 24);
		return to_string;
	}
}

bool lc_Color_equals(const lcColor *color, const lcColor *other)
{
	return *CONST_COLOR_CPP(color) == *CONST_COLOR_CPP(other);
}

lcColor *lc_color_blend(const lcColor *bg, const lcColor *fg)
{
	auto color = lambdacommon::color::blend(*CONST_COLOR_CPP(bg), *CONST_COLOR_CPP(fg));
	// Pass to heap and cast.
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(color));
}

lcColor *lc_color_mix(const lcColor *a, const lcColor *b, float ratio)
{
	auto color = lambdacommon::color::mix(*CONST_COLOR_CPP(a), *CONST_COLOR_CPP(b), ratio);
	// Pass to heap and cast.
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(color));
}

lcColor *lc_color_from_hex(uint64_t hex_color, bool has_alpha)
{
	auto color = lambdacommon::color::from_hex(hex_color, has_alpha);
	// Pass to heap and cast.
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(color));
}

lcColor *lc_color_from_hex_str(const char *hex_color)
{
	auto color = lambdacommon::color::from_hex({hex_color});
	// Pass to heap and cast.
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(color));
}

lcColor *lc_color_from_int_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	auto color = lambdacommon::color::from_int_rgba(red, green, blue, alpha);
	// Pass to heap and cast.
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(color));
}