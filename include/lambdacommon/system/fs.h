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
#include "../types.h"
#include <memory>
#include <system_error>
#include <utility>

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4275)
#endif

namespace lambdacommon
{
    namespace fs
    {
        /*! @brief The type of a file.
         *
         * file_type defines constants that indicate a type of a file or directory a path refers to. The value of the enumerators are distinct.
         */
        enum class file_type : i8
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

        /*! @brief Represents file access permissions.
         *
         * This type represents file access permissions. perms satisfies the requirements of `BitmaskType`.
         */
        enum class perms : u16
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

        constexpr perms operator&(perms x, perms y) noexcept {
            using underlying_type = typename std::underlying_type<perms>::type;
            return static_cast<perms>(static_cast<underlying_type>(x) & static_cast<underlying_type>(y));
        }

        constexpr perms operator|(perms x, perms y) noexcept {
            using underlying_type = typename std::underlying_type<perms>::type;
            return static_cast<perms>(static_cast<underlying_type>(x) | static_cast<underlying_type>(y));
        }

        constexpr perms operator^(perms x, perms y) noexcept {
            using underlying_type = typename std::underlying_type<perms>::type;
            return static_cast<perms>(static_cast<underlying_type>(x) ^ static_cast<underlying_type>(y));
        }

        constexpr perms operator~(perms self) noexcept {
            return static_cast<perms>(~static_cast<typename std::underlying_type<perms>::type>(self));
        }

        inline perms& operator&=(perms& self, perms other) noexcept { return self = self & other; }

        inline perms& operator|=(perms& self, perms other) noexcept { return self = self | other; }

        inline perms& operator^=(perms& self, perms other) noexcept { return self = self ^ other; }

        enum class perm_options : u16
        {
            replace = 3,
            add = 1,
            remove = 2,
            nofollow = 3
        };

        constexpr perm_options operator&(perm_options x, perm_options y) noexcept {
            using underlying_type = typename std::underlying_type<perm_options>::type;
            return static_cast<perm_options>(static_cast<underlying_type>(x) & static_cast<underlying_type>(y));
        }

        constexpr perm_options operator|(perm_options x, perm_options y) noexcept {
            using underlying_type = typename std::underlying_type<perm_options>::type;
            return static_cast<perm_options>(static_cast<underlying_type>(x) | static_cast<underlying_type>(y));
        }

        constexpr perm_options operator^(perm_options x, perm_options y) noexcept {
            using underlying_type = typename std::underlying_type<perm_options>::type;
            return static_cast<perm_options>(static_cast<underlying_type>(x) ^ static_cast<underlying_type>(y));
        }

        constexpr perm_options operator~(perm_options self) noexcept {
            return static_cast<perm_options>(~static_cast<typename std::underlying_type<perm_options>::type>(self));
        }

        inline perm_options& operator&=(perm_options& self, perm_options other) noexcept { return self = self & other; }

        inline perm_options& operator|=(perm_options& self, perm_options other) noexcept { return self = self | other; }

        inline perm_options& operator^=(perm_options& self, perm_options other) noexcept { return self = self ^ other; }

        struct file_status
        {
            file_type type;
            perms prms;
        };

        /*! @brief Information about free and available space on the filesystem.
         *
         * Represents the filesystem information as determined by `space`.
         * The members have the following meaning:
         *  - `capacity` -- Total size of the filesystem, in bytes.
         *  - `free` -- Free space on the filesystem, in bytes.
         *  - `available` -- Free space available to a non-privileged process (may be equal or less than `free`)
         */
        struct space_info
        {
            uintmax_t capacity;
            uintmax_t free;
            uintmax_t available;
        };

        /*!
         * Checks whether the given file status corresponds to a directory.
         * @return True if the file status corresponds to a directory, else false.
         */
        inline bool is_directory(const file_status& ft) noexcept {
            return ft.type == file_type::directory;
        }

        /*!
         * Checks whether the given file status corresponds to a regular file.
         * @return True if file status corresponds to a regular file, else false.
         */
        inline bool is_file(const file_status& ft) noexcept {
            return ft.type == file_type::regular;
        }

        /*!
         * Checks whether the given file status corresponds to a symlink.
         * @return True if the file status corresponds to a symlink, else false.
         */
        inline bool is_symlink(const file_status& ft) noexcept {
            return ft.type == file_type::symlink;
        }

        using namespace std::rel_ops;

        /*! @brief Represents a path.
         *
         * Objects of type FilePath represent paths on a filesystem. Only syntactic aspects of paths are handled: the pathname may represent a non-existing path or even one that is not allowed to exist on the current file system or OS.
         */
        class LAMBDACOMMON_API path : public Path
        {
        public:
#ifdef LAMBDA_WINDOWS
            typedef wchar_t value_type;
            static constexpr value_type preferred_separator = L'\\';
#else
            typedef char value_type;
            static constexpr value_type preferred_separator = '/';
#endif
            typedef std::basic_string<value_type> string_type;

        private:
            string_type _path;

        public:
            path() = default;

            path(std::string path);

            explicit path(std::wstring path);

            path(const path& other);

            path(path&& other) noexcept;

            path& assign(string_type source);

            path& assign(const path& source);

            template<class InputIterator>
            path& assign(InputIterator first, InputIterator last) {
                _path.assign(first, last);
                return *this;
            }

            void clear() noexcept override;

            path& append(const path& path);

            /*!
             * Returns the root name of the generic-format path. If the path (in generic format) does not include root name, returns `FilePath()`.
             * @return The root name of the path.
             */
            [[nodiscard]] path root_name() const;

            /*!
             * Returns the root directory of the generic-format path. If the path (in generic format) does not include root directory, returns `path()`.
             * @return The root directory of the path.
             */
            [[nodiscard]] path root_directory() const;

            /*!
             * Returns the root path of the path. If the path foes not include root path, returns `path()`.
             * Effectively, returns the following `root_name() / root_directory()`.
             * @return The root path of the path.
             */
            [[nodiscard]] path root_path() const;

            [[nodiscard]] path relative_path() const;

            [[nodiscard]] bool empty() const override;

            /*!
             * Checks whether `root_name()` is empty.
             * @return True if the root name is not empty, false otherwise.
             */
            [[nodiscard]] bool has_root_name() const;

            /*!
             * Checks whether `root_directory()` is empty.
             * @return True if the root directory is not empty, false otherwise.
             */
            [[nodiscard]] bool has_root_directory() const;

            /*!
             * Checks whether `root_path()` is empty.
             * @return True if the root path is not empty, false otherwise.
             */
            [[nodiscard]] bool has_root_path() const;

            /*!
             * Checks whether `relative_path()` is empty.
             * @return True if the relative path is not empty, false otherwise.
             */
            [[nodiscard]] bool has_relative_path() const;

            [[nodiscard]] bool has_filename() const;

            [[nodiscard]] bool is_absolute() const override;

            class LAMBDACOMMON_API iterator;

            using const_iterator = iterator;

            /*!
             * Returns an iterator to the first element of the path. If the path is empty, the returned iterator is equal to `end()`.
             * @return Iterator to the first element of the path.
             */
            [[nodiscard]] iterator begin() const;

            /*!
             * Returns an iterator one past the last element of the path.
             * @return Iterator one past the end of the path.
             */
            [[nodiscard]] iterator end() const;

            [[nodiscard]] std::string to_string() const override;

            [[nodiscard]] std::wstring to_wstring() const override;

            [[nodiscard]] std::string to_generic_string() const;

            /*!
             * Gets the path as the native string type.
             * @return The path as the native string type.
             */
            [[nodiscard]] const string_type& native() const noexcept;

            /*!
             * Gets the path as a C value.
             * @return The path as a C value.
             */
            [[nodiscard]] const value_type* c_str() const noexcept;

            /*!
             * Makes a new instance of FilePath with the absolute path.
             * @return The absolute path.
             */
            [[nodiscard]] path to_absolute() const;

            /*!
             * Makes a new instance of FilePath with the absolute path.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             * @return The absolute path.
             */
            path to_absolute(std::error_code& ec) const;

            /*!
             * Checks whether the path exists or not.
             * @return True if the path exists else false.
             */
            [[nodiscard]] bool exists() const;

            /*!
             * Gets the generic-format filename component of the path.
             * @return The filename identified by the path.
             */
            [[nodiscard]] path get_filename() const;

            /*!
             * Gets the file extension.
             * @return The file extension.
             */
            [[nodiscard]] path get_extension() const;

            /*!
             * Determines the type and attributes of the filesystem object.
             * @return The file status.
             */
            [[nodiscard]] file_status status() const;

            /*!
             * Determines the type and attributes of the filesystem object.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             * @return The file status.
             */
            file_status status(std::error_code& ec) const noexcept;

            /*!
             * Determines the type and attributes of the filesystem object.
             * @return The file status.
             */
            [[nodiscard]] file_status symlink_status() const;

            /*!
             * Determines the type and attributes of the filesystem object.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             * @return The file status.
             */
            file_status symlink_status(std::error_code& ec) const noexcept;

            /*!
             * Gets the file type.
             * @return The file type.
             */
            [[nodiscard]] file_type get_file_type() const;

            /*!
             * Gets the file type.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             * @return The file type.
             */
            file_type get_file_type(std::error_code& ec) const noexcept;

            /*! @brief Returns the size of a file.
             *
             * For a regular file, returns the size determined as if by reading the `st_size` member of the structure obtained by POSIX stat (symlinks are followed).
             * The result of attempting to determine the size of a directory (as well as any other file that is not a regular file or a symlink) is implementation-defined.
             * @return The size of the file, in bytes.
             */
            [[nodiscard]] uintmax_t file_size() const;

            /*! @brief Returns the size of a file.
             *
             * For a regular file, returns the size determined as if by reading the `st_size` member of the structure obtained by POSIX stat (symlinks are followed).
             * The result of attempting to determine the size of a directory (as well as any other file that is not a regular file or a symlink) is implementation-defined.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             * @return The size of the file, in bytes.
             */
            uintmax_t file_size(std::error_code& ec) const noexcept;

            /*! @brief Get the time of the last data modification.
             *
             * Returns the time of the last modification of this path (symlinks are followed).
             * @return The time of the last modification.
             */
            [[nodiscard]] file_time_type last_write_time() const;

            /*! @brief Get the time of the last data modification.
             *
             * Returns the time of the last modification of this path (symlinks are followed).
             * @param ec Out-parameter for error reporting.
             * @return The time of the last modification or `file_time_type::min()` on errors.
             */
            file_time_type last_write_time(std::error_code& ec) const noexcept;

            /*! @brief Modifies file access permissions.
             *
             * Changes access permissions of the file to which the path resolves. Symlinks are followed unless {@code perm_options::nofollow} is set in opts.
             * @param prms Permissions to set, add, or remove.
             * @param opts Options controlling the action taken by this function.
             */
            void permissions(perms prms, perm_options opts = perm_options::replace) const;

            /*! @brief Modifies file access permissions.
             *
             * Changes access permissions of the file to which the path resolves. Symlinks are followed.
             * @param perms Permissions to set, add, or remove.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             */
            inline void permissions(perms prms, std::error_code& ec) const noexcept {
                this->permissions(prms, perm_options::replace, ec);
            }

            /*! @brief Modifies file access permissions.
             *
             * Changes access permissions of the file to which the path resolves. Symlinks are followed unless {@code perm_options::nofollow} is set in opts.
             * @param prms Permissions to set, add, or remove.
             * @param opts Options controlling the action taken by this function.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             */
            void permissions(perms prms, perm_options opts, std::error_code& ec) const;

            /*! @brief Obtains the target of a symbolic link.
             *
             * If the path refers to a symbolic link, returns a new path object which refers to the target of that symbolic link.
             * It is an error if the path does not refer to a symbolic link.
             * The non-throwing overload returns an empty path on errors.
             * @return The target of the symlink (which may not necessarily exist).
             */
            [[nodiscard]] path read_symlink() const;

            /*! @brief Obtains the target of a symbolic link.
             *
             * If the path refers to a symbolic link, returns a new path object which refers to the target of that symbolic link.
             * It is an error if the path does not refer to a symbolic link.
             * @param ec Out-parameter for error reporting in the non-throwing overload.
             * @return The target of the symlink (which may not necessarily exist) if success, else an empty path on errors.
             */
            path read_symlink(std::error_code& ec) const;

            bool mkdir(perms prms = perms::all) const;

            bool mkdir(std::error_code& ec) const noexcept;

            bool mkdir(perms prms, std::error_code& ec) const noexcept;

            bool mkdirs() const;

            bool mkdirs(std::error_code& ec) const noexcept;

            /*! @brief Moves or renames a file or directory.
             *
             * Moves or renames this filesystem object to `new_path` as if the POSIX rename.
             * Rename fails if
             *   - `new_path` ends with dot or with dot-dot.
             *   - `new_path` names a non-existing directory ending with a directory separator.
             *   - this path is a directory which is an ancestor of `new_path`.
             * @param new_path Target path for the move/rename operation.
             */
            void move(const path& new_path) const;

            /*! @brief Moves or renames a file or directory.
             *
             * Moves or renames this filesystem object to `new_path` as if the POSIX rename.
             * Rename fails if
             *   - `new_path` ends with dot or with dot-dot.
             *   - `new_path` names a non-existing directory ending with a directory separator.
             *   - this path is a directory which is an ancestor of `new_path`.
             * @param new_path Target path for the move/rename operation.
             * @param ec Out-parameter for error reporting.
             */
            void move(const path& new_path, std::error_code& ec) const noexcept;

            /*! @brief Changes the size of a regular file by truncation or zero-fill.
             *
             * Changes the size of the regular file as if by POSIX truncate.
             * @param size Size that the file will now have.
             */
            void resize_file(uintmax_t size) const;

            /*! @brief Changes the size of a regular file by truncation or zero-fill.
             *
             * Changes the size of the regular file as if by POSIX truncate.
             * @param size Size that the file will now have.
             * @param ec Out-parameter for error reporting.
             */
            void resize_file(uintmax_t size, std::error_code& ec) const noexcept;

            /*!
             * Checks whether the path points to a directory.
             * @return True if the path points to a directory, else false.
             */
            [[nodiscard]] bool is_directory() const;

            /*!
             * Checks whether the path points to a regular file.
             * @return True if the path points to a regular file, else false.
             */
            [[nodiscard]] bool is_file() const;

            /*!
             * Checks whether the path points to a symlink.
             * @return True if the path points to a symlink, else false.
             */
            [[nodiscard]] bool is_symlink() const;

            /*!
             * Gets the size of the file in bytes.
             * @return The size of the file.
             */
            [[nodiscard]] size_t get_size() const;

            /*! @brief Returns the number of hard links referring to the specific file.
             *
             * Returns the number of hard links for the filesystem object.
             *
             * @return The number of hard links.
             */
            [[nodiscard]] size_t hard_link_count() const;

            /*! @brief Returns the number of hard links referring to the specific file.
             *
             * Returns the number of hard links for the filesystem object.
             *
             * @param ec Out-parameter for error reporting.
             * @return The number of hard links, or `static_cast<size_t>(-1)` on errors.
             */
            size_t hard_link_count(std::error_code& ec) const noexcept;

            /*!
             * Deletes the file or the directory.
             */
            bool remove() const;

            bool remove(std::error_code& ec) const noexcept;

            uintmax_t remove_all() const;

            uintmax_t remove_all(std::error_code& ec) const noexcept;

            bool operator==(const path& other) const;

            bool operator<(const path& other) const;

            path& operator=(const path& other);

            path& operator=(path&& other) noexcept;

            path& operator=(string_type source);

            explicit operator string_type() const;

            path& operator/=(const path& other);

            path& operator/=(const std::string& other);

            friend const path operator/(path self, const path& to_append) {
                self /= to_append;
                return self;
            }

            friend const path operator/(path self, const std::string& to_append) {
                self /= to_append;
                return self;
            }
        };

        class LAMBDACOMMON_API path::iterator
        {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = const path;
            using difference_type = std::ptrdiff_t;
            using pointer = const path*;
            using reference = const path&;

            iterator();

            iterator(const string_type::const_iterator& first, const string_type::const_iterator& last, const string_type::const_iterator& pos);

            iterator& operator++();

            iterator operator++(int);

            iterator& operator--();

            iterator operator--(int);

            bool operator==(const iterator& other) const;

            reference operator*() const;

            pointer operator->() const;

        private:
            void update_current();

            string_type::const_iterator increment(const string_type::const_iterator& pos) const;

            string_type::const_iterator decrement(const string_type::const_iterator& pos) const;

            string_type::const_iterator _first;
            string_type::const_iterator _last;
            string_type::const_iterator _root;
            string_type::const_iterator _pos;
            path _current;
        };

        /*!
         * Represents an exception object that is thrown on failure by the throwing overloads of the functions in the filesystem library.
         */
        class LAMBDACOMMON_API filesystem_error : public std::system_error
        {
        private:
            std::string _msg;
            path _p1, _p2;

        public:
            filesystem_error(const std::string& msg, std::error_code ec);

            filesystem_error(const std::string& msg, const path& p1, std::error_code ec);

            filesystem_error(const std::string& msg, const path& p1, const path& p2, std::error_code ec);

            [[nodiscard]] const path& path1() const noexcept;

            [[nodiscard]] const path& path2() const noexcept;
        };

        class LAMBDACOMMON_API directory_entry
        {
        private:
            friend class directory_iterator;

            path _path = {};

        public:
            directory_entry() noexcept = default;

            explicit directory_entry(path p);

            directory_entry(const directory_entry&) = default;

            directory_entry(directory_entry&&) noexcept = default;

            // Modifiers
            void assign(path p);

            // Observers
            [[nodiscard]] const path& get_path() const noexcept;

            operator const path&() const noexcept;

            [[nodiscard]] file_status status() const;

            file_status status(std::error_code& ec) const noexcept;

            [[nodiscard]] file_status symlink_status() const;

            file_status symlink_status(std::error_code& ec) const noexcept;

            // Operators
            bool operator==(const directory_entry& other) const;

            bool operator<(const directory_entry& other) const;

            const path* operator->() const;

            // Assignments
            directory_entry& operator=(const directory_entry&) = default;

            directory_entry& operator=(directory_entry&&) = default;
        };

        class LAMBDACOMMON_API directory_iterator
        {
        private:
            class impl;

            std::shared_ptr<impl> _impl;

        public:
            class proxy
            {
            private:
                directory_entry _dir_entry;

                friend class directory_iterator;

                explicit proxy(directory_entry dir_entry) : _dir_entry(std::move(dir_entry)) {}

            public:
                const directory_entry& operator*() const& noexcept { return _dir_entry; }

                directory_entry operator*()&& noexcept { return std::move(_dir_entry); }
            };

            using iterator_category = std::input_iterator_tag;
            using value_type = directory_entry;
            using difference_type = std::ptrdiff_t;
            using pointer = const directory_entry*;
            using reference = const directory_entry&;

            directory_iterator() noexcept;

            explicit directory_iterator(path p);

            directory_iterator(path p, std::error_code& ec) noexcept;

            directory_iterator(const directory_iterator&) = default;

            directory_iterator(directory_iterator&&) noexcept = default;

            directory_iterator& operator++();

            directory_iterator& increment(std::error_code& ec) noexcept;

            proxy operator++(int) {
                proxy proxy{**this};
                ++*this;
                return proxy;
            }

            // Operators
            bool operator==(const directory_iterator& other) const;

            bool operator!=(const directory_iterator& other) const;

            reference operator*() const;

            pointer operator->() const;

            void swap(directory_iterator& other);

            // Assignements
            directory_iterator& operator=(const directory_iterator& other);

            directory_iterator& operator=(directory_iterator&& other) noexcept;
        };

        inline directory_iterator begin(directory_iterator iter) noexcept {
            return std::move(iter);
        }

        inline directory_iterator end(const directory_iterator&) noexcept {
            return directory_iterator();
        }

        /*! @brief Creates a symbolic link.
         *
         * Creates a symbolic link with its target set to target as if by POSIX `symlink()`: the pathname target may be invalid or non-existing.
         * @param target Path to point the symlink to, does not have to exist.
         * @param link Path of the new symbolic link.
         */
        extern void LAMBDACOMMON_API create_symlink(const path& target, const path& link);

        /*! @brief Creates a symbolic link.
         *
         * Creates a symbolic link with its target set to target as if by POSIX `symlink()`: the pathname target may be invalid or non-existing.
         * @param target Path to point the symlink to, does not have to exist.
         * @param link Path of the new symbolic link.
         * @param ec Out-parameter for error reporting in the non-throwing overload.
         */
        extern void LAMBDACOMMON_API create_symlink(const path& target, const path& link, std::error_code& ec) noexcept;

        /*! @brief Creates a hard link.
         *
         * Creates a hard link with its target set to target as if by POSIX `link()`: the pathname target must exist.
         *
         * Once created, link and target are two logical names that refer to the same file (they are equivalent). Even if the original name target is deleted, the file continues to
         * exist and is accessible as link.
         *
         * @param target Path of the file or directory to link to.
         * @param link Path of the new hard link.
         */
        extern void LAMBDACOMMON_API create_hardlink(const path& target, const path& link);

        /*! @brief Creates a hard link.
         *
         * Creates a hard link with its target set to target as if by POSIX `link()`: the pathname target must exist.
         *
         * Once created, link and target are two logical names that refer to the same file (they are equivalent). Even if the original name target is deleted, the file continues to
         * exist and is accessible as link.
         *
         * @param target Path of the file or directory to link to.
         * @param link Path of the new hard link.
         * @param ec Out-parameter for error reporting in the non-throwing overload.
         */
        extern void LAMBDACOMMON_API create_hardlink(const path& target, const path& link, std::error_code& ec) noexcept;

        /*! @brief Checks whether two paths refer to the same filesystem object.
         *
         * Checks whether the paths path1 and path2 resolve to the same filesystem entity.
         *
         * If either one of the given path does not exist, an error is reported.
         *
         * @param path1 Path to check for equivalence.
         * @param path2 Path to check for equivalence.
         * @return True if the path1 and path2 refer to the same file or directory and their file status is the same, false otherwise.
         */
        extern bool LAMBDACOMMON_API equivalent(const path& path1, const path& path2);

        /*! @brief Checks whether two paths refer to the same filesystem object.
         *
         * Checks whether the paths path1 and path2 resolve to the same filesystem entity.
         *
         * If either one of the given path does not exist, an error is reported.
         *
         * @param path1 Path to check for equivalence.
         * @param path2 Path to check for equivalence.
         * @param ec Out-parameter for error reporting in the non-throwing overload.
         * @return True if the path1 and path2 refer to the same file or directory and their file status is the same, false otherwise.
         */
        extern bool LAMBDACOMMON_API equivalent(const path& path1, const path& path2, std::error_code& ec) noexcept;

        /*! @brief Copies a symbolic link.
         *
         * Copies a symlink to another location.
         *
         * @param from Path to a symbolic link to copy.
         * @param to Destination path of the new symlink.
         */
        extern void LAMBDACOMMON_API copy_symlink(const path& from, const path& to);

        /*! @brief Copies a symbolic link.
        *
        * Copies a symlink to another location.
        *
        * @param from Path to a symbolic link to copy.
        * @param to Destination path of the new symlink.
        * @param ec Out-parameter for error reporting in the non-throwing overload.
        */
        inline void copy_symlink(const path& from, const path& to, std::error_code& ec) noexcept {
            ec.clear();
            auto original = from.read_symlink(ec);
            if (!ec)
                create_symlink(original, to, ec);
        }

        inline void move(const path& old_path, const path& new_path) {
            old_path.move(new_path);
        }

        inline void move(const path& old_path, const path& new_path, std::error_code& ec) noexcept {
            old_path.move(new_path, ec);
        }

        /*! @brief Determines available free space on the filesystem.
         *
         * Determines the information about the filesystem on which the pathname `p` is located.
         * Populates and returns an object of type `space_info`.
         * @param p Path to examine.
         * @return The filesystem information (a `space_info` object).
         */
        extern space_info LAMBDACOMMON_API space(const path& p);

        /*! @brief Determines available free space on the filesystem.
         *
         * Determines the information about the filesystem on which the pathname `p` is located.
         * Populates and returns an object of type `space_info`.
         * All members may be set to `static_cast<uintmax_t>(-1)` on error.
         * @param p Path to examine.
         * @param ec Out-parameter for error reporting in the non-throwing overload.
         * @return The filesystem information (a `space_info` object).
         */
        extern space_info LAMBDACOMMON_API space(const path& p, std::error_code& ec) noexcept;

        /*! @brief Returns a directory suitable for temporary files.
         *
         * Returns a directory suitable for temporary files.
         * @return A directory suitable for temporary files. The path is guaranteed to exist and to be a directory. The overload takes `error_code&` argument returns an empty path on error.
         */
        extern path LAMBDACOMMON_API temp_directory_path();

        /*! @brief Returns a directory suitable for temporary files.
         *
         * Returns a directory suitable for temporary files.
         * @param ec Out-parameter for error reporting in the non-throwing overload.
         * @return A directory suitable for temporary files. The path is guaranteed to exist and to be a directory. The overload takes `error_code&` argument returns an empty path on error.
         */
        extern path LAMBDACOMMON_API temp_directory_path(std::error_code& ec) noexcept;

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
        extern path LAMBDACOMMON_API current_path();
    }
}

#ifdef LAMBDA_WINDOWS
#  pragma warning(pop)
#endif

#endif //LAMBDACOMMON_FS_H
