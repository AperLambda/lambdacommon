/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/system/fs.h"
#include "../../include/lambdacommon/lstring.h"
#include "../../include/lambdacommon/maths.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

#ifdef LAMBDA_WINDOWS
#  include <Windows.h>
#  define STAT_STRUCT _stati64
#  define STAT_METHOD _stati64
#else
#  include <errno.h>
#  define ERROR_INVALID_PARAMETER EINVAL
#  include <unistd.h>
#  include <cstring>
#  include <climits>
#  include <dirent.h>
#  define STAT_STRUCT stat
#  define STAT_METHOD stat
#endif

#include <sys/stat.h>

namespace lambdacommon
{
    namespace fs
    {
        template<typename T>
        inline file_status file_status_from_st_mode(T mode)
        {
#ifdef LAMBDA_WINDOWS
            file_type ft = file_type::unknown;
            if ((mode & _S_IFDIR) == _S_IFDIR) ft = file_type::directory;
            else if ((mode & _S_IFREG) == _S_IFREG) ft = file_type::regular;
            else if ((mode & _S_IFCHR) == _S_IFCHR) ft = file_type::character;
            return {ft, static_cast<perms>(mode & 0xFFF)};
#else
            file_type ft = file_type::unknown;
            if (S_ISDIR(mode)) ft = file_type::directory;
            else if (S_ISREG(mode)) ft = file_type::regular;
            else if (S_ISCHR(mode)) ft = file_type::character;
            else if (S_ISBLK(mode)) ft = file_type::block;
            else if (S_ISFIFO(mode)) ft = file_type::fifo;
            else if (S_ISLNK(mode)) ft = file_type::symlink;
            else if (S_ISSOCK(mode)) ft = file_type::socket;
            return {ft, static_cast<perms>(mode & 0xFFF)};
#endif
        }

#ifdef LAMBDA_WINDOWS

#define FP_ST(val) L##val

        inline file_status status_from_INFO(const path &path, const WIN32_FILE_ATTRIBUTE_DATA *info, std::error_code &ec)
        {
            file_type ft;
            if ((info->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
                ft = file_type::symlink;
            else {
                if ((info->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) ft = file_type::directory;
                else ft = file_type::regular;
            }
            perms prms = perms::owner_read | perms::group_read | perms::others_read;
            if (!(info->dwFileAttributes & FILE_ATTRIBUTE_READONLY))
                prms = prms | perms::owner_write | perms::group_write | perms::others_write;
            std::string ext = path.get_extension().to_generic_string();
            if (lstring::equals_ignore_case(ext, ".exe") || lstring::equals_ignore_case(ext, ".cmd") || lstring::equals_ignore_case(ext, ".bat") ||
                lstring::equals_ignore_case(ext, ".com"))
                prms = prms | perms::owner_exec | perms::group_exec | perms::others_exec;
            return {ft, prms};
        }

#else

#define FP_ST(val) val

#endif

        inline bool is_not_found_error(const std::error_code &ec)
        {
#ifdef LAMBDA_WINDOWS
            return ec.value() == ERROR_FILE_NOT_FOUND || ec.value() == ERROR_PATH_NOT_FOUND;
#else
            return ec.value() == ENOENT;
#endif
        }

        template<typename ErrorNumber>
        inline std::string get_sys_error_txt(ErrorNumber code = 0)
        {
#ifdef LAMBDA_WINDOWS
            LPVOID msg_buf;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, static_cast<DWORD>(code ? code : ::GetLastError()),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &msg_buf, 0, nullptr);
            auto msg = lstring::from_wstring_to_utf8(std::wstring((LPWSTR) msg_buf));
            LocalFree(msg_buf);
            return msg;
#elif defined(LAMBDA_MAC_OSX) || defined(LAMBDA_WASM)
            char buffer[512];
            auto rc = strerror_r(code ? code : errno, buffer, sizeof(buffer));
            return rc == 0 ? buffer : "Error in strerror_r!";
#else
            char buffer[512];
            char *msg = strerror_r(code ? code : errno, buffer, sizeof(buffer));
            return msg ? msg : buffer;
#endif
        }

#ifdef LAMBDA_WINDOWS

        path::path(std::string path) : _path(std::move(lstring::from_utf8_to_wstring(path)))
        {}

        path::path(std::wstring path) : _path(std::move(path))
        {}

#else

        path::path(std::string path) : _path(std::move(path))
        {}

        path::path(std::wstring path) : _path(std::move(lstring::from_wstring_to_utf8(path)))
        {}

#endif

        path::path(const path &other) = default;

        path::path(path &&other) noexcept : _path(std::move(other._path))
        {}

        // =========================================================================================================================================================================
        // Modifiers/Assignments

        path &path::assign(path::string_type source)
        {
            _path = std::move(source);
            return *this;
        }

        path &path::assign(const path &source)
        {
            return assign(source._path);
        }

        void path::clear() noexcept
        {
            this->_path.clear();
        }

        // =========================================================================================================================================================================
        // Appends

        path &path::append(const path &path)
        {
            if (path.empty()) {
                if (!_path.empty() && _path[_path.length() - 1] != preferred_separator && _path[_path.length() - 1] != FP_ST(':'))
                    _path += preferred_separator;
                return *this;
            }
            if (path.is_absolute() &&
                ((_path != this->root_name().native() || path._path != string_type() + preferred_separator) || (path.has_root_name() && path.root_name() != this->root_name()))) {
                this->assign(path);
                return *this;
            }
            if (path.has_root_directory())
                this->assign(root_name());
            else if ((!this->has_root_directory() && this->is_absolute()) || this->has_filename())
                _path += preferred_separator;

            auto iter = path.begin();
            bool first = true;
            if (path.has_root_name())
                ++iter;
            while (iter != path.end()) {
                if (!first && !(!this->empty() && _path[_path.length() - 1] == preferred_separator))
                    _path += preferred_separator;
                first = false;
                _path += (*iter++).native();
            }
            return *this;
        }

        // =========================================================================================================================================================================
        // Decomposition

        path path::root_name() const
        {
#ifdef LAMBDA_WINDOWS
            if (_path.length() >= 2 && std::toupper(static_cast<uint8_t>(_path[0])) >= 'A' && std::toupper(static_cast<uint8_t>(_path[0])) <= 'Z' && _path[1] == L':')
                return _path.substr(0, 2);
#endif
            if (_path.length() > 2 && _path[0] == '/' && _path[1] == '/' && _path[2] != '/' && std::isprint(_path[2])) {
                string_type::size_type pos = _path.find_first_of(FP_ST("/\\"), 3);
                if (pos == string_type::npos) return _path;
                else return _path.substr(0, pos);
            }
            return {};
        }

        path path::root_directory() const
        {
            auto root = root_name();
            if (_path.length() > root._path.length() && _path[root._path.length()] == preferred_separator)
                return string_type() + preferred_separator;
            return {};
        }

        path path::root_path() const
        {
            return root_name() / root_directory();
        }

        path path::relative_path() const
        {
            string_type root = this->root_path()._path;
            return _path.substr((maths::min)(root.length(), _path.length()));
        }

        // =========================================================================================================================================================================
        // Query

        bool path::empty() const
        {
            return this->_path.empty();
        }

        bool path::has_root_name() const
        {
            return !this->root_name().empty();
        }

        bool path::has_root_directory() const
        {
            return !this->root_directory().empty();
        }

        bool path::has_root_path() const
        {
            return !this->root_path().empty();
        }

        bool path::has_relative_path() const
        {
            return !this->relative_path().empty();
        }

        bool path::has_filename() const
        {
            return !this->get_filename().empty();
        }

        bool path::is_absolute() const
        {
#ifdef LAMBDA_WINDOWS
            return this->has_root_name() && this->has_root_directory();
#else
            return this->has_root_directory();
#endif
        }

        // =========================================================================================================================================================================
        // Iterators

        path::iterator path::begin() const
        {
            return iterator(_path.begin(), _path.end(), _path.begin());
        }

        path::iterator path::end() const
        {
            return iterator(_path.begin(), _path.end(), _path.end());
        }

        // =========================================================================================================================================================================
        // String manipulation

        std::string path::to_string() const
        {
#ifdef LAMBDA_WINDOWS
            return std::move(lstring::from_wstring_to_utf8(_path));
#else
            return _path;
#endif
        }

        std::wstring path::to_wstring() const
        {
#ifdef LAMBDA_WINDOWS
            return this->_path;
#else
            return std::move(Object::to_wstring());
#endif
        }

        std::string path::to_generic_string() const
        {
            auto p = std::move(this->to_string());
            return std::move((this->is_absolute() && !(lstring::starts_with(p, "/") || lstring::starts_with(p, "\\")) ? "/" : "") + lstring::replace_all(std::move(p), "\\", "/"));
        }

        const path::string_type &path::native() const noexcept
        {
            return _path;
        }

        const path::value_type *path::c_str() const noexcept
        {
            return _path.c_str();
        }

        // =========================================================================================================================================================================
        // Filesystem operations

        path path::to_absolute() const
        {
            std::error_code ec;
            auto result = std::move(this->to_absolute(ec));
            if (ec)
                throw filesystem_error(get_sys_error_txt(ec.value()), *this, ec);
            return std::move(result);
        }

        path path::to_absolute(std::error_code &ec) const
        {
            ec.clear();
#ifdef LAMBDA_WINDOWS
            if (this->empty())
                return (current_path() / "").to_absolute(ec);
            uint32_t size = ::GetFullPathNameW(this->c_str(), 0, nullptr, nullptr);
            if (size) {
                std::vector<wchar_t> buffer(size, 0);
                uint32_t a = ::GetFullPathNameW(this->c_str(), size, buffer.data(), nullptr);
                if (a && a < size) {
                    path result(std::wstring(buffer.data(), a));
                    return result;
                }
            }
            ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
            return {};
#else
            char temp[PATH_MAX];
            if (realpath(this->c_str(), temp) == nullptr) {
                ec = std::error_code(errno, std::system_category());
                return {};
            }
            return {temp};
#endif
        }

        bool path::exists() const
        {
#ifdef LAMBDA_WINDOWS
            return GetFileAttributesA(to_string().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
            struct STAT_STRUCT sb{};
            return stat(this->to_string().c_str(), &sb) == 0;
#endif
        }

        path path::get_filename() const
        {
            return !this->has_relative_path() ? path() : *--end();
        }

        path path::get_extension() const
        {
            string_type file_name = std::move(get_filename());
            auto pos = file_name.find_last_of('.');
            if (pos == std::string::npos || pos == 0)
                return {""};
            return file_name.substr(pos);
        }

        file_status path::status() const
        {
            std::error_code ec;
            auto result = this->status(ec);
            return result;
        }

        file_status path::status(std::error_code &ec) const noexcept
        {
            ec.clear();
#ifdef LAMBDA_WINDOWS
            WIN32_FILE_ATTRIBUTE_DATA attr;
            if (!::GetFileAttributesExW(this->c_str(), GetFileExInfoStandard, &attr))
                ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
            else if (attr.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                auto target = this->read_symlink(ec);
                if (!ec && !target.empty())
                    return target.status(ec);
                return {file_type::unknown};
            }
            if (ec) {
                if (is_not_found_error(ec))
                    return {file_type::not_found, perms::unknown};
                return {file_type::none};
            }
            return status_from_INFO(*this, &attr, ec);
#else
            struct STAT_STRUCT st{};
            auto result = ::lstat(this->c_str(), &st);
            if (result == 0) {
                file_status fs = file_status_from_st_mode(st.st_mode);
                if (fs.type == file_type::symlink) {
                    result = ::stat(this->c_str(), &st);
                    if (result == 0)
                        fs = file_status_from_st_mode(st.st_mode);
                }
                return fs;
            } else {
                auto error = errno;
                ec = std::error_code(errno, std::system_category());
                if (error == ENOENT)
                    return {file_type::not_found, perms::unknown};
                return {file_type::none};
            }
#endif
        }

        file_status path::symlink_status() const
        {
            std::error_code ec;
            auto result = this->symlink_status(ec);
            return result;
        }

        file_status path::symlink_status(std::error_code &ec) const noexcept
        {
#ifdef LAMBDA_WINDOWS
            file_status fs{};
            WIN32_FILE_ATTRIBUTE_DATA attr;
            if (!GetFileAttributesExW(this->c_str(), GetFileExInfoStandard, &attr))
                ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
            else {
                ec.clear();
                fs = status_from_INFO(*this, &attr, ec);
                if (attr.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
                    fs.type = file_type::symlink;
            }
            if (ec) {
                if (is_not_found_error(ec))
                    return {file_type::not_found, perms::unknown};
                return {file_type::none};
            }
            return fs;
#else
            struct STAT_STRUCT st{};
            auto result = ::stat(this->c_str(), &st);
            if (result == 0) {
                ec.clear();
                return file_status_from_st_mode(st.st_mode);
            }
            auto error = errno;
            ec = std::error_code(error, std::system_category());
            if (error == ENOENT)
                return {file_type::not_found, perms::unknown};
            return {file_type::none};
#endif
        }

        file_type path::get_file_type() const
        {
            return this->status().type;
        }

        file_type path::get_file_type(std::error_code &ec) const noexcept
        {
            return this->status(ec).type;
        }

        void path::permissions(perms prms, perm_options opts) const
        {
            std::error_code ec;
            this->permissions(prms, opts, ec);
            if (ec)
                throw filesystem_error(get_sys_error_txt(ec.value()), *this, ec);
        }

        void path::permissions(perms prms, perm_options opts, std::error_code &ec) const
        {
            if (static_cast<int>(opts & (perm_options::replace | perm_options::add | perm_options::remove)) == 0) {
                ec = std::error_code(ERROR_INVALID_PARAMETER, std::system_category());
                return;
            }
            auto fs = this->symlink_status(ec);
            if ((opts & perm_options::replace) != perm_options::replace) {
                if ((opts & perm_options::add) == perm_options::add)
                    prms = fs.prms | prms;
                else
                    prms = fs.prms & ~prms;
            }
#ifdef LAMBDA_WINDOWS
            int mode = 0;
            if ((prms & perms::owner_read) == perms::owner_read) mode |= _S_IREAD;
            if ((prms & perms::owner_write) == perms::owner_write) mode |= _S_IWRITE;
            if (::_wchmod(this->c_str(), mode) != 0)
                ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
#else
            if ((opts & perm_options::nofollow) != perm_options::nofollow)
                if (::chmod(this->c_str(), static_cast<mode_t>(prms)) != 0)
                    ec = std::error_code(errno, std::system_category());
#endif
        }

        path path::read_symlink() const
        {
            std::error_code ec;
            auto result = this->read_symlink(ec);
            if (ec)
                throw filesystem_error(get_sys_error_txt(ec.value()), *this, ec);
            return result;
        }

        path path::read_symlink(std::error_code &ec) const
        {
            file_type type = this->get_file_type(ec);
            if (type != file_type::symlink) {
                ec = std::error_code(ERROR_INVALID_PARAMETER, std::system_category());
                return {};
            }
#ifdef LAMBDA_WINDOWS
#ifndef REPARSE_DATA_BUFFER_HEADER_SIZE
            typedef struct _REPARSE_DATA_BUFFER
            {
                ULONG ReparseTag;
                USHORT ReparseDataLength;
                USHORT Reserved;
                union
                {
                    struct
                    {
                        USHORT SubstituteNameOffset;
                        USHORT SubstituteNameLength;
                        USHORT PrintNameOffset;
                        USHORT PrintNameLength;
                        ULONG Flags;
                        WCHAR PathBuffer[1];
                    } SymbolicLinkReparseBuffer;
                    struct
                    {
                        USHORT SubstituteNameOffset;
                        USHORT SubstituteNameLength;
                        USHORT PrintNameOffset;
                        USHORT PrintNameLength;
                        WCHAR PathBuffer[1];
                    } MountPointReparseBuffer;
                    struct
                    {
                        UCHAR DataBuffer[1];
                    } GenericReparseBuffer;
                } DUMMYUNIONNAME;
            } REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#endif
            std::shared_ptr<void> file(::CreateFileW(this->c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING,
                                                     FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, nullptr), ::CloseHandle);
            if (file.get() == INVALID_HANDLE_VALUE) {
                ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
                return {};
            }

            char buffer[MAXIMUM_REPARSE_DATA_BUFFER_SIZE] = {0};
            REPARSE_DATA_BUFFER &reparse_data = *(REPARSE_DATA_BUFFER *) buffer;
            DWORD buffer_used;
            path result;
            if (DeviceIoControl(file.get(), FSCTL_GET_REPARSE_POINT, nullptr, 0, &reparse_data, sizeof(buffer), &buffer_used, nullptr)) {
                if (IsReparseTagMicrosoft(reparse_data.ReparseTag))
                    switch (reparse_data.ReparseTag) {
                        case IO_REPARSE_TAG_SYMLINK:
                            result = std::wstring(&reparse_data.SymbolicLinkReparseBuffer.PathBuffer[reparse_data.SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(wchar_t)],
                                                  reparse_data.SymbolicLinkReparseBuffer.PrintNameLength / sizeof(wchar_t));
                            break;
                        case IO_REPARSE_TAG_MOUNT_POINT:
                            result = std::wstring(&reparse_data.MountPointReparseBuffer.PathBuffer[reparse_data.MountPointReparseBuffer.PrintNameOffset / sizeof(wchar_t)],
                                                  reparse_data.MountPointReparseBuffer.PrintNameLength / sizeof(wchar_t));
                            break;
                        default:
                            break;
                    }
            } else ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
            return result;
#else
            size_t buffer_size = 256;
            while (true) {
                std::vector<char> buffer(buffer_size, 0);
                auto rc = ::readlink(this->c_str(), buffer.data(), buffer.size());
                if (rc < 0) {
                    ec = std::error_code(errno, std::system_category());
                    return {};
                } else if (rc < static_cast<int>(buffer_size))
                    return {std::string(buffer.data(), rc)};
                buffer_size *= 2;
            }
#endif
        }

        bool path::mkdir(perms prms) const
        {
            std::error_code ec;
            auto result = mkdir(prms, ec);
            return result;
        }

        bool path::mkdir(std::error_code &ec) const noexcept
        {
            return mkdir(perms::all, ec);
        }

        bool path::mkdir(perms prms, std::error_code &ec) const noexcept
        {
            std::error_code ec1;
            ec.clear();
            auto fs = this->status(ec1);
            if (fs.type != file_type::none && this->exists())
                return false;
#ifdef LAMBDA_WINDOWS
            if (!::CreateDirectoryW(this->c_str(), nullptr)) {
                ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
                return false;
            }
            this->permissions(prms, ec);
            if (ec)
                return false;
#else
            mode_t attribs = static_cast<mode_t>(prms);
            if (::mkdir(this->c_str(), attribs) != 0) {
                ec = std::error_code(errno, std::system_category());
                return false;
            }
#endif
            return true;
        }

        bool path::mkdirs() const
        {
            std::error_code ec;
            return mkdirs(ec);
        }

        bool path::mkdirs(std::error_code &ec) const noexcept
        {
            path current;
            ec.clear();
            auto root_name_v = this->root_name(), root_path_v = this->root_path();
            for (const auto &part : *this) {
                current /= part;
                if (current != root_name_v && current != root_path_v) {
                    std::error_code ec1;
                    auto fs = current.status(ec1);
                    if (ec1 && fs.type != file_type::not_found) {
                        ec = ec1;
                        return false;
                    }
                    if (!current.exists()) {
                        current.mkdir(ec);
                        if (ec) return false;
                    } else if (fs.type != file_type::directory)
                        return false;
                }
            }
            return true;
        }

        void path::move(const path &new_path) const
        {
            std::error_code ec;
            this->move(new_path, ec);
            if (ec) throw filesystem_error(get_sys_error_txt(ec.value()), *this, new_path, ec);
        }

        void path::move(const path &new_path, std::error_code &ec) const noexcept
        {
            ec.clear();
            if (*this != new_path) {
#ifdef LAMBDA_WINDOWS
                if (!MoveFileW(this->c_str(), new_path.c_str()))
                    ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
#else
                if (::rename(this->c_str(), new_path.c_str()) != 0)
                    ec = std::error_code(errno, std::system_category());
#endif
            }
        }

        bool path::remove() const
        {
            std::error_code ec;
            auto result = this->remove(ec);
            if (ec) throw filesystem_error(get_sys_error_txt(ec.value()), *this, ec);
            return result;
        }

        bool path::remove(std::error_code &ec) const noexcept
        {
            ec.clear();
#ifdef LAMBDA_WINDOWS
            DWORD attr = ::GetFileAttributesW(this->c_str());
            if (attr == INVALID_FILE_ATTRIBUTES) {
                int error = static_cast<int>(::GetLastError());
                if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
                    return false;
                ec = std::error_code(error, std::system_category());
            }
            if (!ec) {
                if (attr & FILE_ATTRIBUTE_DIRECTORY) {
                    if (!::RemoveDirectoryW(this->c_str()))
                        ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
                } else {
                    if (!::DeleteFileW(this->c_str()))
                        ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
                }
            }
#else
            if (::remove(this->c_str()) == -1) {
                auto error = errno;
                if (error == ENOENT) return false;
                ec = std::error_code(error, std::system_category());
            }
#endif
            return ec ? false : true;
        }

        uintmax_t path::remove_all() const
        {
            std::error_code ec;
            auto result = this->remove_all(ec);
            if (ec) throw filesystem_error(get_sys_error_txt(ec.value()), *this, ec);
            return result;
        }

        uintmax_t path::remove_all(std::error_code &ec) const noexcept
        {
            ec.clear();
            uintmax_t count = 0;
            return 0;
        }

        void path::resize_file(uintmax_t size) const
        {
            std::error_code ec;
            this->resize_file(size, ec);
            if (ec) throw filesystem_error(get_sys_error_txt(ec.value()), ec);
        }

        void path::resize_file(uintmax_t size, std::error_code &ec) const noexcept
        {
            ec.clear();
#ifdef LAMBDA_WINDOWS
            LARGE_INTEGER li_size;
            li_size.QuadPart = size;
            std::shared_ptr<void> file(::CreateFileW(this->c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr), ::CloseHandle);
            if (file.get() == INVALID_HANDLE_VALUE)
                ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
            else if (::SetFilePointerEx(file.get(), li_size, nullptr, FILE_BEGIN) == 0 || ::SetEndOfFile(file.get()) == 0)
                ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
#else
            if (::truncate(this->c_str(), size) != 0)
                ec = std::error_code(errno, std::system_category());
#endif
        }

        bool path::is_directory() const
        {
            return this->get_file_type() == file_type::directory;
        }

        bool path::is_file() const
        {
            return this->get_file_type() == file_type::regular;
        }

        bool path::is_symlink() const
        {
            return this->get_file_type() == file_type::symlink;
        }

        size_t path::get_size() const
        {
            if (!this->exists())
                return 0;
            struct STAT_STRUCT sb{};
            if (STAT_METHOD(this->to_string().c_str(), &sb) != 0)
                throw std::runtime_error("lambdacommon::fs::path.get_size(): cannot stat file \"" + to_string() + "\"!");
            return (size_t) sb.st_size;
        }

        bool path::operator==(const path &other) const
        {
            return _path == other._path;
        }

        bool path::operator<(const path &other) const
        {
            return _path < other._path;
        }

        path &path::operator=(const path &other)
        {
            if (this != &other) {
                if (this->_path != other._path)
                    this->_path = other._path;
            }
            return *this;
        }

        path &path::operator=(path &&other) noexcept
        {
            this->_path = std::move(other._path);
            return *this;
        }

        path &path::operator=(path::string_type source)
        {
            this->assign(std::move(source));
            return *this;
        }

        path::operator string_type() const
        {
            return this->native();
        }

        path &path::operator/=(const path &other)
        {
            this->append(other);
            return *this;
        }

        path &path::operator/=(const std::string &other)
        {
            this->append(other);
            return *this;
        }

        path::iterator::iterator() = default;

        path::iterator::iterator(const string_type::const_iterator &first, const string_type::const_iterator &last, const string_type::const_iterator &pos) : _first(first),
                                                                                                                                                              _last(last),
                                                                                                                                                              _pos(pos)
        {
            update_current();
            // Find the position of a potential root directory.
#ifdef LAMBDA_WINDOWS
            if (_last - _first >= 3 && std::toupper(static_cast<uint8_t>(*first)) >= 'A' && std::toupper(static_cast<uint8_t>(*first)) <= 'Z' && *(first + 1) == L':' &&
                *(first + 2) == preferred_separator)
                _root = _first + 2;
            else
#endif
            if (_first != _last && *_first == preferred_separator) {
                if (_last - _first >= 2 && *(_first + 1) == preferred_separator && !(_last - _first >= 3 && *(_first + 2) == preferred_separator))
                    _root = increment(_first);
                else
                    _root = _first;
            } else
                _root = _last;
        }

        path::iterator &path::iterator::operator++()
        {
            _pos = increment(_pos);
            while (_pos != _last && _pos != _root && *_pos == path::preferred_separator && (_pos + 1) != _last)
                ++_pos;
            update_current();
            return *this;
        }

        path::iterator path::iterator::operator++(int)
        {
            path::iterator i{*this};
            ++(*this);
            return i;
        }

        path::iterator &path::iterator::operator--()
        {
            _pos = decrement(_pos);
            update_current();
            return *this;
        }

        path::iterator path::iterator::operator--(int)
        {
            path::iterator i{*this};
            --(*this);
            return i;
        }

        bool path::iterator::operator==(const path::iterator &other) const
        {
            return _pos == other._pos;
        }

        const path &path::iterator::operator*() const
        {
            return _current;
        }

        path::iterator::pointer path::iterator::operator->() const
        {
            return &_current;
        }

        void path::iterator::update_current()
        {
            if (_pos != _first && _pos != _last && (*_pos == preferred_separator && _pos != _root) && (_pos + 1 == _last))
                _current = FP_ST("");
            else {
                _current.assign(_pos, increment(_pos));
                if (_current.native().size() > 1 && _current.native()[0] == preferred_separator && _current.native()[_current.native().size() - 1] == preferred_separator)
                    _current = path::string_type() + preferred_separator;
            }
        }

        path::string_type::const_iterator path::iterator::increment(const path::string_type::const_iterator &pos) const
        {
            path::string_type::const_iterator i = _pos;
            bool from_start = i == _first;
            if (i != _last) {
                // We can only sit on a separator if it's a network name or root.
                if (*i++ == preferred_separator) {
                    if (i != _last && *i == preferred_separator) {
                        if (from_start && !(i + 1 != _last && *(i + 1) == preferred_separator))
                            i = std::find(++i, _last, preferred_separator);
                            // Skip redundant separators. (Why would they be here?)
                        else while (i != _last && *i == preferred_separator) ++i;
                    }
                } else {
                    if (from_start && i != _last && *i == FP_ST(':')) ++i;
                    else i = std::find(i, _last, preferred_separator);
                }
            }
            return i;
        }

        path::string_type::const_iterator path::iterator::decrement(const path::string_type::const_iterator &pos) const
        {
            path::string_type::const_iterator i = pos;
            if (i != _first) {
                --i;

                if (i != _root && (pos != _last || *i != path::preferred_separator)) {
#ifdef LAMBDA_WINDOWS
                    static const std::wstring seps = L"\\/:";
                    i = std::find_first_of(std::reverse_iterator<path::string_type::const_iterator>(i), std::reverse_iterator<path::string_type::const_iterator>(_first),
                                           seps.begin(), seps.end()).base();
                    if (i > _first && *i == L':')
                        i++;
#else
                    i = std::find(std::reverse_iterator<path::string_type::const_iterator>(i), std::reverse_iterator<path::string_type::const_iterator>(_first),
                                  '/').base();
#endif
                    if (i - _first == 2 && *_first == path::preferred_separator && *(_first + 1) == path::preferred_separator)
                        i -= 2;
                }
            }
            return i;
        }

        filesystem_error::filesystem_error(const std::string &msg, std::error_code ec) : system_error(ec, msg)
        {}

        filesystem_error::filesystem_error(const std::string &msg, const path &p1, std::error_code ec) : system_error(ec, msg), _p1(p1)
        {}

        filesystem_error::filesystem_error(const std::string &msg, const path &p1, const path &p2, std::error_code ec) : system_error(ec, msg), _p1(p1), _p2(p2)
        {}

        const path &filesystem_error::path1() const noexcept
        {
            return _p1;
        }

        const path &filesystem_error::path2() const noexcept
        {
            return _p2;
        }

        // =========================================================================================================================================================================
        // Directory entry

        directory_entry::directory_entry(path p) : _path(std::move(p))
        {}

        void directory_entry::assign(path p)
        {
            _path = std::move(p);
        }

        const path &directory_entry::get_path() const noexcept
        {
            return _path;
        }

        directory_entry::operator const path &() const noexcept
        {
            return this->get_path();
        }

        file_status directory_entry::status() const
        {
            return this->get_path().status();
        }

        file_status directory_entry::status(std::error_code &ec) const noexcept
        {
            return this->get_path().status(ec);
        }

        file_status directory_entry::symlink_status() const
        {
            return this->get_path().symlink_status();
        }

        file_status directory_entry::symlink_status(std::error_code &ec) const noexcept
        {
            return this->get_path().symlink_status(ec);
        }

        bool directory_entry::operator==(const directory_entry &other) const
        {
            return _path == other._path;
        }

        bool directory_entry::operator<(const directory_entry &other) const
        {
            return _path < other._path;
        }

        const path *directory_entry::operator->() const
        {
            return &_path;
        }

        // =========================================================================================================================================================================
        // Directory iterator

        class directory_iterator::impl
        {
        private:
            path _base;
#ifdef LAMBDA_WINDOWS
            WIN32_FIND_DATAW _find_data;
            HANDLE _dir_handle;
#else
            std::shared_ptr<DIR> _dir;
            struct ::dirent *_entry;
            size_t _buffer_size;
            std::unique_ptr<char[]> _buffer;

            inline size_t directory_entry_buffer_size(DIR *d)
            {
                size_t result = maths::max(sizeof(::dirent), sizeof(::dirent) - sizeof(::dirent::d_name) + NAME_MAX) + 1;
                if (d) {
                    long rc = ::fpathconf(dirfd(d), _PC_NAME_MAX);
                    if (rc > long(result))
                        result = static_cast<size_t>(rc);
                }
                return result;
            }

#endif

        public:
            path _current;
            directory_entry _dir_entry;
            std::error_code ec;

#ifdef LAMBDA_WINDOWS

            explicit impl(path p) : _base(std::move(p)),
                                    _dir_handle(_base.empty() ? INVALID_HANDLE_VALUE : FindFirstFileW((_base / "*").c_str(), &_find_data))
            {
                _current = _dir_handle != INVALID_HANDLE_VALUE ? _base / std::wstring(_find_data.cFileName) : path();
                if (_dir_handle == INVALID_HANDLE_VALUE && !p.empty()) {
                    auto error = ::GetLastError();
                    _base = path();
                    if (error != ERROR_ACCESS_DENIED)
                        ec = std::error_code(static_cast<int>(error), std::system_category());
                } else {
                    if (std::wstring(_find_data.cFileName) == L"." || std::wstring(_find_data.cFileName) == L"..") this->increment(ec);
                    else _dir_entry._path = _current;
                }
            }

#else

            explicit impl(path p) : _base(std::move(p)),
                                    _dir((p.empty() ? nullptr : ::opendir(p.c_str())), [](DIR *d) { if (d) ::closedir(d); }),
                                    _buffer_size(directory_entry_buffer_size(_dir.get())),
                                    _buffer(new char[_buffer_size])
            {
                _entry = reinterpret_cast<::dirent *>(&_buffer[0]);
                if (!p.empty()) {
                    if (!_dir) {
                        auto error = errno;
                        _base = path();
                        if (error != EACCES)
                            ec = std::error_code(error, std::system_category());
                    } else
                        this->increment(ec);
                }
            }

#endif

            impl(const impl &other) = delete;

            ~impl()
            {
#ifdef LAMBDA_WINDOWS
                if (_dir_handle != INVALID_HANDLE_VALUE) {
                    FindClose(_dir_handle);
                    _dir_handle = INVALID_HANDLE_VALUE;
                }
#endif
            }

#ifndef LAMBDA_WINDOWS

            int i_readdir_r(DIR *dir, struct dirent *entry, struct dirent **result)
            {
#if defined(__GLIBC__) && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 24))
                errno = 0;
                auto de = ::readdir(dir);
                if (de) {
                    *entry = *de;
                    *result = entry;
                    return 0;
                }
                return errno;
#else
                return ::readdir_r(dir, entry, result);
#endif
            }

#endif

            void increment(std::error_code &ec)
            {
#ifdef LAMBDA_WINDOWS
                if (_dir_handle != INVALID_HANDLE_VALUE) {
                    do {
                        if (FindNextFileW(_dir_handle, &_find_data))
                            _dir_entry._path = _current = _base / std::wstring(_find_data.cFileName);
                        else {
                            FindClose(_dir_handle);
                            _dir_handle = INVALID_HANDLE_VALUE;
                            _current = path();
                            break;
                        }
                    } while (std::wstring(_find_data.cFileName) == L"." || std::wstring(_find_data.cFileName) == L"..");
                } else {
                    ec = this->ec;
                }
#else
                if (_dir) {
                    do {
                        dirent *result = nullptr;
                        if (this->i_readdir_r(_dir.get(), _entry, &result) == 0) {
                            if (result) {
                                _current = _base / path(_entry->d_name);
                                _dir_entry = directory_entry(_current);
                            } else {
                                _dir.reset();
                                _current = path();
                                break;
                            }
                        } else {
                            _current = path();
                            ec = std::error_code(errno, std::system_category());
                            break;
                        }
                    } while (lstring::equals(_entry->d_name, ".") || lstring::equals(_entry->d_name, ".."));
                }
#endif
            }
        };

        directory_iterator::directory_iterator() noexcept : _impl(new impl(std::move(path())))
        {}

        directory_iterator::directory_iterator(path p) : _impl(new impl(std::move(p)))
        {
            if (this->_impl->ec) throw filesystem_error(get_sys_error_txt(this->_impl->ec.value()), p, this->_impl->ec);
            this->_impl->ec.clear();
        }

        directory_iterator::directory_iterator(path p, std::error_code &ec) noexcept : _impl(new impl(p))
        {
            if (this->_impl->ec)
                ec = this->_impl->ec;
        }

        directory_iterator &directory_iterator::operator++()
        {
            std::error_code ec;
            this->increment(ec);
            if (ec) throw filesystem_error(get_sys_error_txt(ec.value()), this->_impl->_current, ec);
            return *this;
        }

        directory_iterator &directory_iterator::increment(std::error_code &ec) noexcept
        {
            this->_impl->increment(ec);
            return *this;
        }

        bool directory_iterator::operator==(const directory_iterator &other) const
        {
            return this->_impl->_current == other._impl->_current;
        }

        bool directory_iterator::operator!=(const directory_iterator &other) const
        {
            return !(*this == other);
        }

        const directory_entry &directory_iterator::operator*() const
        {
            return this->_impl->_dir_entry;
        }

        directory_iterator::pointer directory_iterator::operator->() const
        {
            return (&**this);
        }

        void directory_iterator::swap(directory_iterator &other)
        {
            std::swap(this->_impl, other._impl);
        }

        directory_iterator &directory_iterator::operator=(const directory_iterator &other)
        {
            _impl = other._impl;
            return *this;
        }

        directory_iterator &directory_iterator::operator=(directory_iterator &&other)
        {
            _impl = std::move(other._impl);
            return *this;
        }

        // =========================================================================================================================================================================
        // Filesystem operations

        std::wstring LAMBDACOMMON_API current_path_wstr()
        {
#ifdef LAMBDA_WINDOWS
            wchar_t temp[MAX_PATH];
            if (!_wgetcwd(temp, MAX_PATH))
                throw std::runtime_error("fs.cpp(" + std::to_string(__LINE__ - 1) + ")@lambdacommon::fs::current_path_wstr(): Internal error \"" + std::to_string(GetLastError())
                                         + "\"");
            return {temp};
#else
            return lstring::from_utf8_to_wstring(current_path_str());
#endif
        }

        std::string LAMBDACOMMON_API current_path_str()
        {
#ifdef LAMBDA_WINDOWS
            return lstring::from_wstring_to_utf8(current_path_wstr());
#else
            char temp[PATH_MAX];
            if (getcwd(temp, PATH_MAX) == nullptr)
                throw std::runtime_error("fs.cpp(" + std::to_string(__LINE__ - 1) + ")@lambdacommon::fs::current_path_str(): Internal error \"" + strerror(errno) + "\"");
            return {temp};
#endif
        }

        path LAMBDACOMMON_API current_path()
        {
            return path(std::move(current_path_str()));
        }
    }
}
