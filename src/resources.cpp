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
#include <tuple>

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

	ResourceName::ResourceName(std::string domain, std::string path) noexcept : _domain(std::move(domain)),
	                                                                            _path(std::move(path))
	{}

	ResourceName::ResourceName(const ResourceName &resourceName) = default;

	ResourceName::ResourceName(ResourceName &&resourceName) noexcept : _domain(std::move(resourceName._domain)),
	                                                                   _path(std::move(resourceName._path))
	{}

	const std::string &ResourceName::getDomain() const
	{
		return _domain;
	}

	const std::string &ResourceName::getName() const
	{
		return _path;
	}

	ResourceName ResourceName::sub(const std::string &path) const
	{
		return ResourceName(_domain, lambdastring::mergePath(_path, path));
	}

	ResourceName &ResourceName::operator=(const ResourceName &other)
	{
		if (this != &other)
		{
			if (other._domain != _domain)
				_domain = other._domain;
			if (other._path != _path)
				_path = other._path;
		}
		return *this;
	}

	ResourceName &ResourceName::operator=(ResourceName &&other) noexcept
	{
		if (this != &other)
		{
			_domain = std::move(other._domain);
			_path = std::move(other._path);
		}
		return *this;
	}

	bool ResourceName::operator==(const ResourceName &other) const
	{
		return _domain == other._domain && _path == other._path;
	}

	bool ResourceName::operator<(const ResourceName &other) const
	{
		return std::tie(_domain, _path) < std::tie(other._domain, other._path);
	}
}