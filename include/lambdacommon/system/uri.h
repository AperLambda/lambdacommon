/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_URL_H
#define LAMBDACOMMON_URL_H

#include "fs.h"
#include "../connection/address.h"
#include <map>

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

        extern SchemeType LAMBDACOMMON_API get_scheme_type_by_string(const std::string& scheme);

        extern port_t LAMBDACOMMON_API get_scheme_default_port(SchemeType scheme);

        extern bool LAMBDACOMMON_API is_scheme_type_non_file_special(SchemeType scheme);

        /**
         * Based on https://url.spec.whatwg.org
         */
        class LAMBDACOMMON_API URI : public AbstractPath
        {
        protected:
            std::string _scheme;
            std::string _username;
            std::string _password;
            Address _address;
            std::map<std::string, std::string> _queries;
            std::string _fragment;

        public:
            URI(std::string scheme, std::string username, std::string password, Address address, std::string path = "", const std::map<std::string, std::string>& queries = {},
                std::string fragment = "");

            URI(const URI& uri);

            URI(URI&& uri) noexcept;

            const std::string& get_scheme() const;

            inline SchemeType get_scheme_type() const {
                return get_scheme_type_by_string(get_scheme());
            }

            const std::string& get_username() const;

            const std::string& get_password() const;

            void set_username_and_password(std::string username, std::string password);

            const Address& get_address() const;

            void set_address(Address address);

            std::map<std::string, std::string> get_queries() const;

            void set_queries(const std::map<std::string, std::string>& queries);

            bool has_query(const std::string& query) const;

            std::string get_query(const std::string& query) const;

            const std::string& get_fragment() const;

            void set_fragment(std::string fragment);

            std::string to_string(char delimiter = '/') const override;

            /*!
             * Opens the URI in the default assigned application.
             */
            void open_in_system() const;

            URI& operator=(const URI& uri);

            URI& operator=(URI&& uri) noexcept;

            bool operator==(const URI& uri);

            bool operator!=(const URI& uri);

            URI operator/(const URI& uri);

            URI operator/(const AbstractPath& path);
        };

        /**
         * Returns an URL from a file path.
         * @param path Filesystem's file path.
         * @return An URL.
         */
        extern URI LAMBDACOMMON_API from_file_path(fs::path path = fs::current_path());

        extern URI LAMBDACOMMON_API from_string(const std::string& uri);
    }
}

#endif //LAMBDACOMMON_URL_H
