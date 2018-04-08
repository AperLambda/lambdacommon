/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/utils.h"

using namespace std;

namespace lambdacommon
{
	namespace utils
	{
		string LAMBDACOMMON_API toStringForVector(const vector<string> &vector1)
		{
			string final{"{"};
			for (auto str : vector1)
				final += (str + ", ");
			final = final.substr(0, final.size() - 2) + '}';
			return final;
		}

		ostream LAMBDACOMMON_API &operator<<(ostream &stream, const vector<string> &vectorToPrint)
		{
			stream << toStringForVector(vectorToPrint);
			return stream;
		}
	}
}