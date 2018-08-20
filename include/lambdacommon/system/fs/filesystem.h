/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_FILESYSTEM_H
#define LAMBDACOMMON_FILESYSTEM_H

#include "../../path.h"
#include <vector>

namespace lambdacommon
{
	namespace fs
	{
		enum PathType
		{
			WINDOWS = 1,
			COMMON = 0,
#ifdef LAMBDA_WINDOWS
			NATIVE = WINDOWS
#else
			NATIVE = COMMON
#endif
		};

		class LAMBDACOMMON_API FilePath : public Path
		{
		protected:
			//std::vector<std::string> *_path;
			bool _absolute;

		public:
			FilePath();

			FilePath(const char *path);

			FilePath(const std::string &path);

#ifdef LAMBDA_WINDOWS

			FilePath(const wchar_t *path);

			FilePath(const std::wstring &path);

#endif

			FilePath(const FilePath &path);

			FilePath(FilePath &&path) noexcept;

			void set(const std::string &str, PathType type = NATIVE);

			/**
			 * Checks whether the path is absolute or not.
			 * @return True if the path is absolute else false.
			 */
			bool isAbsolute() const;

			bool empty() const;

			bool remove() const;

			bool mkdir(bool recursive = false) const;

			/**
			 * Checks whether the path exists or not.
			 * @return True if the path exists else false.
			 */
			bool exists() const;

			bool isDirectory() const;

			bool isFile() const;

			std::string getFileName() const;

			std::string getExtension() const;

			size_t getFileSize() const;

			FilePath toAbsolute() const;

			/**
			 * Gets the parent path.
			 * @return Parent path.
			 */
			FilePath getParent() const;

			std::string toString(PathType type = NATIVE) const;

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

#ifdef LAMBDA_WINDOWS

		extern std::wstring LAMBDACOMMON_API getCurrentWorkingDirectoryWStr();

#endif

		extern std::string LAMBDACOMMON_API getCurrentWorkingDirectoryStr();

		extern FilePath LAMBDACOMMON_API getCurrentWorkingDirectory();

		extern FilePath LAMBDACOMMON_API mkdir(const char *path, bool recursive = false);

		extern FilePath LAMBDACOMMON_API mkdir(const std::string &path, bool recursive = false);
	}
}

#endif //LAMBDACOMMON_FILESYSTEM_H