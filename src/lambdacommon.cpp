/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/lambdacommon.h"

namespace lambdacommon
{
	std::string LAMBDACOMMON_API get_version()
	{
		auto result = std::to_string(LAMBDACOMMON_VERSION_MAJOR) + '.' + std::to_string(LAMBDACOMMON_VERSION_MINOR) + '.' + std::to_string(LAMBDACOMMON_VERSION_PATCH);
		if (std::string type{LAMBDACOMMON_VERSION_TYPE}; type != "Release")
			result = type + result;
		return result;
	}
}