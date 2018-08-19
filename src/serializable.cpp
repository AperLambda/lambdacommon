/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/serializable.h"

namespace lambdacommon
{
	namespace serializable
	{
		std::vector<std::string> LAMBDACOMMON_API tokenize(const std::string &_string, const std::string &delim)
		{
			std::string::size_type lastPos = 0, pos = _string.find_first_of(delim, lastPos);
			std::vector<std::string> tokens;

			while (lastPos != std::string::npos)
			{
				if (pos != lastPos)
					tokens.push_back(_string.substr(lastPos, pos - lastPos));
				lastPos = pos;
				if (lastPos == std::string::npos || lastPos + 1 == _string.length())
					break;
				pos = _string.find_first_of(delim, ++lastPos);
			}

			return tokens;
		}
	}
}