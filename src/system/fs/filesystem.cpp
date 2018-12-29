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
		{}

		FilePath::FilePath(const char *path) : Path()
		{
			set(std::string(path));
		}

		FilePath::FilePath(const std::string &path) : Path()
		{
			set(path);
		}

		FilePath::FilePath(const wchar_t *path) : Path()
		{
			set(lstring::convert_wstring_to_string(std::wstring(path)));
		}

		FilePath::FilePath(const std::wstring &path) : Path()
		{
			set(lstring::convert_wstring_to_string(path));
		}

		FilePath::FilePath(const FilePath &path) : Path(path._path), _absolute(path._absolute)
		{}

		FilePath::FilePath(FilePath &&path) noexcept : Path(path._path), _absolute(path._absolute)
		{}

		void FilePath::set(const std::string &str, PathType type)
		{
			if (type == WINDOWS) {
				_path = serializable::tokenize(str, "/\\");
				_absolute = str.size() > 2 && isalpha(str[0]) && str[1] == ':';
			} else {
				_path = serializable::tokenize(str, "/");
				_absolute = !str.empty() && str[0] == '/';
			}
		}

		bool FilePath::remove() const
		{
#ifdef LAMBDA_WINDOWS
			return DeleteFileA(to_string().c_str()) != 0;
#else
			return std::remove(to_string().c_str()) == 0;
#endif
		}

		bool FilePath::mkdir(bool recursive) const
		{
			if (recursive) {
				auto parent = get_parent();
				if (!parent.exists())
					parent.mkdir(true);
			}
#ifdef LAMBDA_WINDOWS
			return CreateDirectoryA(to_string().c_str(), nullptr) != 0;
#else
			return ::mkdir(to_string().c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
#endif
		}

		bool FilePath::empty() const
		{
			return _path.empty();
		}

		bool FilePath::is_absolute() const
		{
			return _absolute;
		}

		bool FilePath::exists() const
		{
#ifdef LAMBDA_WINDOWS
			return GetFileAttributesA(to_string().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
			struct STAT_STRUCT sb;
			return stat(to_string().c_str(), &sb) == 0;
#endif
		}

		bool FilePath::is_directory() const
		{
#ifdef LAMBDA_WINDOWS
			auto attr = GetFileAttributesA(to_string().c_str());
			return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
			struct STAT_STRUCT sb;
			if (stat(to_string().c_str(), &sb))
				return false;
			return S_ISDIR(sb.st_mode);
#endif
		}

		bool FilePath::is_file() const
		{
#ifdef LAMBDA_WINDOWS
			auto attr = GetFileAttributesA(to_string().c_str());
			return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
			struct STAT_STRUCT sb;
			if (stat(to_string().c_str(), &sb))
				return false;
			return S_ISREG(sb.st_mode);
#endif
		}

		std::string FilePath::get_filename() const
		{
			if (empty())
				return "";
			const std::string &last = _path[_path.size() - 1];
			return last;
		}

		std::string FilePath::get_extension() const
		{
			const std::string &name = get_filename();
			size_t pos = name.find_last_of('.');
			if (pos == std::string::npos)
				return "";
			return name.substr(pos + 1);
		}

		size_t FilePath::get_file_size() const
		{
			if (!exists())
				return 0;
			struct STAT_STRUCT sb{};
			if (STAT_METHOD(to_string().c_str(), &sb) != 0)
				throw std::runtime_error(
						"lambdacommon::path::FilePath.get_file_size(): cannot stat file \"" + to_string() + "\"!");
			return (size_t) sb.st_size;
		}

		FilePath FilePath::to_absolute() const
		{
#ifdef LAMBDA_WINDOWS
			if (to_string().empty())
				return get_current_working_directory();
			char temp[MAX_PATH];
			if (GetFullPathNameA(to_string().c_str(), MAX_PATH, temp, nullptr) == 0)
				throw std::runtime_error(
						"Internal error in lambdacommon::path::FilePath.to_absolute(): " +
						std::to_string(GetLastError()));
			return FilePath(temp);
#else
			char temp[PATH_MAX];
			if (realpath(to_string().c_str(), temp) == nullptr)
				throw std::runtime_error(
						"Internal error in lambdacommon::path::FilePath::to_absolute(): " +
						std::string(strerror(errno)));
			return FilePath(temp);
#endif
		}

		FilePath FilePath::get_parent() const
		{
			FilePath result;
			result._absolute = _absolute;

			if (empty()) {
				if (!_absolute)
					result._path.emplace_back("..");
			} else
				for (size_t i = 0; i < _path.size() - 1; ++i)
					result._path.push_back(_path[i]);
			return result;
		}

		std::string FilePath::to_string(PathType type) const
		{
			std::ostringstream oss;

			if (type == COMMON && _absolute)
				oss << "/";

			for (size_t i = 0; i < _path.size(); i++) {
				oss << _path[i];
				if (i + 1 < _path.size()) {
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
				throw std::runtime_error("lambdacommon->system/fs/filesystem.cpp@FilePath.sub(const FilePath&)(Line " +
										 std::to_string(__LINE__ - 1) + "): Expected a relative path as argument!");

			FilePath result(*this);

			for (const auto &part : other._path)
				result._path.push_back(part);

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
			if (this != &path) {
				_path = path._path;
				_absolute = path._absolute;
			}
			return *this;
		}

		FilePath &FilePath::operator=(FilePath &&path) noexcept
		{
			if (this != &path) {
				_path = std::move(path._path);
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

		FilePath LAMBDACOMMON_API get_current_working_directory()
		{
			return FilePath(get_cwd_str());
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