/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/path.h"
#include "../include/lambdacommon/lstring.h"
#include "lambdacommon/lerror.h"
#include <sstream>

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4101)
#endif

namespace lambdacommon
{
    std::string fix_path(const std::string& path) {
        return lstring::replace_all(path, '\\', '/');
    }

    AbstractPath::AbstractPath() : _path() {}

    AbstractPath::AbstractPath(std::string path) : _path(std::move(lstring::replace_all(std::move(path), '\\', '/'))) {}

    AbstractPath::AbstractPath(const AbstractPath& other) = default;

    AbstractPath::AbstractPath(AbstractPath&& other) noexcept : _path(std::move(other._path)) {}

    void AbstractPath::append(const std::string& path) {
        if (lstring::starts_with(path, "/"))
            throw std::invalid_argument("Cannot append an absolute path to the current path.");
        _path += fix_path(path);
    }

    void AbstractPath::append(const AbstractPath& path) {
        if (!path.is_absolute())
            throw std::invalid_argument("Cannot append an absolute path to the current path.");
        _path += path._path;
    }


    const std::string& AbstractPath::get_path() const {
        return _path;
    }

    void AbstractPath::clear() noexcept {
        this->_path.clear();
    }

    bool AbstractPath::empty() const {
        return this->_path.empty();
    }

    bool AbstractPath::is_absolute() const {
        return lstring::starts_with(_path, "/") && _path.find("..") == std::string::npos;
    }

    std::string AbstractPath::to_string() const {
        return _path;
    }

    std::string AbstractPath::to_string(char delimiter) const {
        if (this->empty())
            return "";
        return std::move(lstring::replace_all(_path, '/', delimiter));
    }

    bool AbstractPath::operator==(const AbstractPath& other) const {
        return this->_path == other._path;
    }

    bool AbstractPath::operator<(const AbstractPath& other) const {
        return this->_path < other._path;
    }
}
