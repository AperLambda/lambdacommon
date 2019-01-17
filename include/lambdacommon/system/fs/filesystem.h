/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_FILESYSTEM_H
#define LAMBDACOMMON_FILESYSTEM_H

#include "../../path.h"
#include "../fs.h"

namespace lambdacommon
{
	namespace fs
	{
		enum PathType
		{
			WINDOWS = 1,
			COMMON  = 0,
#ifdef LAMBDA_WINDOWS
			NATIVE = WINDOWS
#else
			NATIVE = COMMON
#endif
		};

		class LAMBDACOMMON_API FilePath : public Path
		{
		protected:
			bool _absolute;

		public:
			FilePath();

			FilePath(const char *path);

			FilePath(const std::string &path);

			explicit FilePath(const wchar_t *path);

			explicit FilePath(const std::wstring &path);

			FilePath(const FilePath &path);

			FilePath(FilePath &&path) noexcept;

			void set(const std::string &str, PathType type = NATIVE);

			/**
			 * Checks whether the path is absolute or not.
			 * @return True if the path is absolute else false.
			 */
			bool is_absolute() const;

			bool empty() const;

			bool remove() const;

			bool mkdir(bool recursive = false) const;

			/**
			 * Checks whether the path exists or not.
			 * @return True if the path exists else false.
			 */
			bool exists() const;

			bool is_directory() const;

			bool is_file() const;

			std::string get_filename() const;

			std::string get_extension() const;

			size_t get_file_size() const;

			FilePath to_absolute() const;

			/**
			 * Gets the parent path.
			 * @return Parent path.
			 */
			FilePath get_parent() const;

			std::string to_string(PathType type = NATIVE) const;

			FilePath sub(const FilePath &other) const;

			/*
			 * Operators
			 */

			FilePath operator/(const FilePath &other) const;

			FilePath &operator=(const FilePath &path);

			FilePath &operator=(FilePath &&path) noexcept;

			bool operator==(const FilePath &_path) const;

			bool operator!=(const FilePath &_path) const;
		};

		extern FilePath LAMBDACOMMON_API get_current_working_directory();

		extern FilePath LAMBDACOMMON_API mkdir(const char *path, bool recursive = false);

		extern FilePath LAMBDACOMMON_API mkdir(const std::string &path, bool recursive = false);
	}
}

#endif //LAMBDACOMMON_FILESYSTEM_H
