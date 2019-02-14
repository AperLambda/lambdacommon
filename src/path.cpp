/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/path.h"
#include <sstream>

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4101)
#endif

namespace lambdacommon
{
	Path::Path() : _path()
	{}

	Path::Path(std::vector<std::string> path) : _path(std::move(path))
	{}

	Path::Path(const Path &other) = default;

	Path::Path(Path &&other) noexcept : _path(std::move(other._path))
	{}

	Path::~Path()
	{}

	const std::vector<std::string> &Path::get_path() const
	{
		return _path;
	}

	std::string Path::to_string() const
	{
		return to_string('/');
	}

	std::string Path::to_string(char delimiter) const
	{
		std::ostringstream oss;
		for (size_t i = 0; i < _path.size(); i++) {
			oss << _path[i];
			if (i + 1 < _path.size())
				oss << delimiter;
		}
		return oss.str();
	}
}
