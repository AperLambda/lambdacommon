/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/resources.h"
#include <stdexcept>

namespace lambdacommon
{
	ResourceName::ResourceName(const std::string &name)
	{
		auto separator = name.find_first_of(':');
		if (separator <= 0)
			throw std::invalid_argument("The resource name '" + name + "' is invalid.");
		_domain = name.substr(0, separator);
		_path = name.substr(separator + 1);
	}

	ResourceName::ResourceName(std::string domain, std::string path) : _domain(std::move(domain)), _path(std::move(path))
	{}

	std::string ResourceName::getDomain()
	{
		return _domain;
	}

	std::string ResourceName::getName()
	{
		return _path;
	}

	ResourceName ResourceName::sub(const std::string &path)
	{
		return ResourceName(_domain, lambdastring::mergePath(_path, path));
	}
}