/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_SERIALIZABLE_H
#define LAMBDACOMMON_SERIALIZABLE_H

#include "lambdacommon.h"
#include <vector>

namespace lambdacommon
{
	class LAMBDACOMMON_API Serializable
	{
	public:
		virtual std::string toString() const = 0;
	};

	namespace serializable
	{
		extern std::vector<std::string> LAMBDACOMMON_API tokenize(const std::string &_string, const std::string &delim);
	}
}

#endif //LAMBDACOMMON_SERIALIZABLE_H