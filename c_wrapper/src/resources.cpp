/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
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

lc_ResourceName *lc_newResourceName(const char *name)
{
	return reinterpret_cast<lc_ResourceName *>(new lambdacommon::ResourceName(std::string(name)));
}

lc_ResourceName *lc_newResourceNameDP(const char *domain, const char *path)
{
	return reinterpret_cast<lc_ResourceName *>(new lambdacommon::ResourceName({domain}, {path}));
}

void lc_deleteResourceName(lc_ResourceName *resourceName)
{
	delete RESOURCENAME_CPP(resourceName);
}

const char *lc_ResourceName_getDomain(const lc_ResourceName *resourceName)
{
	auto domainCpp = CONST_RESOURCENAME_CPP(resourceName)->getDomain();
	char *domain = new char[domainCpp.size()];
	domainCpp.copy(domain, domainCpp.size());
	return domain;
}

const char *lc_ResourceName_getName(const lc_ResourceName *resourceName)
{
	auto nameCpp = CONST_RESOURCENAME_CPP(resourceName)->getName();
	char *name = new char[nameCpp.size()];
	nameCpp.copy(name, nameCpp.size());
	return name;
}

const char *lc_ResourceName_toString(const lc_ResourceName *resourceName)
{
	auto toStringCpp = CONST_RESOURCENAME_CPP(resourceName)->toString();
	char *toString = new char[toStringCpp.size()];
	toStringCpp.copy(toString, toStringCpp.size());
	return toString;
}

bool lc_ResourceName_equals(const lc_ResourceName *resourceName, const lc_ResourceName *other)
{
	return *CONST_RESOURCENAME_CPP(resourceName) == *CONST_RESOURCENAME_CPP(other);
}