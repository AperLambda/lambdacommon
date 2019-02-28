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

        inline file_status status_from_INFO(const FilePath &path, const WIN32_FILE_ATTRIBUTE_DATA *info, std::error_code &ec)
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

        FilePath::FilePath() = default;

#ifdef LAMBDA_WINDOWS

        FilePath::FilePath(std::string path) : _path(std::move(lstring::from_utf8_to_wstring(path)))
        {}

        FilePath::FilePath(std::wstring path) : _path(std::move(path))
        {}

#else

        FilePath::FilePath(std::string path) : _path(std::move(path))
        {}

        FilePath::FilePath(std::wstring path) : _path(std::move(lstring::from_wstring_to_utf8(path)))
        {}

#endif

        FilePath::FilePath(const FilePath &other) = default;

        FilePath::FilePath(FilePath &&other) noexcept : _path(std::move(other._path))
        {}

        // =========================================================================================================================================================================
        // Modifiers/Assignments

        FilePath &FilePath::assign(FilePath::string_type source)
        {
            _path = std::move(source);
            return *this;
        }

        FilePath &FilePath::assign(const FilePath &source)
        {
            return assign(source._path);
        }

        void FilePath::clear() noexcept
        {
            this->_path.clear();
        }

        // =========================================================================================================================================================================
        // Appends

        FilePath &FilePath::append(const FilePath &path)
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

        FilePath FilePath::root_name() const
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

        FilePath FilePath::root_directory() const
        {
            auto root = root_name();
            if (_path.length() > root._path.length() && _path[root._path.length()] == preferred_separator)
                return string_type() + preferred_separator;
            return {};
        }

        FilePath FilePath::root_path() const
        {
            return root_name() / root_directory();
        }

        FilePath FilePath::relative_path() const
        {
            string_type root = this->root_path()._path;
            return _path.substr((maths::min)(root.length(), _path.length()));
        }

        // =========================================================================================================================================================================
        // Query

        bool FilePath::empty() const
        {
            return this->_path.empty();
        }

        bool FilePath::has_root_name() const
        {
            return !this->root_name().empty();
        }

        bool FilePath::has_root_directory() const
        {
            return !this->root_directory().empty();
        }

        bool FilePath::has_root_path() const
        {
            return !this->root_path().empty();
        }

        bool FilePath::has_relative_path() const
        {
            return !this->relative_path().empty();
        }

        bool FilePath::has_filename() const
        {
            return !this->get_filename().empty();
        }

        bool FilePath::is_absolute() const
        {
#ifdef LAMBDA_WINDOWS
            return this->has_root_name() && this->has_root_directory();
#else
            return this->has_root_directory();
#endif
        }

        // =========================================================================================================================================================================
        // Iterators

        FilePath::iterator FilePath::begin() const
        {
            return iterator(_path.begin(), _path.end(), _path.begin());
        }

        FilePath::iterator FilePath::end() const
        {
            return iterator(_path.begin(), _path.end(), _path.end());
        }

        // =========================================================================================================================================================================
        // String manipulation

        std::string FilePath::to_string() const
        {
#ifdef LAMBDA_WINDOWS
            return std::move(lstring::from_wstring_to_utf8(_path));
#else
            return _path;
#endif
        }

        std::wstring FilePath::to_wstring() const
        {
#ifdef LAMBDA_WINDOWS
            return this->_path;
#else
            return std::move(Object::to_wstring());
#endif
        }

        std::string FilePath::to_generic_string() const
        {
            auto p = std::move(this->to_string());
            return std::move((this->is_absolute() && !(lstring::starts_with(p, "/") || lstring::starts_with(p, "\\")) ? "/" : "") + lstring::replace_all(std::move(p), "\\", "/"));
        }

        const FilePath::string_type &FilePath::native() const noexcept
        {
            return _path;
        }

        const FilePath::value_type *FilePath::c_str() const noexcept
        {
            return _path.c_str();
        }

        // =========================================================================================================================================================================
        // Filesystem operations

        FilePath FilePath::to_absolute() const
        {
            std::error_code ec;
            auto result = std::move(this->to_absolute(ec));
            if (ec)
                throw filesystem_error(get_sys_error_txt(ec.value()), *this, ec);
            return std::move(result);
        }

        FilePath FilePath::to_absolute(std::error_code &ec) const
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
                    FilePath result(std::wstring(buffer.data(), a));
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

        bool FilePath::exists() const
        {
#ifdef LAMBDA_WINDOWS
            return GetFileAttributesA(to_string().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
            struct STAT_STRUCT sb{};
            return stat(this->to_string().c_str(), &sb) == 0;
#endif
        }

        FilePath FilePath::get_filename() const
        {
            return !this->has_relative_path() ? FilePath() : *--end();
        }

        FilePath FilePath::get_extension() const
        {
            string_type file_name = std::move(get_filename());
            auto pos = file_name.find_last_of('.');
            if (pos == std::string::npos || pos == 0)
                return {""};
            return file_name.substr(pos);
        }

        file_status FilePath::status() const
        {
            std::error_code ec;
            auto result = this->status(ec);
            return result;
        }

        file_status FilePath::status(std::error_code &ec) const noexcept
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

        file_status FilePath::symlink_status() const
        {
            std::error_code ec;
            auto result = this->symlink_status(ec);
            return result;
        }

        file_status FilePath::symlink_status(std::error_code &ec) const noexcept
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

        file_type FilePath::get_file_type() const
        {
            return this->status().type;
        }

        file_type FilePath::get_file_type(std::error_code &ec) const noexcept
        {
            return this->status(ec).type;
        }

        void FilePath::permissions(perms perms, perm_options opts)
        {
            std::error_code ec;
            this->permissions(perms, opts, ec);
            if (ec)
                throw filesystem_error(get_sys_error_txt(ec.value()), *this, ec);
        }

        void FilePath::permissions(perms perms, perm_options opts, std::error_code &ec)
        {
            if (static_cast<int>(opts & (perm_options::replace | perm_options::add | perm_options::remove)) == 0) {
                ec = std::error_code(ERROR_INVALID_PARAMETER, std::system_category());
                return;
            }
            auto fs = this->symlink_status(ec);
            if ((opts & perm_options::replace) != perm_options::replace) {
                if ((opts & perm_options::add) == perm_options::add)
                    perms = fs.perms | perms;
                else
                    perms = fs.perms & ~perms;
            }
#ifdef LAMBDA_WINDOWS
            int mode = 0;
            if ((perms & perms::owner_read) == perms::owner_read) mode |= _S_IREAD;
            if ((perms & perms::owner_write) == perms::owner_write) mode |= _S_IWRITE;
            if (::_wchmod(this->c_str(), mode) != 0)
                ec = std::error_code(static_cast<int>(::GetLastError()), std::system_category());
#else
            if ((opts & perm_options::nofollow) != perm_options::nofollow)
                if (::chmod(this->c_str(), static_cast<mode_t>(perms)) != 0)
                    ec = std::error_code(errno, std::system_category());
#endif
        }

        FilePath FilePath::read_symlink() const
        {
            std::error_code ec;
            auto result = this->read_symlink(ec);
            if (ec)
                throw filesystem_error(get_sys_error_txt(ec.value()), *this, ec);
            return result;
        }

        FilePath FilePath::read_symlink(std::error_code &ec) const
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
            FilePath result;
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
            return {};
#endif
        }

        bool FilePath::is_directory() const
        {
            return this->get_file_type() == file_type::directory;
        }

        bool FilePath::is_file() const
        {
            return this->get_file_type() == file_type::regular;
        }

        bool FilePath::is_symlink() const
        {
            return this->get_file_type() == file_type::symlink;
        }

        size_t FilePath::get_size() const
        {
            if (!this->exists())
                return 0;
            struct STAT_STRUCT sb{};
            if (STAT_METHOD(this->to_string().c_str(), &sb) != 0)
                throw std::runtime_error("lambdacommon::fs::FilePath.get_size(): cannot stat file \"" + to_string() + "\"!");
            return (size_t) sb.st_size;
        }

        bool FilePath::remove(bool recursive)
        {
            return false;
            /*if (recursive)
                return std::filesystem::remove_all({to_string()}) != 0;
            else
                return std::filesystem::remove({to_string()});*/
        }

        bool FilePath::operator==(const FilePath &other) const
        {
            return _path == other._path;
        }

        bool FilePath::operator<(const FilePath &other) const
        {
            return _path < other._path;
        }

        FilePath &FilePath::operator=(const FilePath &other)
        {
            if (this != &other) {
                if (this->_path != other._path)
                    this->_path = other._path;
            }
            return *this;
        }

        FilePath &FilePath::operator=(FilePath &&other) noexcept
        {
            this->_path = std::move(other._path);
            return *this;
        }

        FilePath &FilePath::operator=(FilePath::string_type source)
        {
            this->assign(std::move(source));
            return *this;
        }

        FilePath::operator string_type() const
        {
            return this->native();
        }

        FilePath &FilePath::operator/=(const FilePath &other)
        {
            this->append(other);
            return *this;
        }

        FilePath &FilePath::operator/=(const std::string &other)
        {
            this->append(other);
            return *this;
        }

        FilePath::iterator::iterator() = default;

        FilePath::iterator::iterator(const string_type::const_iterator &first, const string_type::const_iterator &last, const string_type::const_iterator &pos) : _first(first),
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

        FilePath::iterator &FilePath::iterator::operator++()
        {
            _pos = increment(_pos);
            while (_pos != _last && _pos != _root && *_pos == FilePath::preferred_separator && (_pos + 1) != _last)
                ++_pos;
            update_current();
            return *this;
        }

        FilePath::iterator FilePath::iterator::operator++(int)
        {
            FilePath::iterator i{*this};
            ++(*this);
            return i;
        }

        FilePath::iterator &FilePath::iterator::operator--()
        {
            _pos = decrement(_pos);
            update_current();
            return *this;
        }

        FilePath::iterator FilePath::iterator::operator--(int)
        {
            FilePath::iterator i{*this};
            --(*this);
            return i;
        }

        bool FilePath::iterator::operator==(const FilePath::iterator &other) const
        {
            return _pos == other._pos;
        }

        const FilePath &FilePath::iterator::operator*() const
        {
            return _current;
        }

        FilePath::iterator::pointer FilePath::iterator::operator->() const
        {
            return &_current;
        }

        void FilePath::iterator::update_current()
        {
            if (_pos != _first && _pos != _last && (*_pos == preferred_separator && _pos != _root) && (_pos + 1 == _last))
                _current = FP_ST("");
            else {
                _current.assign(_pos, increment(_pos));
                if (_current.native().size() > 1 && _current.native()[0] == preferred_separator && _current.native()[_current.native().size() - 1] == preferred_separator)
                    _current = FilePath::string_type() + preferred_separator;
            }
        }

        FilePath::string_type::const_iterator FilePath::iterator::increment(const FilePath::string_type::const_iterator &pos) const
        {
            FilePath::string_type::const_iterator i = _pos;
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

        FilePath::string_type::const_iterator FilePath::iterator::decrement(const FilePath::string_type::const_iterator &pos) const
        {
            FilePath::string_type::const_iterator i = pos;
            if (i != _first) {
                --i;

                if (i != _root && (pos != _last || *i != FilePath::preferred_separator)) {
#ifdef LAMBDA_WINDOWS
                    static const std::wstring seps = L"\\/:";
                    i = std::find_first_of(std::reverse_iterator<FilePath::string_type::const_iterator>(i), std::reverse_iterator<FilePath::string_type::const_iterator>(_first),
                                           seps.begin(), seps.end()).base();
                    if (i > _first && *i == L':')
                        i++;
#else
                    i = std::find(std::reverse_iterator<FilePath::string_type::const_iterator>(i), std::reverse_iterator<FilePath::string_type::const_iterator>(_first),
                                  '/').base();
#endif
                    if (i - _first == 2 && *_first == FilePath::preferred_separator && *(_first + 1) == FilePath::preferred_separator)
                        i -= 2;
                }
            }
            return i;
        }

        filesystem_error::filesystem_error(const std::string &msg, std::error_code ec) : system_error(ec, msg)
        {
        }

        filesystem_error::filesystem_error(const std::string &msg, const FilePath &p1, std::error_code ec) : system_error(ec, msg), _p1(p1)
        {}

        filesystem_error::filesystem_error(const std::string &msg, const FilePath &p1, const FilePath &p2, std::error_code ec) : system_error(ec, msg), _p1(p1), _p2(p2)
        {}

        const FilePath &filesystem_error::path1() const noexcept
        {
            return _p1;
        }

        const FilePath &filesystem_error::path2() const noexcept
        {
            return _p2;
        }

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

        FilePath LAMBDACOMMON_API current_path()
        {
            return FilePath(std::move(current_path_str()));
        }
    }
}
