/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef CLAMBDACOMMON_RESOURCES_H
#define CLAMBDACOMMON_RESOURCES_H

#include "clambdacommon.h"
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lc_Identifier lc_Identifier;

lc_Identifier* lc_new_Identifier(const char* name);

lc_Identifier* lc_new_Identifier_DP(const char* domain, const char* path);

void lc_delete_Identifier(lc_Identifier* resourceName);

/*!
 * Gets the namespace of the resource.
 * @param resourceName The resource name instance.
 * @return The namespace of the resource as a dynamically allocated string.
 */
const char* lc_Identifier_get_namespace(const lc_Identifier* resourceName);

/*!
 * Gets the name of the resource.
 * @param resourceName The resource name instance.
 * @return The name of the resource as a dynamically allocated string.
 */
const char* lc_Identifier_get_name(const lc_Identifier* resourceName);

/*!
 * Gets the resource name as a string.
 * @param resourceName The resource name instance.
 * @return The resource name as a dynamically allocated string.
 */
const char* lc_Identifier_to_string(const lc_Identifier* resourceName);

bool lc_Identifier_equals(const lc_Identifier* resourceName, const lc_Identifier* other);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_RESOURCES_H
