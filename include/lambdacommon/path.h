/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_PATH_H
#define LAMBDACOMMON_PATH_H

#include "object.h"
#include "serializable.h"

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4251)
#endif

namespace lambdacommon
{
    using namespace std::rel_ops;

    /*!
     * Represents a path.
     */
    class LAMBDACOMMON_API Path : public Object
    {
    public:
        /*!
         * Clears the current path.
         */
        virtual void clear() noexcept = 0;

        /*!
         * Checks whether the path is empty or not.
         * @return True if the path is empty, else false.
         */
        virtual bool empty() const = 0;

        /*!
         * Checks whether the path is absolute or relative.
         * @return True if the path is absolute, else false.
         */
        virtual bool is_absolute() const = 0;
    };

    class LAMBDACOMMON_API AbstractPath : public Path
    {
    protected:
        std::string _path;

    public:
        AbstractPath();

        AbstractPath(std::string path);

        AbstractPath(const AbstractPath &other);

        AbstractPath(AbstractPath &&other) noexcept;

        void append(const std::string &path);

        void append(const AbstractPath &path);

        void clear() noexcept override;

        /*!
         * Gets the path value.
         * @return The path value.
         */
        virtual const std::string &get_path() const;

        bool empty() const override;

        bool is_absolute() const override;

        std::string to_string() const override;

        virtual std::string to_string(char delimiter = '/') const;

        bool operator==(const AbstractPath &other) const;

        bool operator<(const AbstractPath &other) const;
    };
}

#endif //LAMBDACOMMON_PATH_H
