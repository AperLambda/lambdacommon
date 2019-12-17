/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_COLOR_H
#define CLAMBDACOMMON_COLOR_H

#include "../clambdacommon.h"
#include "../types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lcColor lcColor;

lcColor* lc_new_Color(f32 red, f32 green, f32 blue, f32 alpha);

void lc_delete_Color(lcColor* color);

f32 lc_Color_get_red(const lcColor* color);

f32 lc_Color_get_green(const lcColor* color);

f32 lc_Color_get_blue(const lcColor* color);

f32 lc_Color_get_alpha(const lcColor* color);

u8 lc_Color_get_red_as_int(const lcColor* color);

u8 lc_Color_get_green_as_int(const lcColor* color);

u8 lc_Color_get_blue_as_int(const lcColor* color);

u8 lc_Color_get_alpha_as_int(const lcColor* color);

const char* lc_Color_to_string(const lcColor* color, bool hex);

bool lc_Color_equals(const lcColor* color, const lcColor* other);

lcColor* lc_color_blend(const lcColor* bg, const lcColor* fg);

lcColor* lc_color_mix(const lcColor* a, const lcColor* b, float ratio);

lcColor* lc_color_from_hex(u64 hex_color, bool has_alpha);

lcColor* lc_color_from_hex_str(const char* hex_color);

lcColor* lc_color_from_int_rgba(u8 red, u8 green, u8 blue, u8 alpha);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_COLOR_H
