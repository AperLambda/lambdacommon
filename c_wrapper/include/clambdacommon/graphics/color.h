/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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

lcColor *lc_new_Color(float red, float green, float blue, float alpha);

void lc_delete_Color(lcColor *color);

float lc_Color_get_red(const lcColor *color);

float lc_Color_get_green(const lcColor *color);

float lc_Color_get_blue(const lcColor *color);

float lc_Color_get_alpha(const lcColor *color);

uint8_t lc_Color_get_red_as_int(const lcColor *color);

uint8_t lc_Color_get_green_as_int(const lcColor *color);

uint8_t lc_Color_get_blue_as_int(const lcColor *color);

uint8_t lc_Color_get_alpha_as_int(const lcColor *color);

const char *lc_Color_to_string(const lcColor *color, bool hex);

bool lc_Color_equals(const lcColor *color, const lcColor *other);

lcColor *lc_color_blend(const lcColor *bg, const lcColor *fg);

lcColor *lc_color_mix(const lcColor *a, const lcColor *b, float ratio);

lcColor *lc_color_from_hex(uint64_t hex_color, bool has_alpha);

lcColor *lc_color_from_hex_str(const char *hex_color);

lcColor *lc_color_from_int_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_COLOR_H
