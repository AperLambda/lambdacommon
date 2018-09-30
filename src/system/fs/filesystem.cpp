/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../../include/lambdacommon/system/fs/filesystem.h"
#include "../../../include/lambdacommon/lstring.h"
#include <stdexcept>
#include <sstream>

#ifdef LAMBDA_WINDOWS

#include <Windows.h>

#define STAT_STRUCT _stati64
#define STAT_METHOD _stati64

#else

#include <unistd.h>
#include <cstring>
#include <climits>

#define STAT_STRUCT stat
#define STAT_METHOD stat

#endif

#include <sys/stat.h>

namespace lambdacommon
{
	namespace fs
	{
		FilePath::FilePath() : Path(), _absolute(false)
		{

		}

		FilePath::FilePath(const char *path) : Path()
		{
			set(std::string(path));
		}

		FilePath::FilePath(const std::string &path) : Path()
		{
			set(path);
		}

#ifdef LAMBDA_WINDOWS

		FilePath::FilePath(const wchar_t *path) : Path()
		{
			set(lstring::convertWStringToString(std::wstring(path)));
		}

		FilePath::FilePath(const std::wstring &path) : Path()
		{
			set(lstring::convertWStringToString(path));
		}

#endif

		FilePath::FilePath(const FilePath &path) : Path(*path._path), _absolute(path._absolute)
		{}

		FilePath::FilePath(FilePath &&path) noexcept : Path(std::vector<std::string>(move(*path._path))),
		                                               _absolute(path._absolute)
		{}

		void FilePath::set(const std::string &str, PathType type)
		{
			if (type == WINDOWS)
			{
				*_path = serializable::tokenize(str, "/\\");
				_absolute = str.size() > 2 && isalpha(str[0]) && str[1] == ':';
			} else
			{
				*_path = serializable::tokenize(str, "/");
				_absolute = !str.empty() && str[0] == '/';
			}
		}

		bool FilePath::remove() const
		{
#ifdef LAMBDA_WINDOWS
			return DeleteFileA(toString().c_str()) != 0;
#else
			return std::remove(toString().c_str()) == 0;
#endif
		}

		bool FilePath::mkdir(bool recursive) const
		{
			if (recursive)
			{
				auto parent = getParent();
				if (!parent.exists())
					parent.mkdir(true);
			}
#ifdef LAMBDA_WINDOWS
			return CreateDirectoryA(toString().c_str(), nullptr) != 0;
#else
			return ::mkdir(toString().c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
#endif
		}

		bool FilePath::empty() const
		{
			return _path->empty();
		}

		bool FilePath::isAbsolute() const
		{
			return _absolute;
		}

		bool FilePath::exists() const
		{
#ifdef LAMBDA_WINDOWS
			return GetFileAttributesA(toString().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
			struct STAT_STRUCT sb;
			return stat(toString().c_str(), &sb) == 0;
#endif
		}

		bool FilePath::isDirectory() const
		{
#ifdef LAMBDA_WINDOWS
			auto attr = GetFileAttributesA(toString().c_str());
			return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
			struct STAT_STRUCT sb;
			if (stat(toString().c_str(), &sb))
				return false;
			return S_ISDIR(sb.st_mode);
#endif
		}

		bool FilePath::isFile() const
		{
#ifdef LAMBDA_WINDOWS
			auto attr = GetFileAttributesA(toString().c_str());
			return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
			struct STAT_STRUCT sb;
			if (stat(toString().c_str(), &sb))
				return false;
			return S_ISREG(sb.st_mode);
#endif
		}

		std::string FilePath::getFileName() const
		{
			if (empty())
				return "";
			const std::string &last = (*_path)[_path->size() - 1];
			return last;
		}

		std::string FilePath::getExtension() const
		{
			const std::string &name = getFileName();
			size_t pos = name.find_last_of('.');
			if (pos == std::string::npos)
				return "";
			return name.substr(pos + 1);
		}

		size_t FilePath::getFileSize() const
		{
			if (!exists())
				return 0;
			struct STAT_STRUCT sb;
			if (STAT_METHOD(toString().c_str(), &sb) != 0)
				throw std::runtime_error(
						"lambdacommon::path::FilePath.getFileSize(): cannot stat file \"" + toString() + "\"!");
			return (size_t) sb.st_size;
		}

		FilePath FilePath::toAbsolute() const
		{
#ifdef LAMBDA_WINDOWS
			if (toString().empty())
				return getCurrentWorkingDirectory();
			char temp[MAX_PATH];
			auto length = GetFullPathNameA(toString().c_str(), MAX_PATH, temp, nullptr);
			if (length == 0)
				throw std::runtime_error(
						"Internal error in lambdacommon::path::FilePath.toAbsolute(): " + std::to_string(GetLastError()));
			return FilePath(temp);
#else
			char temp[PATH_MAX];
			if (realpath(toString().c_str(), temp) == nullptr)
				throw std::runtime_error(
						"Internal error in lambdacommon::path::FilePath::toAbsolute(): " + std::string(strerror(errno)));
			return FilePath(temp);
#endif
		}

		FilePath FilePath::getParent() const
		{
			FilePath result;
			result._absolute = _absolute;

			if (empty())
			{
				if (!_absolute)
					result._path->push_back("..");
			} else
				for (size_t i = 0; i < _path->size() - 1; ++i)
					result._path->push_back((*_path)[i]);
			return result;
		}

		std::string FilePath::toString(PathType type) const
		{
			std::ostringstream oss;

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

		FilePath FilePath::sub(const FilePath &other) const
		{
			if (other._absolute)
				throw std::runtime_error("lambdacommon->system/fs/filesystem.cpp@FilePath.sub(const FilePath&)(Line " + std::to_string(__LINE__ - 1) + "): Expected a relative path as argument!");

			FilePath result(*this);

			for (const auto &part : *other._path)
				(*result._path).push_back(part);

			return result;
		}

		/*
		 * OPERATORS
		 */

		FilePath FilePath::operator/(const FilePath &other) const
		{
			return sub(other);
		}

		FilePath &FilePath::operator=(const FilePath &path)
		{
			*_path = *path._path;
			_absolute = path._absolute;
			return *this;
		}

		FilePath &FilePath::operator=(FilePath &&path) noexcept
		{
			if (this != &path)
			{
				delete _path;
				_path = new std::vector<std::string>(move(*path._path));
				_absolute = path._absolute;
			}
			return *this;
		}

		bool FilePath::operator==(const FilePath &_p) const
		{
			return _p._path == _path;
		}

		bool FilePath::operator!=(const FilePath &_p) const
		{
			return _p._path != _path;
		}

#ifdef LAMBDA_WINDOWS

		std::wstring LAMBDACOMMON_API getCurrentWorkingDirectoryWStr()
		{
			wchar_t temp[MAX_PATH];
			if (!_wgetcwd(temp, MAX_PATH))
				throw std::runtime_error(
						"Internal error in lambdacommon::path::getCurrentWorkingDirectoryWStr(): " +
						std::to_string(GetLastError()));
			return std::wstring(temp);
		}

#endif

		std::string LAMBDACOMMON_API getCurrentWorkingDirectoryStr()
		{
#ifdef LAMBDA_WINDOWS
			return lstring::convertWStringToString(getCurrentWorkingDirectoryWStr());
#else
			char temp[PATH_MAX];
			if (getcwd(temp, PATH_MAX) == nullptr)
				throw std::runtime_error("Internal error in lambdacommon::path::getCurrentWorkingDirectoryStr(): " +
				                    std::string(strerror(errno)));
			return std::string(temp);
#endif
		}

		FilePath LAMBDACOMMON_API getCurrentWorkingDirectory()
		{
			return FilePath(getCurrentWorkingDirectoryStr());
		}

		FilePath LAMBDACOMMON_API mkdir(const char *path, bool recursive)
		{
			return mkdir(std::string(path), recursive);
		}

		FilePath LAMBDACOMMON_API mkdir(const std::string &path, bool recursive)
		{
			FilePath result{path};
			result.mkdir(recursive);
			return result;
		}
	}
}

#undef STAT_STRUCT
#undef STAT_METHOD