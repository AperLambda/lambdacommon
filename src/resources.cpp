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

	std::string ResourceName::toString() const
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


	ResourcesManager::ResourcesManager(const fs::FilePath &workingDirectory) : _workingDirectory(
			workingDirectory / "resources")
	{}

	ResourcesManager::ResourcesManager(const ResourcesManager &resourcesManager) = default;

	ResourcesManager::ResourcesManager(ResourcesManager &&resourcesManager) noexcept : _workingDirectory(
			std::move(resourcesManager._workingDirectory))
	{}

	const lambdacommon::fs::FilePath &ResourcesManager::getWorkingDirectory() const
	{
		return _workingDirectory;
	}

	bool ResourcesManager::doesResourceExist(const ResourceName &resourceName, const std::string &extension) const
	{
		return getResourcePath(resourceName, extension).exists();
	}

	lambdacommon::fs::FilePath
	ResourcesManager::getResourcePath(const ResourceName &resourceName, const std::string &extension) const
	{
		return (_workingDirectory / resourceName.getDomain()) / (resourceName.getName() + "." + extension);
	}

	std::string
	ResourcesManager::loadResource(const ResourceName &resourceName, const std::string &extension) const
	{
		auto resourcePath = getResourcePath(resourceName, extension);
		if (!resourcePath.exists())
			return "";
		try
		{
			std::ifstream fileStream{resourcePath.toString()};
			std::stringstream outputStream;
			outputStream << fileStream.rdbuf();
			fileStream.close();
			return outputStream.str();
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
			if (other._workingDirectory != _workingDirectory)
				_workingDirectory = other._workingDirectory;
		}
		return *this;
	}

	ResourcesManager &ResourcesManager::operator=(ResourcesManager &&other) noexcept
	{
		if (this != &other)
			_workingDirectory = std::move(other._workingDirectory);
		return *this;
	}

	bool ResourcesManager::operator==(const ResourcesManager &other) const
	{
		return other._workingDirectory == _workingDirectory;
	}
}