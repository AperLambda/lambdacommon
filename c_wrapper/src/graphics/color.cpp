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

lcColor *lc_newColor(float red, float green, float blue, float alpha)
{
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(red, green, blue, alpha));
}

void lc_deleteColor(lcColor *color)
{
	delete COLOR_CPP(color);
}

float lc_Color_getRed(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->red();
}

float lc_Color_getGreen(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->green();
}

float lc_Color_getBlue(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->blue();
}

float lc_Color_getAlpha(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->alpha();
}

uint8_t lc_Color_getRedAsInt(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->redAsInt();
}

uint8_t lc_Color_getGreenAsInt(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->greenAsInt();
}

uint8_t lc_Color_getBlueAsInt(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->blueAsInt();
}

uint8_t lc_Color_getAlphaAsInt(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->alphaAsInt();
}

uint64_t lc_Color_toHex(const lcColor *color)
{
	return CONST_COLOR_CPP(color)->toHex();
}

const char *lc_Color_toString(const lcColor *color, bool hex)
{
	if (hex)
	{
		static char toString[9];
		CONST_COLOR_CPP(color)->toString(hex).copy(toString, 9);
		return toString;
	}
	else
	{
		static char toString[24];
		CONST_COLOR_CPP(color)->toString(hex).copy(toString, 24);
		return toString;
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

lcColor *lc_color_fromHex(uint64_t hexColor, bool hasAlpha)
{
	auto color = lambdacommon::color::fromHex(hexColor, hasAlpha);
	// Pass to heap and cast.
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(color));
}

lcColor *lc_color_fromHexStr(const char *hexColor)
{
	auto color = lambdacommon::color::fromHex({hexColor});
	// Pass to heap and cast.
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(color));
}

lcColor *lc_color_fromIntRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	auto color = lambdacommon::color::fromIntRGBA(red, green, blue, alpha);
	// Pass to heap and cast.
	return reinterpret_cast<lcColor *>(new lambdacommon::Color(color));
}