/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
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
#include <utility>

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4101)
#endif

namespace lambdacommon
{
    Identifier::Identifier(const std::string& name) {
        auto separator = name.find_first_of(':');
        if (separator <= 0)
            throw std::invalid_argument("The resource name '" + name + "' is invalid.");
        _namespace = name.substr(0, separator);
        _name = name.substr(separator + 1);
    }

    Identifier::Identifier(std::string domain, std::string name) noexcept : _namespace(std::move(domain)), _name(std::move(name)) {}

    Identifier::Identifier(const Identifier& other) = default;

    Identifier::Identifier(Identifier&& other) noexcept : _namespace(std::move(other._namespace)), _name(std::move(other._name)) {}

    const std::string& Identifier::get_domain() const {
        return _namespace;
    }

    const std::string& Identifier::get_name() const {
        return _name;
    }

    Identifier Identifier::sub(const std::string& name) const {
        return Identifier(_namespace, lstring::merge_path(_name, name));
    }

    std::string Identifier::to_string() const {
        return _namespace + ":" + _name;
    }

    Identifier& Identifier::operator=(const Identifier& other) {
        if (this != &other) {
            if (other._namespace != _namespace)
                _namespace = other._namespace;
            if (other._name != _name)
                _name = other._name;
        }
        return *this;
    }

    Identifier& Identifier::operator=(Identifier&& other) noexcept {
        if (this != &other) {
            _namespace = std::move(other._namespace);
            _name = std::move(other._name);
        }
        return *this;
    }

    bool Identifier::operator==(const Identifier& other) const {
        return _namespace == other._namespace && _name == other._name;
    }

    bool Identifier::operator<(const Identifier& other) const {
        return std::tie(_namespace, _name) < std::tie(other._namespace, other._name);
    }

    /*
     * ResourcesManager
     */
    u32 last_id = 0;

    ResourcesManager::ResourcesManager() : _id(++last_id) {}

    ResourcesManager::ResourcesManager(const ResourcesManager& other) = default;

    ResourcesManager::ResourcesManager(ResourcesManager&& other) noexcept : _id(other._id) {}

    u32 ResourcesManager::get_id() const {
        return _id;
    }

    bool ResourcesManager::operator==(const ResourcesManager& other) const {
        return _id == other._id;
    }

    /*
     * FileResourcesManager
     */

    FileResourcesManager::FileResourcesManager(fs::path working_directory) : ResourcesManager(), _working_directory(std::move(working_directory)) {}

    FileResourcesManager::FileResourcesManager(const FileResourcesManager& other) = default;

    FileResourcesManager::FileResourcesManager(FileResourcesManager&& other) noexcept : _working_directory(std::move(other._working_directory)) {
        _id = other._id;
    }

    const lambdacommon::fs::path& FileResourcesManager::get_working_directory() const {
        return _working_directory;
    }

    bool FileResourcesManager::has_resource(const Identifier& resource) const {
        return this->has_resource(resource, "");
    }

    bool FileResourcesManager::has_resource(const Identifier& resource, const std::string& extension) const {
        return get_resource_path(resource, extension).exists();
    }

    lambdacommon::fs::path FileResourcesManager::get_resource_path(const Identifier& resource, const std::string& extension) const {
        auto file = resource.get_name();
        if (!extension.empty())
            file += "." + extension;
        return (_working_directory / resource.get_domain()) / file;
    }

    std::string FileResourcesManager::load_resource(const Identifier& resource) const {
        return this->load_resource(resource, "");
    }

    std::string FileResourcesManager::load_resource(const Identifier& resource, const std::string& extension) const {
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
        catch (std::exception& e) {
            return "";
        }
    }

    FileResourcesManager& FileResourcesManager::operator=(const FileResourcesManager& other) {
        if (this != &other) {
            if (_id != other._id)
                _id = other._id;
            if (_working_directory != other._working_directory)
                _working_directory = other._working_directory;
        }
        return *this;
    }

    FileResourcesManager& FileResourcesManager::operator=(FileResourcesManager&& other) noexcept {
        if (this != &other) {
            _id = other._id;
            _working_directory = std::move(other._working_directory);
        }
        return *this;
    }

    bool FileResourcesManager::operator==(const FileResourcesManager& other) const {
        return _id == other._id && _working_directory == other._working_directory;
    }
}
