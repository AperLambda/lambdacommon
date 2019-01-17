/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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

typedef struct lc_ResourceName lc_ResourceName;

lc_ResourceName *lc_new_ResourceName(const char *name);

lc_ResourceName *lc_new_ResourceName_DP(const char *domain, const char *path);

void lc_delete_ResourceName(lc_ResourceName *resourceName);

/*!
 * Gets the domain of the resource.
 * @param resourceName The resource name instance.
 * @return The domain of the resource as a dynamically allocated string.
 */
const char *lc_ResourceName_get_domain(const lc_ResourceName *resourceName);

/*!
 * Gets the name of the resource.
 * @param resourceName The resource name instance.
 * @return The name of the resource as a dynamically allocated string.
 */
const char *lc_ResourceName_get_name(const lc_ResourceName *resourceName);

/*!
 * Gets the resource name as a string.
 * @param resourceName The resource name instance.
 * @return The resource name as a dynamically allocated string.
 */
const char *lc_ResourceName_to_string(const lc_ResourceName *resourceName);

bool lc_ResourceName_equals(const lc_ResourceName *resourceName, const lc_ResourceName *other);

#ifdef __cplusplus
}
#endif

#endif //CLAMBDACOMMON_RESOURCES_H
