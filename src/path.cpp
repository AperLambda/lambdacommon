/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/path.h"
#include <sstream>

namespace lambdacommon
{
	Path::Path() : _path(new std::vector<std::string>())
	{}

	Path::Path(const std::vector<std::string> &path) : _path(new std::vector<std::string>(path))
	{}

	Path::~Path()
	{
		delete _path;
	}

	std::vector<std::string> Path::getPath() const
	{
		return *_path;
	}

	std::string Path::toString() const
	{
		return toString('/');
	}

	std::string Path::toString(char delimiter) const
	{
		std::ostringstream oss;
		for (size_t i = 0; i < _path->size(); i++)
		{
			oss << (*_path)[i];
			if (i + 1 < _path->size())
				oss << delimiter;
		}
		return oss.str();
	}
}