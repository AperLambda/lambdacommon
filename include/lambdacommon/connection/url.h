/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_URL_H
#define LAMBDACOMMON_URL_H

#include "../system/filesystem/filesystem.h"
#include "address.h"

namespace lambdacommon
{
    namespace URL
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
        class LAMBDACOMMON_API URL : public Path
        {
        protected:
            pstring _scheme;
            pstring _username;
            pstring _password;
            Address _address;
            std::vector<std::pair<std::string, std::string>> *_queries;
            pstring _fragment;

        public:
            URL(const std::string &scheme, const std::string &username, const std::string &password,
                const Address &address, const std::vector<std::string> &path = std::vector<std::string>(),
                const std::vector<std::pair<std::string, std::string>> &queries = std::vector<std::pair<std::string, std::string>>(),
                const std::string &fragment = "");

            URL(const URL &url);

            URL(URL &&url);

            ~URL();

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

            std::string toString() const;

            URL &operator=(const URL &url);

            URL &operator=(URL &&url) noexcept;

            bool operator==(const URL &url);

            bool operator!=(const URL &url);
        };

        /**
         * Returns an URL from a file path.
         * @param path Filesystem's file path.
         * @return An URL.
         */
        extern URL LAMBDACOMMON_API fromFilePath(filesystem::FilePath path = filesystem::getCurrentWorkingDirectory());

        extern URL LAMBDACOMMON_API fromString(const std::string &url);
    }
}

#endif //LAMBDACOMMON_URL_H