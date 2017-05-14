/*
 * Copyright © 2017 Clément "wAperClem" Wagner
 *
 * This file is part of AperCommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef APERCOMMON_FILESYSTEM_H
#define APERCOMMON_FILESYSTEM_H

#include "../../../include/apercommon/system/filesystem/filesystem.h"
#include "../../../include/apercommon/string.h"
#include <stdexcept>
#include <sstream>

#ifdef APER_WINDOWS

#include <windows.h>

#define STAT_STRUCT _stati64
#define STAT_METHOD _stati64

#else

#include <unistd.h>
#define STAT_STRUCT stat
#define STAT_METHOD stat

#endif

#include <sys/stat.h>

using namespace std;

namespace apercommon
{
    namespace filesystem
    {
        vector<string> tokenize(const string &_string, const string &delim)
        {
            string::size_type lastPos = 0, pos = _string.find_first_of(delim, lastPos);
            vector<string> tokens;

            while (lastPos != string::npos)
            {
                if (pos != lastPos)
                    tokens.push_back(_string.substr(lastPos, pos - lastPos));
                lastPos = pos;
                if (lastPos == string::npos || lastPos + 1 == _string.length())
                    break;
                pos = _string.find_first_of(delim, ++lastPos);
            }

            return tokens;
        }

        Path::Path() : _type(NATIVE), _path(new vector<string>()), _absolute(false)
        {

        }

        Path::Path(const char *path) : _path(new vector<string>())
        {
            set(string(path));
        }

        Path::Path(string path) : _path(new vector<string>())
        {
            set(path);
        }

#ifdef APER_WINDOWS

        Path::Path(const wchar_t *path) : _path(new vector<string>())
        {
            set(aperstring::convertWStringToString(wstring(path)));
        }

        Path::Path(wstring path) : _path(new vector<string>())
        {
            set(aperstring::convertWStringToString(path));
        }

#endif

        Path::Path(const Path &path) : _type(path._type), _path(new vector<string>(*path._path)),
                                       _absolute(path._absolute)
        {}

        Path::~Path()
        {
            if (_path != nullptr)
                delete _path;
        }

        void Path::set(const string &str, PathType type)
        {
            _type = type;
            if (type == WINDOWS)
            {
                *_path = tokenize(str, "/\\");
                _absolute = str.size() > 2 && isalpha(str[0]) && str[1] == ':';
            }
            else
            {
                *_path = tokenize(str, "/");
                _absolute = !str.empty() && str[0] == '/';
            }
        }

        bool Path::remove()
        {
#ifdef APER_WINDOWS
            return DeleteFileA(toString().c_str()) != 0;
#else
            return std::remove(toString().c_str()) == 0;
#endif
        }

        bool Path::mkdir()
        {
#ifdef APER_WINDOWS
            return CreateDirectoryA(toString().c_str(), NULL) != 0;
#else
            return ::mkdir(toString().c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
#endif
        }

        bool Path::empty() const
        {
            return _path->empty();
        }

        bool Path::isAbsolute() const
        {
            return _absolute;
        }

        bool Path::exists() const
        {
#ifdef APER_WINDOWS
            return GetFileAttributesA(toString().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
            struct STAT_STRUCT sb;
            return stat(toString().c_str(), &sb) == 0;
#endif
        }

        bool Path::isDirectory() const
        {
#ifdef APER_WINDOWS
            auto attr = GetFileAttributesA(toString().c_str());
            return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
            struct STAT_STRUCT sb;
            if (stat(toString().c_str(), &sb))
                return false;
            return S_ISDIR(sb.st_mode);
#endif
        }

        bool Path::isFile() const
        {
#ifdef APER_WINDOWS
            auto attr = GetFileAttributesA(toString().c_str());
            return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
            struct STAT_STRUCT sb;
            if (stat(toString().c_str(), &sb))
                return false;
            return S_ISREG(sb.st_mode);
#endif
        }

        string Path::getFileName() const
        {
            if (empty())
                return "";
            const string &last = (*_path)[_path->size() - 1];
            return last;
        }

        string Path::getExtension() const
        {
            const string &name = getFileName();
            size_t pos = name.find_last_of(".");
            if (pos == string::npos)
                return "";
            return name.substr(pos + 1);
        }

        size_t Path::getFileSize() const
        {
            if (!exists())
                return 0;
            struct STAT_STRUCT sb;
            if (STAT_METHOD(toString().c_str(), &sb) != 0)
                throw runtime_error("Path::getFileSize(): cannot stat file \"" + toString() + "\"!");
            return (size_t) sb.st_size;
        }

        Path Path::toAbsolute() const
        {
#ifdef APER_WINDOWS
            if (toString().empty())
                return getCurrentWorkingDirectory();
            char temp[MAX_PATH];
            auto length = GetFullPathNameA(toString().c_str(), MAX_PATH, temp, NULL);
            if (length == 0)
                throw runtime_error("Internal error in apercommon::Path::toAbsolute(): " + to_string(GetLastError()));
            return Path(temp);
#else
            char temp[PATH_MAX];
            if (realpath(toString().c_str(), temp) == NULL)
                throw runtime_error("Internal error in apercommon::Path::toAbsolute(): " + string(strerror(errno)));
            return Path(temp);
#endif
        }

        Path Path::getParent() const
        {
            Path result;
            result._type = _type;
            result._absolute = _absolute;

            if (empty())
            {
                if (!_absolute)
                    result._path->push_back("..");
            }
            else
                for (size_t i = 0; i < _path->size() - 1; ++i)
                    result._path->push_back((*_path)[i]);
            return result;
        }

        string Path::toString(PathType type) const
        {
            ostringstream oss;

            if (type == COMMON && _absolute)
                oss << "/";

            for (size_t i = 0; i < _path->size(); i++)
            {
                oss << (*_path)[i];
                if (i + 1 < _path->size())
                {
                    if (type == COMMON)
                        oss << '/';
                    else
                        oss << '\\';
                }
            }

            return oss.str();
        }

        /*
         * OPERATORS
         */

        Path Path::operator/(const Path &other)
        {
            if (other._absolute)
                throw runtime_error("apercommon::Path::operator/(): Expected a relative path!");

            Path result(*this);

            for (auto part : *other._path)
                (*result._path).push_back(part);

            return result;
        }

        bool Path::operator==(const Path &_p)
        {
            return _p._path == _path;
        }

        bool Path::operator!=(const Path &_p)
        {
            return _p._path != _path;
        }

#ifdef APER_WINDOWS

        wstring getCurrentWorkingDirectoryWStr()
        {
            wchar_t temp[MAX_PATH];
            if (!_wgetcwd(temp, MAX_PATH))
                throw runtime_error(
                        "Internal error in apercommon::getCurrentWorkingDirectoryWStr(): " + to_string(GetLastError()));
            return wstring(temp);
        }

#endif

        string getCurrentWorkingDirectoryStr()
        {
#ifdef APER_WINDOWS
            return aperstring::convertWStringToString(getCurrentWorkingDirectoryWStr());
#else
            char temp[PATH_MAX];
            if (getcwd(temp, PATH_MAX) == NULL)
                throw runtime_error("Internal error in apercommon::getCurrentWorkingDirectoryStr(): " + string(strerror(errno)));
            return string(temp);
#endif
        }

        Path getCurrentWorkingDirectory()
        {
            return Path(getCurrentWorkingDirectoryStr());
        }

        Path APERCOMMON_API mkdir(const char *path)
        {
            return mkdir(string(path));
        }

        Path APERCOMMON_API mkdir(std::string path)
        {
            Path result{path};
            result.mkdir();
            return result;
        }
    }
}

#undef STAT_STRUCT
#endif //APERCOMMON_FILESYSTEM_H
