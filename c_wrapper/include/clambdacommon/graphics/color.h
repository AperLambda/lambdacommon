/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_COLOR_H
#define CLAMBDACOMMON_COLOR_H

#include "../clambdacommon.h"
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lcColor lcColor;

lcColor *lc_newColor(float red, float green, float blue, float alpha);

void lc_deleteColor(lcColor *color);

float lc_Color_getRed(const lcColor *color);

float lc_Color_getGreen(const lcColor *color);

float lc_Color_getBlue(const lcColor *color);

float lc_Color_getAlpha(const lcColor *color);

uint8_t lc_Color_getRedAsInt(const lcColor *color);

uint8_t lc_Color_getGreenAsInt(const lcColor *color);

uint8_t lc_Color_getBlueAsInt(const lcColor *color);

uint8_t lc_Color_getAlphaAsInt(const lcColor *color);

uint64_t lc_Color_toHex(const lcColor *color);

const char *lc_Color_toString(const lcColor *color, bool hex);

bool lc_Color_equals(const lcColor *color, const lcColor *other);

lcColor *lc_color_blend(const lcColor *bg, const lcColor *fg);

lcColor *lc_color_mix(const lcColor *a, const lcColor *b, float ratio);

lcColor *lc_color_fromHex(uint64_t hexColor, bool hasAlpha);

lcColor *lc_color_fromHexStr(const char *hexColor);

lcColor *lc_color_fromIntRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_COLOR_H