/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/clambdacommon/resources.h"
#include <lambdacommon/resources.h>

#define RESOURCENAME_CPP(RESOURCENAME) reinterpret_cast<lambdacommon::ResourceName *>(RESOURCENAME)
#define CONST_RESOURCENAME_CPP(RESOURCENAME) reinterpret_cast<const lambdacommon::ResourceName *>(RESOURCENAME)

lc_ResourceName *lc_new_ResourceName(const char *name)
{
    return reinterpret_cast<lc_ResourceName *>(new lambdacommon::ResourceName(std::string(name)));
}

lc_ResourceName *lc_new_ResourceName_DP(const char *domain, const char *path)
{
    return reinterpret_cast<lc_ResourceName *>(new lambdacommon::ResourceName({domain}, {path}));
}

void lc_delete_ResourceName(lc_ResourceName *resourceName)
{
    delete RESOURCENAME_CPP(resourceName);
}

const char *lc_ResourceName_get_domain(const lc_ResourceName *resourceName)
{
    auto domain_cpp = CONST_RESOURCENAME_CPP(resourceName)->get_domain();
    char *domain = new char[domain_cpp.size()];
    domain_cpp.copy(domain, domain_cpp.size());
    return domain;
}

const char *lc_ResourceName_get_name(const lc_ResourceName *resourceName)
{
    auto name_cpp = CONST_RESOURCENAME_CPP(resourceName)->get_name();
    char *name = new char[name_cpp.size()];
    name_cpp.copy(name, name_cpp.size());
    return name;
}

const char *lc_ResourceName_to_string(const lc_ResourceName *resourceName)
{
    auto to_string_cpp = CONST_RESOURCENAME_CPP(resourceName)->to_string();
    char *toString = new char[to_string_cpp.size()];
    to_string_cpp.copy(toString, to_string_cpp.size());
    return toString;
}

bool lc_ResourceName_equals(const lc_ResourceName *resourceName, const lc_ResourceName *other)
{
    return *CONST_RESOURCENAME_CPP(resourceName) == *CONST_RESOURCENAME_CPP(other);
}
