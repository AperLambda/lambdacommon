/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_FS_H
#define LAMBDACOMMON_FS_H

#include "../path.h"
#include <system_error>

namespace lambdacommon
{
    namespace fs
    {
        enum class file_type : int8_t
        {
            none = 0,
            not_found = -1,
            regular = 1,
            directory = 2,
            symlink = 3,
            block = 4,
            character = 5,
            fifo = 6,
            socket = 7,
            unknown = 8
        };

        /*!
         * Represents file access permissions.
         * Is `BitmaskType` compliant.
         */
        enum class perms : uint16_t
        {
            none = 0,
            owner_read = 0400,
            owner_write = 0200,
            owner_exec = 0100,
            owner_all = 0700,
            group_read = 040,
            group_write = 020,
            group_exec = 010,
            group_all = 070,
            others_read = 04,
            others_write = 02,
            others_exec = 01,
            others_all = 07,
            all = 0777,
            set_uid = 04000,
            set_gid = 02000,
            sticky_bit = 01000,
            mask = 07777,
            unknown = 0xFFFF
        };

        constexpr perms operator&(perms x, perms y) noexcept
        {
            using underlying_type = typename std::underlying_type<perms>::type;
            return static_cast<perms>(static_cast<underlying_type>(x) & static_cast<underlying_type>(y));
        }

        constexpr perms operator|(perms x, perms y) noexcept
        {
            using underlying_type = typename std::underlying_type<perms>::type;
            return static_cast<perms>(static_cast<underlying_type>(x) | static_cast<underlying_type>(y));
        }

        constexpr perms operator^(perms x, perms y) noexcept
        {
            using underlying_type = typename std::underlying_type<perms>::type;
            return static_cast<perms>(static_cast<underlying_type>(x) ^ static_cast<underlying_type>(y));
        }

        constexpr perms operator~(perms self) noexcept
        {
            return static_cast<perms>(~static_cast<typename std::underlying_type<perms>::type>(self));
        }

        inline perms &operator&=(perms &self, perms other) noexcept
        { return self = self & other; }

        inline perms &operator|=(perms &self, perms other) noexcept
        { return self = self | other; }

        inline perms &operator^=(perms &self, perms other) noexcept
        { return self = self ^ other; }

        enum class perm_options : uint16_t
        {
            replace = 3,
            add = 1,
            remove = 2,
            nofollow = 3
        };

        constexpr perm_options operator&(perm_options x, perm_options y) noexcept
        {
            using underlying_type = typename std::underlying_type<perm_options>::type;
            return static_cast<perm_options>(static_cast<underlying_type>(x) & static_cast<underlying_type>(y));
        }

        constexpr perm_options operator|(perm_options x, perm_options y) noexcept
        {
            using underlying_type = typename std::underlying_type<perm_options>::type;
            return static_cast<perm_options>(static_cast<underlying_type>(x) | static_cast<underlying_type>(y));
        }

        constexpr perm_options operator^(perm_options x, perm_options y) noexcept
        {
            using underlying_type = typename std::underlying_type<perm_options>::type;
            return static_cast<perm_options>(static_cast<underlying_type>(x) ^ static_cast<underlying_type>(y));
        }

        constexpr perm_options operator~(perm_options self) noexcept
        {
            return static_cast<perm_options>(~static_cast<typename std::underlying_type<perm_options>::type>(self));
        }

        inline perm_options &operator&=(perm_options &self, perm_options other) noexcept
        { return self = self & other; }

        inline perm_options &operator|=(perm_options &self, perm_options other) noexcept
        { return self = self | other; }

        inline perm_options &operator^=(perm_options &self, perm_options other) noexcept
        { return self = self ^ other; }

        struct file_status
        {
            file_type type;
            perms prms;
        };

        using namespace std::rel_ops;

        class LAMBDACOMMON_API FilePath : public Path
        {
        public:
#ifdef LAMBDA_WINDOWS
            typedef wchar_t	value_type;
            static constexpr value_type preferred_separator = L'\\';
#else
            typedef char value_type;
            static constexpr value_type preferred_separator = '/';
#endif
            typedef std::basic_string<value_type> string_type;

        private:
            string_type _path;

        public:
            FilePath();

            FilePath(std::string path);

            FilePath(std::wstring path);

            FilePath(const FilePath &other);

            FilePath(FilePath &&other) noexcept;

            FilePath &assign(string_type source);

            FilePath &assign(const FilePath &source);

            template<class InputIterator>
            FilePath &assign(InputIterator first, InputIterator last)
            {
                _path.assign(first, last);
                return *this;
            }

            void clear() noexcept override;

            FilePath &append(const FilePath &path);

            /*!
             * Returns the root name of the generic-format path. If the path (in generic format) does not include root name, returns `FilePath()`.
             * @return The root name of the path.
             */
            FilePath root_name() const;

            /*!
             * Returns the root directory of the generic-format path. If the path (in generic format) does not include root directory, returns `path()`.
             * @return The root directory of the path.
             */
            FilePath root_directory() const;

            /*!
             * Returns the root path of the path. If the path foes not include root path, returns `path()`.
             * Effectively, returns the following `root_name() / root_directory()`.
             * @return The root path of the path.
             */
            FilePath root_path() const;

            FilePath relative_path() const;

            bool empty() const override;

            /*!
             * Checks whether `root_name()` is empty.
             * @return True if the root name is not empty, false otherwise.
             */
            bool has_root_name() const;

            /*!
             * Checks whether `root_directory()` is empty.
             * @return True if the root directory is not empty, false otherwise.
             */
            bool has_root_directory() const;

            /*!
             * Checks whether `root_path()` is empty.
             * @return True if the root path is not empty, false otherwise.
             */
            bool has_root_path() const;

            /*!
             * Checks whether `relative_path()` is empty.
             * @return True if the relative path is not empty, false otherwise.
             */
            bool has_relative_path() const;

            bool has_filename() const;

            bool is_absolute() const override;

            class LAMBDACOMMON_API iterator;

            using const_iterator = iterator;

            /*!
             * Returns an iterator to the first element of the path. If the path is empty, the returned iterator is equal to `end()`.
             * @return Iterator to the first element of the path.
             */
            iterator begin() const;

            /*!
             * Returns an iterator one past the last element of the path.
             * @return Iterator one past the end of the path.
             */
            iterator end() const;

            /*!
             * Makes a new instance of FilePath with the absolute path.
             * @return The absolute path.
             */
            FilePath to_absolute() const;

            FilePath to_absolute(std::error_code &ec) const;

            std::string to_string() const override;

            std::wstring to_wstring() const override;

            std::string to_generic_string() const;

            /*!
             * Gets the path as the native string type.
             * @return The path as the native string type.
             */
            const string_type &native() const noexcept;

            /*!
             * Gets the path as a C value.
             * @return The path as a C value.
             */
            const value_type *c_str() const noexcept;

            /*!
             * Checks whether the path exists or not.
             * @return True if the path exists else false.
             */
            bool exists() const;

            /*!
             * Gets the generic-format filename component of the path.
             * @return The filename identified by the path.
             */
            FilePath get_filename() const;

            /*!
             * Gets the file extension.
             * @return The file extension.
             */
            FilePath get_extension() const;

            /*!
             * Determines the type and attributes of the filesystem object.
             * @return The file status.
             */
            file_status status() const;

            /*!
             * Determines the type and attributes of the filesystem object.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             * @return The file status.
             */
            file_status status(std::error_code &ec) const noexcept;

            /*!
             * Determines the type and attributes of the filesystem object.
             * @return The file status.
             */
            file_status symlink_status() const;

            /*!
             * Determines the type and attributes of the filesystem object.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             * @return The file status.
             */
            file_status symlink_status(std::error_code &ec) const noexcept;

            /*!
             * Gets the file type.
             * @return The file type.
             */
            file_type get_file_type() const;

            /*!
             * Gets the file type.
             * @param ec Error code.
             * @return The file type.
             */
            file_type get_file_type(std::error_code &ec) const noexcept;

            /*! @brief Modifies file access permissions.
             *
             * Changes access permissions of the file to which the path resolves. Symlinks are followed unless {@code perm_options::nofollow} is set in opts.
             * @param prms Permissions to set, add, or remove.
             * @param opts Options controlling the action taken by this function.
             */
            void permissions(perms prms, perm_options opts = perm_options::replace);

            /*! @brief Modifies file access permissions.
             *
             * Changes access permissions of the file to which the path resolves. Symlinks are followed.
             * @param perms Permissions to set, add, or remove.
             * @param ec Error code.
             */
            inline void permissions(perms prms, std::error_code &ec) noexcept
            {
                this->permissions(prms, perm_options::replace, ec);
            }

            /*! @brief Modifies file access permissions.
             *
             * Changes access permissions of the file to which the path resolves. Symlinks are followed unless {@code perm_options::nofollow} is set in opts.
             * @param prms Permissions to set, add, or remove.
             * @param opts Options controlling the action taken by this function.
             * @param ec Error code.
             */
            void permissions(perms prms, perm_options opts, std::error_code &ec);

            /*! @brief Obtains the target of a symbolic link.
             *
             * If the path refers to a symbolic link, returns a new path object which refers to the target of that symbolic link.
             * It is an error if the path does not refer to a symbolic link.
             * The non-throwing overload returns an empty path on errors.
             * @return The target of the symlink (which may not necessarily exist).
             */
            FilePath read_symlink() const;

            /*! @brief Obtains the target of a symbolic link.
             *
             * If the path refers to a symbolic link, returns a new path object which refers to the target of that symbolic link.
             * It is an error if the path does not refer to a symbolic link.
             * @param ec Error code.
             * @return The target of the symlink (which may not necessarily exist) if success, else an empty path on errors.
             */
            FilePath read_symlink(std::error_code &ec) const;

            /*!
             * Checks whether the path points to a directory.
             * @return True if the path points to a directory, else false.
             */
            bool is_directory() const;

            /*!
             * Checks whether the path points to a regular file.
             * @return True if the path points to a regular file, else false.
             */
            bool is_file() const;

            /*!
             * Checks whether the path points to a symlink.
             * @return True if the path points to a symlink, else false.
             */
            bool is_symlink() const;

            /*!
             * Gets the size of the file in bytes.
             * @return The size of the file.
             */
            size_t get_size() const;

            /*!
             * Deletes the file or the directory.
             * @param recursive True if delete recursively, else false.
             */
            bool remove(bool recursive = false);

            inline bool remove_all()
            { return remove(true); }

            bool operator==(const FilePath &other) const;

            bool operator<(const FilePath &other) const;

            FilePath &operator=(const FilePath &other);

            FilePath &operator=(FilePath &&other) noexcept;

            FilePath &operator=(string_type source);

            operator string_type() const;

            FilePath &operator/=(const FilePath &other);

            FilePath &operator/=(const std::string &other);

            friend const FilePath operator/(FilePath self, const FilePath &to_append)
            {
                self /= to_append;
                return self;
            }

            friend const FilePath operator/(FilePath self, const std::string &to_append)
            {
                self /= to_append;
                return self;
            }
        };

        class LAMBDACOMMON_API FilePath::iterator
        {
        public:
            using value_type = const FilePath;
            using difference_type = std::ptrdiff_t;
            using pointer = const FilePath *;
            using reference = const FilePath &;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator();

            iterator(const string_type::const_iterator &first, const string_type::const_iterator &last, const string_type::const_iterator &pos);

            iterator &operator++();

            iterator operator++(int);

            iterator &operator--();

            iterator operator--(int);

            bool operator==(const iterator &other) const;

            reference operator*() const;

            pointer operator->() const;

        private:
            void update_current();

            string_type::const_iterator increment(const string_type::const_iterator &pos) const;

            string_type::const_iterator decrement(const string_type::const_iterator &pos) const;

            string_type::const_iterator _first;
            string_type::const_iterator _last;
            string_type::const_iterator _root;
            string_type::const_iterator _pos;
            FilePath _current;
        };

        /*!
         * Represents an exception object that is thrown on failure by the throwing overloads of the functions in the filesystem library.
         */
        class filesystem_error : public std::system_error
        {
        private:
            std::string _msg;
            FilePath _p1, _p2;

        public:
            filesystem_error(const std::string &msg, std::error_code ec);

            filesystem_error(const std::string &msg, const FilePath& p1, std::error_code ec);

            filesystem_error(const std::string &msg, const FilePath& p1, const FilePath& p2, std::error_code ec);

            const FilePath& path1() const noexcept;

            const FilePath& path2() const noexcept;
        };

        /*!
         * Gets the current working directory as a wide string.
         * @return The current working directory as a wide string.
         */
        extern std::wstring LAMBDACOMMON_API current_path_wstr();

        /*!
         * Gets the current working directory as a string.
         * @return The current working directory as a string.
         */
        extern std::string LAMBDACOMMON_API current_path_str();

        /*!
         * Gets the current working directory.
         * @return The current working directory.
         */
        extern FilePath LAMBDACOMMON_API current_path();
    }
}

#endif //LAMBDACOMMON_FS_H
