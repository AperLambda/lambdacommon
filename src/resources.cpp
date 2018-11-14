/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/resources.h"
#include <stdexcept>
#include <tuple>
#include <fstream>
#include <sstream>

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4101)
#endif

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

	ResourceName::ResourceName(const std::string &domain, const std::string &path) noexcept : _domain(domain),
																							  _path(path)
	{}

	ResourceName::ResourceName(const ResourceName &other) = default;

	ResourceName::ResourceName(ResourceName &&other) noexcept : _domain(std::move(other._domain)),
																_path(std::move(other._path))
	{}

	const std::string &ResourceName::get_domain() const
	{
		return _domain;
	}

	const std::string &ResourceName::get_name() const
	{
		return _path;
	}

	ResourceName ResourceName::sub(const std::string &path) const
	{
		return ResourceName(_domain, lstring::merge_path(_path, path));
	}

	std::string ResourceName::to_string() const
	{
		return _domain + ":" + _path;
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


	ResourcesManager::ResourcesManager(const fs::FilePath &working_directory) : _working_directory(
			working_directory / "resources")
	{}

	ResourcesManager::ResourcesManager(const ResourcesManager &other) = default;

	ResourcesManager::ResourcesManager(ResourcesManager &&other) noexcept : _working_directory(
			std::move(other._working_directory))
	{}

	const lambdacommon::fs::FilePath &ResourcesManager::get_working_directory() const
	{
		return _working_directory;
	}

	bool ResourcesManager::does_resource_exist(const ResourceName &resource_name, const std::string &extension) const
	{
		return get_resource_path(resource_name, extension).exists();
	}

	lambdacommon::fs::FilePath
	ResourcesManager::get_resource_path(const ResourceName &resource_name, const std::string &extension) const
	{
		return (_working_directory / resource_name.get_domain()) / (resource_name.get_name() + "." + extension);
	}

	std::string
	ResourcesManager::load_resource(const ResourceName &resource_name, const std::string &extension) const
	{
		auto resource_path = get_resource_path(resource_name, extension);
		if (!resource_path.exists())
			return "";
		try
		{
			std::ifstream file_stream{resource_path.to_string()};
			std::stringstream output_stream;
			output_stream << file_stream.rdbuf();
			file_stream.close();
			return output_stream.str();
		}
		catch (std::exception &e)
		{
			return "";
		}
	}

	ResourcesManager &ResourcesManager::operator=(const ResourcesManager &other)
	{
		if (this != &other)
		{
			if (other._working_directory != _working_directory)
				_working_directory = other._working_directory;
		}
		return *this;
	}

	ResourcesManager &ResourcesManager::operator=(ResourcesManager &&other) noexcept
	{
		if (this != &other)
			_working_directory = std::move(other._working_directory);
		return *this;
	}

	bool ResourcesManager::operator==(const ResourcesManager &other) const
	{
		return other._working_directory == _working_directory;
	}
}