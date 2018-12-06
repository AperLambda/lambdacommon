/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_URL_H
#define LAMBDACOMMON_URL_H

#include "fs/filesystem.h"
#include "../connection/address.h"

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4101)
#endif

namespace lambdacommon
{
	namespace uri
	{
		enum SchemeType
		{
			FILE,
			FTP,
			GOPHER,
			HTTP,
			HTTPS,
			WS,
			WSS,
			OTHER
		};

		extern SchemeType LAMBDACOMMON_API get_scheme_type_by_string(const std::string &scheme);

		extern port_t LAMBDACOMMON_API get_scheme_default_port(SchemeType scheme);

		extern bool LAMBDACOMMON_API is_scheme_type_non_file_special(SchemeType scheme);

		/**
		 * Based on https://url.spec.whatwg.org
		 */
		class LAMBDACOMMON_API URI : public Path
		{
		protected:
			pstring _scheme;
			pstring _username;
			pstring _password;
			Address _address;
			std::vector<std::pair<std::string, std::string>> *_queries;
			pstring _fragment;

		public:
			URI(const std::string &scheme, const std::string &username, const std::string &password,
				const Address &address, const std::vector<std::string> &path = std::vector<std::string>(),
				const std::vector<std::pair<std::string, std::string>> &queries = std::vector<std::pair<std::string, std::string>>(),
				const std::string &fragment = "");

			URI(const URI &uri);

			URI(URI &&uri) noexcept;

			~URI() override;

			std::string get_scheme() const;

			inline SchemeType get_scheme_type() const
			{
				return get_scheme_type_by_string(get_scheme());
			}

			std::string get_username() const;

			std::string get_password() const;

			void set_username_and_password(const std::string &username, const std::string &password);

			Address get_address() const;

			void set_address(const Address &address);

			std::vector<std::pair<std::string, std::string>> get_queries() const;

			void set_queries(const std::vector<std::pair<std::string, std::string>> &queries);

			bool has_query(std::string query) const;

			std::string get_query(std::string query) const;

			std::string get_fragment() const;

			void set_fragment(const std::string &fragment);

			std::string to_string(char delimiter = '/') const override;

			/*!
			 * Opens the URI in the default assigned application.
			 */
			void open_in_system() const;

			URI &operator=(const URI &uri);

			URI &operator=(URI &&uri) noexcept;

			bool operator==(const URI &uri);

			bool operator!=(const URI &uri);

			URI operator/(const URI &uri);

			URI operator/(const Path &path);
		};

		/**
		 * Returns an URL from a file path.
		 * @param path Filesystem's file path.
		 * @return An URL.
		 */
		extern URI LAMBDACOMMON_API from_file_path(fs::FilePath path = fs::get_current_working_directory());

		extern URI LAMBDACOMMON_API from_string(const std::string &uri);
	}
}

#endif //LAMBDACOMMON_URL_H