/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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
		_name = name.substr(separator + 1);
	}

	ResourceName::ResourceName(const std::string &domain, const std::string &name) noexcept : _domain(domain), _name(name)
	{}

	ResourceName::ResourceName(const ResourceName &other) = default;

	ResourceName::ResourceName(ResourceName &&other) noexcept : _domain(std::move(other._domain)), _name(std::move(other._name))
	{}

	const std::string &ResourceName::get_domain() const
	{
		return _domain;
	}

	const std::string &ResourceName::get_name() const
	{
		return _name;
	}

	ResourceName ResourceName::sub(const std::string &name) const
	{
		return ResourceName(_domain, lstring::merge_path(_name, name));
	}

	std::string ResourceName::to_string() const
	{
		return _domain + ":" + _name;
	}

	ResourceName &ResourceName::operator=(const ResourceName &other)
	{
		if (this != &other) {
			if (other._domain != _domain)
				_domain = other._domain;
			if (other._name != _name)
				_name = other._name;
		}
		return *this;
	}

	ResourceName &ResourceName::operator=(ResourceName &&other) noexcept
	{
		if (this != &other) {
			_domain = std::move(other._domain);
			_name = std::move(other._name);
		}
		return *this;
	}

	bool ResourceName::operator==(const ResourceName &other) const
	{
		return _domain == other._domain && _name == other._name;
	}

	bool ResourceName::operator<(const ResourceName &other) const
	{
		return std::tie(_domain, _name) < std::tie(other._domain, other._name);
	}

	/*
	 * ResourcesManager
	 */
	uint32_t last_id = 0;

	ResourcesManager::ResourcesManager() : _id(++last_id)
	{}

	ResourcesManager::ResourcesManager(const ResourcesManager &other) = default;

	ResourcesManager::ResourcesManager(ResourcesManager &&other) noexcept : _id(other._id)
	{}

	uint32_t ResourcesManager::get_id() const
	{
		return _id;
	}

	bool ResourcesManager::operator==(const ResourcesManager &other) const
	{
		return _id == other._id;
	}

	/*
	 * FileResourcesManager
	 */

	FileResourcesManager::FileResourcesManager(const fs::FilePath &working_directory)
			: ResourcesManager(), _working_directory(working_directory)
	{}

	FileResourcesManager::FileResourcesManager(const FileResourcesManager &other) = default;

	FileResourcesManager::FileResourcesManager(FileResourcesManager &&other) noexcept : _working_directory(std::move(other._working_directory))
	{
		_id = other._id;
	}

	const lambdacommon::fs::FilePath &FileResourcesManager::get_working_directory() const
	{
		return _working_directory;
	}

	bool FileResourcesManager::has_resource(const ResourceName &resource) const
	{
		return this->has_resource(resource, "");
	}

	bool FileResourcesManager::has_resource(const ResourceName &resource, const std::string &extension) const
	{
		return get_resource_path(resource, extension).exists();
	}

	lambdacommon::fs::FilePath
	FileResourcesManager::get_resource_path(const ResourceName &resource, const std::string &extension) const
	{
		auto file = resource.get_name();
		if (!extension.empty())
			file += "." + extension;
		return (_working_directory / resource.get_domain()) / file;
	}

	std::string FileResourcesManager::load_resource(const ResourceName &resource) const
	{
		return this->load_resource(resource, "");
	}

	std::string
	FileResourcesManager::load_resource(const ResourceName &resource, const std::string &extension) const
	{
		auto resource_path = get_resource_path(resource, extension);
		if (!resource_path.exists())
			return "";
		try {
			std::ifstream file_stream{resource_path.to_string()};
			std::stringstream output_stream;
			output_stream << file_stream.rdbuf();
			file_stream.close();
			return output_stream.str();
		}
		catch (std::exception &e) {
			return "";
		}
	}

	FileResourcesManager &FileResourcesManager::operator=(const FileResourcesManager &other)
	{
		if (this != &other) {
			if (_id != other._id)
				_id = other._id;
			if (_working_directory != other._working_directory)
				_working_directory = other._working_directory;
		}
		return *this;
	}

	FileResourcesManager &FileResourcesManager::operator=(FileResourcesManager &&other) noexcept
	{
		if (this != &other) {
			_id = other._id;
			_working_directory = std::move(other._working_directory);
		}
		return *this;
	}

	bool FileResourcesManager::operator==(const FileResourcesManager &other) const
	{
		return _id == other._id && _working_directory == other._working_directory;
	}
}
