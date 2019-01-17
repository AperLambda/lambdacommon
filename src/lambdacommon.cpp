/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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
		std::string result(LAMBDACOMMON_VERSION_STR);
		if (std::string type{LAMBDACOMMON_VERSION_TYPE}; type != "Release")
			result = type + result;
		return result;
	}
}
