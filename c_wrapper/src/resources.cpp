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

#define RESOURCENAME_CPP(RESOURCENAME) reinterpret_cast<lambdacommon::Identifier *>(RESOURCENAME)
#define CONST_RESOURCENAME_CPP(RESOURCENAME) reinterpret_cast<const lambdacommon::Identifier *>(RESOURCENAME)

lc_Identifier* lc_new_Identifier(const char* name) {
    return reinterpret_cast<lc_Identifier*>(new lambdacommon::Identifier(std::string(name)));
}

lc_Identifier* lc_new_Identifier_DP(const char* domain, const char* path) {
    return reinterpret_cast<lc_Identifier*>(new lambdacommon::Identifier({domain}, {path}));
}

void lc_delete_Identifier(lc_Identifier* resourceName) {
    delete RESOURCENAME_CPP(resourceName);
}

const char* lc_Identifier_get_namespace(const lc_Identifier* resourceName) {
    auto domain_cpp = CONST_RESOURCENAME_CPP(resourceName)->get_domain();
    char* domain = new char[domain_cpp.size()];
    domain_cpp.copy(domain, domain_cpp.size());
    return domain;
}

const char* lc_Identifier_get_name(const lc_Identifier* resourceName) {
    auto name_cpp = CONST_RESOURCENAME_CPP(resourceName)->get_name();
    char* name = new char[name_cpp.size()];
    name_cpp.copy(name, name_cpp.size());
    return name;
}

const char* lc_Identifier_to_string(const lc_Identifier* resourceName) {
    auto to_string_cpp = CONST_RESOURCENAME_CPP(resourceName)->to_string();
    char* toString = new char[to_string_cpp.size()];
    to_string_cpp.copy(toString, to_string_cpp.size());
    return toString;
}

bool lc_Identifier_equals(const lc_Identifier* resourceName, const lc_Identifier* other) {
    return *CONST_RESOURCENAME_CPP(resourceName) == *CONST_RESOURCENAME_CPP(other);
}
