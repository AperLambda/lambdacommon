/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/serializable.h"

using namespace std;

namespace lambdacommon
{
	namespace serializable
	{
		vector<string> LAMBDACOMMON_API tokenize(const string &_string, const string &delim)
		{
			string::size_type lastPos = 0, pos = _string.find_first_of(delim, lastPos);
			vector<string> tokens;

			while (lastPos != string::npos)
			{
				if (pos != lastPos)
					tokens.push_back(_string.substr(lastPos, pos - lastPos));
				lastPos = pos;
				if (lastPos == string::npos || lastPos + 1 == _string.length())
					break;
				pos = _string.find_first_of(delim, ++lastPos);
			}

			return tokens;
		}
	}
}