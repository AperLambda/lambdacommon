/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/system/uri.h"
#include "../../include/lambdacommon/system/system.h"
#include "../../include/lambdacommon/exceptions/exceptions.h"
#include <sstream>
#include <regex>

namespace lambdacommon
{
	namespace uri
	{
		SchemeType LAMBDACOMMON_API get_scheme_type_by_string(const std::string &scheme)
		{
			if (lstring::equals_ignore_case(scheme, "file"))
				return FILE;
			else if (lstring::equals_ignore_case(scheme, "ftp"))
				return FTP;
			else if (lstring::equals_ignore_case(scheme, "gopher"))
				return GOPHER;
			else if (lstring::equals_ignore_case(scheme, "http"))
				return HTTP;
			else if (lstring::equals_ignore_case(scheme, "https"))
				return HTTPS;
			else if (lstring::equals_ignore_case(scheme, "ws"))
				return WS;
			else if (lstring::equals_ignore_case(scheme, "wss"))
				return WSS;
			else
				return OTHER;
		}

		port_t LAMBDACOMMON_API get_scheme_default_port(SchemeType scheme)
		{
			switch (scheme) {
				case FILE:
					return 0;
				case FTP:
					return 21;
				case GOPHER:
					return 70;
				case HTTP:
					return 80;
				case HTTPS:
					return 443;
				case WS:
					return 80;
				case WSS:
					return 443;
				case OTHER:
					return 0;
				default:
					return 0;
			}
		}

		bool LAMBDACOMMON_API is_scheme_type_non_file_special(SchemeType scheme)
		{
			return scheme != FILE && scheme != OTHER;
		}

		/*
		 * URI
		 */

		std::vector<std::pair<std::string, std::string>>
		fix_queries(const std::vector<std::pair<std::string, std::string>> &queries)
		{
			std::vector<std::pair<std::string, std::string>> new_queries{queries.size()};
			for (size_t i = 0; i < queries.size(); i++)
				new_queries[i] = {lstring::replace_all(queries[i].first, " ", "%20"),
								  lstring::replace_all(queries[i].second, " ", "%20")};
			return new_queries;
		};

		URI::URI(const std::string &scheme, const std::string &username, const std::string &password, const Address &address, const std::vector<std::string> &path,
				 const std::vector<std::pair<std::string, std::string>> &queries, const std::string &fragment)
				: Path(path),
				  _scheme(new std::string(scheme)),
				  _username(new std::string(username)),
				  _password(new std::string(password)),
				  _address(address),
				  _queries(new std::vector<std::pair<std::string, std::string>>(fix_queries(queries))),
				  _fragment(new std::string(fragment))
		{}

		URI::URI(const URI &url) : Path(url._path), _scheme(new std::string(*url._scheme)), _username(new std::string(*url._username)), _password(new std::string(*url._password)),
								   _address(url._address), _queries(new std::vector<std::pair<std::string, std::string>>(*url._queries)), _fragment(new std::string(*url._fragment))
		{}

		URI::URI(URI &&url) noexcept : Path(std::move(url)), _scheme(new std::string(move(*url._scheme))), _username(new std::string(move(*url._username))),
									   _password(new std::string(move(*url._password))), _address(std::move(url._address)),
									   _queries(new std::vector<std::pair<std::string, std::string>>(move(*url._queries))),
									   _fragment(new std::string(move(*url._fragment)))
		{}

		URI::~URI()
		{
			delete _scheme;
			delete _username;
			delete _password;
			delete _fragment;
		}

		std::string URI::get_scheme() const
		{
			return *_scheme;
		}

		std::string URI::get_username() const
		{
			return *_username;
		}

		std::string URI::get_password() const
		{
			return *_password;
		}

		void URI::set_username_and_password(const std::string &username, const std::string &password)
		{
			if (username.empty() || password.empty()) {
				*_username = "";
				*_password = "";
			} else {
				*_username = username;
				*_password = password;
			}
		}

		Address URI::get_address() const
		{
			return _address;
		}

		void URI::set_address(const Address &address)
		{
			if (address.get_host().empty() && is_scheme_type_non_file_special(get_scheme_type()))
				throw IllegalArgumentException("Hostname cannot be null for this scheme.");

			_address = address;
		}

		std::vector<std::pair<std::string, std::string>> URI::get_queries() const
		{
			return *_queries;
		}

		void URI::set_queries(const std::vector<std::pair<std::string, std::string>> &queries)
		{
			*_queries = fix_queries(queries);
		}

		bool URI::has_query(std::string query) const
		{
			bool has_query = false;
			for (const auto &q : *_queries)
				if (lstring::equals(q.first, query)) {
					has_query = true;
					break;
				}
			return has_query;
		}

		std::string URI::get_query(std::string query) const
		{
			std::string result;
			for (auto q : *_queries)
				if (lstring::equals(q.first, query)) {
					result = q.second;
					break;
				}
			return result;
		}

		std::string URI::get_fragment() const
		{
			return *_fragment;
		}

		void URI::set_fragment(const std::string &fragment)
		{
			*_fragment = fragment;
		}

		std::string URI::to_string(char delimiter) const
		{
			std::ostringstream oss;

			// Write the scheme.
			if (!_scheme->empty())
				oss << get_scheme() << ':';

			if (!_address.is_empty()) {
				oss << "//";
				// Write the username and the password.
				if (!_username->empty() && !_password->empty())
					oss << get_username() << ':' << get_password() << '@';

				// Write the address.
				oss << _address.to_string();
			} else if (*_scheme == "file")
				oss << "//";

			// Write the path.
			if (!_path.empty())
				oss << '/' << Path::to_string(delimiter);

			// Write the queries.
			if (!_queries->empty()) {
				oss << '?';
				for (size_t i = 0; i < _queries->size(); i++) {
					std::pair<std::string, std::string> query = (*_queries)[i];
					oss << query.first;
					if (!query.second.empty())
						oss << '=' << query.second;

					if (i + 1 != _queries->size())
						oss << '&';
				}
			}

			// Write the fragment.
			if (!_fragment->empty())
				oss << '#' << get_fragment();

			return oss.str();
		}

		void URI::open_in_system() const
		{
			system::open_uri(to_string('/'));
		}

		URI &URI::operator=(const URI &url)
		{
			if (this != &url) {
				_path = url._path;
				delete _scheme;
				_scheme = new std::string(*url._scheme);
				delete _username;
				_username = new std::string(*url._username);
				delete _password;
				_password = new std::string(*url._password);
				_address = url._address;
				delete _queries;
				_queries = new std::vector<std::pair<std::string, std::string>>(*url._queries);
				delete _fragment;
				_fragment = new std::string(*url._fragment);
			}
			return *this;
		}

		URI &URI::operator=(URI &&url) noexcept
		{
			if (this != &url) {
				_path = std::move(url._path);
				delete _scheme;
				_scheme = new std::string(move(*url._scheme));
				delete _username;
				_username = new std::string(move(*url._username));
				delete _password;
				_password = new std::string(move(*url._password));
				_address = url._address;
				delete _queries;
				_queries = new std::vector<std::pair<std::string, std::string>>(move(*url._queries));
				delete _fragment;
				_fragment = new std::string(move(*url._fragment));
			}
			return *this;
		}

		bool URI::operator==(const URI &url)
		{
			return *_scheme == (*url._scheme) && *_username == (*url._username) && *_password == (*url._password) && _address == url._address && _path == (url._path) &&
				   *_queries == (*url._queries) && *_fragment == (*url._fragment);
		}

		bool URI::operator!=(const URI &url)
		{
			return *_scheme != (*url._scheme) || *_username != (*url._username) || *_password != (*url._password) || _address != url._address || _path != url._path ||
				   *_queries != (*url._queries) || *_fragment != (*url._fragment);
		}

		URI URI::operator/(const URI &uri)
		{
			URI new_uri = URI(*this);
			for (const auto &s : uri.get_path())
				new_uri._path.emplace_back(s);
			return new_uri;
		}

		URI URI::operator/(const Path &path)
		{
			URI new_uri = URI(*this);
			for (const auto &s : path.get_path())
				new_uri._path.emplace_back(s);
			return new_uri;
		}

		URI LAMBDACOMMON_API from_file_path(fs::FilePath path)
		{
			if (!path.is_absolute())
				path = path.to_absolute();

			return URI("file", "", "", Address::EMPTY, path.get_path());
		}

		URI LAMBDACOMMON_API from_string(const std::string &url)
		{
			if (url.empty())
				throw IllegalArgumentException("URI cannot be empty.");

			auto scheme_separator = url.find_first_of("://");
			if (scheme_separator == std::string::npos) {
				scheme_separator = url.find_first_of(':');
				if (scheme_separator == std::string::npos)
					throw ParseException("Cannot parse uri '" + url + "': cannot find the scheme separator (':' or '://')!");
			}
			auto scheme = url.substr(0, scheme_separator);
			auto tmp_uri = url.substr(scheme_separator + 3, url.size());

			auto auth_separator = tmp_uri.find_first_of('@');
			std::string username;
			std::string password;
			if (auth_separator != std::string::npos) {
				auto user_and_pwd = tmp_uri.substr(0, auth_separator);
				auto split = lstring::split(user_and_pwd, ':');
				if (split.size() == 1)
					throw ParseException("Cannot parse uri '" + url + "': missing username or password!");
				username = split[0];
				password = split[1];

				tmp_uri = tmp_uri.substr(auth_separator + 1, url.size());
			}

			auto path_separator = tmp_uri.find_first_of('/');
			auto query_separator = tmp_uri.find_first_of('?');
			auto fragment_separator = tmp_uri.find_first_of('#');

			if ((query_separator != std::string::npos && path_separator > query_separator) || (fragment_separator != std::string::npos && path_separator > fragment_separator))
				throw ParseException("Cannot parse uri '" + url + "', malformed URI: path is before query and fragment!");

			std::string tmp_address;

			std::string path;
			if (path_separator != std::string::npos) {
				tmp_address = tmp_uri.substr(0, path_separator);
				if (query_separator != std::string::npos || fragment_separator != std::string::npos) {
					if (query_separator < fragment_separator || fragment_separator == std::string::npos)
						path = tmp_uri.substr(path_separator + 1, (query_separator - path_separator - 1));
					else
						path = tmp_uri.substr(path_separator + 1, (fragment_separator - path_separator - 1));
				} else
					path = tmp_uri.substr(path_separator);
			}

			std::vector<std::pair<std::string, std::string>> queries;
			if (query_separator != std::string::npos) {
				std::string str_queries;
				if (fragment_separator > query_separator) {
					str_queries = tmp_uri.substr(query_separator + 1, (fragment_separator - query_separator - 1));
					if (tmp_address.empty())
						tmp_address = tmp_uri.substr(0, query_separator);
				} else
					str_queries = tmp_uri.substr(query_separator + 1, tmp_uri.size());

				if (!str_queries.empty()) {
					auto splitted_queries = lstring::split(str_queries, '&');
					for (auto query : splitted_queries) {
						size_t j;
						if ((j = query.find_first_of('=')) != std::string::npos)
							queries.emplace_back(query.substr(0, j), query.substr(j + 1, query.size()));
						else
							queries.emplace_back(query, "");
					}
				}
			}

			std::string fragment;
			if (fragment_separator != std::string::npos) {
				if (query_separator > fragment_separator) {
					fragment = tmp_uri.substr(fragment_separator + 1, (query_separator - fragment_separator - 1));
					if (tmp_address.empty())
						tmp_address = tmp_uri.substr(0, fragment_separator);
				} else
					fragment = tmp_uri.substr(fragment_separator + 1, tmp_uri.size());
			}

			Address address = Address::EMPTY;
			if (!tmp_address.empty()) {
				auto address_separator = tmp_address.find_last_of(':');
				auto end_ipv6 = tmp_address.find_last_of(']');
				std::string host;
				if (end_ipv6 != std::string::npos && end_ipv6 > address_separator)
					host = tmp_address;
				else
					host = tmp_address.substr(0, address_separator);
				host = lstring::replace_all(lstring::replace_all(host, "[", ""), "]", "");
				port_t port = 0;
				if (address_separator != std::string::npos && address_separator > end_ipv6) {
					auto opt_port = lstring::parse_int(tmp_address.substr(address_separator + 1, tmp_address.size()));
					if (!opt_port)
						throw ParseException("Cannot parse uri '" + url + "': invalid port!");
					else port = static_cast<port_t>(*opt_port);
				}
				address = {host, port};
			}

			URI result_uri{scheme, username, password, address, serializable::tokenize(path, "/"), queries, fragment};

			if (result_uri.get_address().is_empty() && is_scheme_type_non_file_special(result_uri.get_scheme_type()))
				throw ParseException("Cannot parse uri '" + url + "': the scheme doesn't support empty address!");

			return result_uri;
		}
	}
}