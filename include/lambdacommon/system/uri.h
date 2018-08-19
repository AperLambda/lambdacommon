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

#include "filesystem/filesystem.h"
#include "../connection/address.h"

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

		extern SchemeType LAMBDACOMMON_API getSchemeTypeByString(const std::string &scheme);

		extern port LAMBDACOMMON_API getSchemeDefaultPort(SchemeType scheme);

		extern bool LAMBDACOMMON_API isSchemeTypeNonFileSpecial(SchemeType scheme);

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

			std::string getScheme() const;

			inline SchemeType getSchemeType() const
			{
				return getSchemeTypeByString(getScheme());
			}

			std::string getUsername() const;

			std::string getPassword() const;

			void setUserAndPassword(const std::string &username, const std::string &password);

			Address getAddress() const;

			void setAddress(const Address &address);

			std::vector<std::pair<std::string, std::string>> getQueries() const;

			void setQueries(const std::vector<std::pair<std::string, std::string>> &queries);

			bool hasQuery(std::string query) const;

			std::string getQuery(std::string query) const;

			std::string getFragment() const;

			void setFragment(const std::string &fragment);

			std::string toString(char delimiter = '/') const override;

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
		extern URI LAMBDACOMMON_API fromFilePath(fs::FilePath path = fs::getCurrentWorkingDirectory());

		extern URI LAMBDACOMMON_API fromString(const std::string &uri);
	}
}

#endif //LAMBDACOMMON_URL_H