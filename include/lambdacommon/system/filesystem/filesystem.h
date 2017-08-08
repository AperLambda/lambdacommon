/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_PATH_H
#define LAMBDACOMMON_PATH_H

#include "../../lambdacommon.h"
#include <vector>

namespace lambdacommon
{
    namespace filesystem
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

        class LAMBDACOMMON_API Path
        {
        protected:
            PathType _type;
            std::vector<std::string> *_path;
            bool _absolute;

        public:
            Path();

            Path(const char *path);

            Path(std::string path);

#ifdef LAMBDA_WINDOWS

            Path(const wchar_t *path);

            Path(std::wstring path);

#endif

            Path(const Path &path);

            Path(Path &&path);

            ~Path();

            void set(const std::string &str, PathType type = NATIVE);

            bool remove();

            bool mkdir(bool recursive = false);

            bool empty() const;

            /**
             * Checks whether the path is absolute or not.
             * @return True if the path is absolute else false.
             */
            bool isAbsolute() const;

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

            Path toAbsolute() const;

            /**
             * Gets the parent path.
             * @return Parent path.
             */
            Path getParent() const;

            std::string toString(PathType type = NATIVE) const;

            /*
             * Operators
             */

            Path operator/(const Path &other);

            Path &operator=(const Path &path);

            Path &operator=(Path &&path);

            bool operator==(const Path &_path);

            bool operator!=(const Path &_path);
        };

#ifdef LAMBDA_WINDOWS

        extern std::wstring LAMBDACOMMON_API getCurrentWorkingDirectoryWStr();

#endif

        extern std::string LAMBDACOMMON_API getCurrentWorkingDirectoryStr();

        extern Path LAMBDACOMMON_API getCurrentWorkingDirectory();

        extern Path LAMBDACOMMON_API mkdir(const char *path, bool recursive = false);

        extern Path LAMBDACOMMON_API mkdir(std::string path, bool recursive = false);
    }
}

#endif //LAMBDACOMMON_PATH_H
