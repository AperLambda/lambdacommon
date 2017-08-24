/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/connection/url.h"
#include "../../include/lambdacommon/string.h"
#include "../../include/lambdacommon/exceptions/exceptions.h"
#include <sstream>
#include <regex>

using namespace std;

namespace lambdacommon
{
    namespace URL
    {
        SchemeType LAMBDACOMMON_API getSchemeTypeByString(const string &scheme)
        {
            if (lambdastring::equalsIgnoreCase(scheme, "file"))
                return FILE;
            else if (lambdastring::equalsIgnoreCase(scheme, "ftp"))
                return FTP;
            else if (lambdastring::equalsIgnoreCase(scheme, "gopher"))
                return GOPHER;
            else if (lambdastring::equalsIgnoreCase(scheme, "http"))
                return HTTP;
            else if (lambdastring::equalsIgnoreCase(scheme, "https"))
                return HTTPS;
            else if (lambdastring::equalsIgnoreCase(scheme, "ws"))
                return WS;
            else if (lambdastring::equalsIgnoreCase(scheme, "wss"))
                return WSS;
            else
                return OTHER;
        }

        port LAMBDACOMMON_API getSchemeDefaultPort(SchemeType scheme)
        {
            switch (scheme)
            {
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

        bool LAMBDACOMMON_API isSchemeTypeNonFileSpecial(SchemeType scheme)
        {
            return scheme != FILE && scheme != OTHER;
        }

        /*
         * URL
         */

        vector<pair<string, string>> fixQueries(const vector<pair<string, string>> &queries)
        {
            vector<pair<string, string>> newQueries;
            for (auto query : queries)
                newQueries.emplace_back(lambdastring::replaceAll(query.first, " ", "%20"),
                                        lambdastring::replaceAll(query.second, " ", "%20"));
            return newQueries;
        };

        URL::URL(const string &scheme, const string &username, const string &password, const Address &address,
                 const vector<string> &path, const vector<pair<string, string>> &queries, const string &fragment)
                : Path(path),
                  _scheme(new string(scheme)),
                  _username(new string(username)),
                  _password(new string(password)),
                  _address(address),
                  _queries(new vector<pair<string, string>>(fixQueries(queries))),
                  _fragment(new string(fragment))
        {}

        URL::URL(const URL &url) : Path(*url._path), _scheme(new string(*url._scheme)),
                                   _username(new string(*url._username)), _password(new string(*url._password)),
                                   _address(url._address), _queries(new vector<pair<string, string>>(*url._queries)),
                                   _fragment(new string(*url._fragment))
        {}

        URL::URL(URL &&url) : Path(*url._path), _scheme(new string(move(*url._scheme))),
                              _username(new string(move(*url._username))), _password(new string(move(*url._password))),
                              _address(url._address), _queries(new vector<pair<string, string>>(move(*url._queries))),
                              _fragment(new string(move(*url._fragment)))
        {}

        URL::~URL()
        {
            delete _scheme;
            delete _username;
            delete _password;
            delete _fragment;
        }

        string URL::getScheme() const
        {
            return *_scheme;
        }

        string URL::getUsername() const
        {
            return *_username;
        }

        string URL::getPassword() const
        {
            return *_password;
        }

        void URL::setUserAndPassword(const string &username, const string &password)
        {
            if (username.empty() || password.empty())
            {
                *_username = "";
                *_password = "";
            }
            else
            {
                *_username = username;
                *_password = password;
            }
        }

        Address URL::getAddress() const
        {
            return _address;
        }

        void URL::setAddress(const Address &address)
        {
            if (address.getHost().empty() && isSchemeTypeNonFileSpecial(getSchemeType()))
                throw IllegalArgumentException("Hostname cannot be null for this scheme.");

            _address = address;
        }

        vector<pair<string, string>> URL::getQueries() const
        {
            return *_queries;
        }

        void URL::setQueries(const vector<pair<string, string>> &queries)
        {
            *_queries = fixQueries(queries);
        }

        bool URL::hasQuery(string query) const
        {
            bool hasQuery = false;
            for (auto q : *_queries)
                if (lambdastring::equals(q.first, query))
                {
                    hasQuery = true;
                    break;
                }
            return hasQuery;
        }

        string URL::getQuery(string query) const
        {
            string result = "";
            for (auto q : *_queries)
                if (lambdastring::equals(q.first, query))
                {
                    result = q.second;
                    break;
                }
            return result;
        }

        string URL::getFragment() const
        {
            return *_fragment;
        }

        void URL::setFragment(const string &fragment)
        {
            *_fragment = fragment;
        }

        string URL::toString() const
        {
            ostringstream oss;

            // Write the scheme
            if (!_scheme->empty())
                oss << getScheme() << "://";

            if (!_address.isEmpty())
            {
                // Write the username and the password
                if (!_username->empty() && !_password->empty())
                    oss << getUsername() << ':' << getPassword() << '@';

                // Write the address.
                oss << _address.toString();
            }

            // Write the path.
            if (!_path->empty())
                oss << '/' << Path::toString();

            if (!_queries->empty())
            {
                oss << '?';
                for (size_t i = 0; i < _queries->size(); i++)
                {
                    pair<string, string> query = (*_queries)[i];
                    oss << query.first;
                    if (!query.second.empty())
                        oss << '=' << query.second;

                    if (i + 1 != _queries->size())
                        oss << '&';
                }
            }

            if (!_fragment->empty())
                oss << '#' << getFragment();

            return oss.str();
        }

        URL &URL::operator=(const URL &url)
        {
            delete _path;
            _path = new vector<string>(*url._path);
            delete _scheme;
            _scheme = new string(*url._scheme);
            delete _username;
            _username = new string(*url._username);
            delete _password;
            _password = new string(*url._password);
            _address = url._address;
            delete _queries;
            _queries = new vector<pair<string, string>>(*url._queries);
            delete _fragment;
            _fragment = new string(*url._fragment);
            return *this;
        }

        URL &URL::operator=(URL &&url) noexcept
        {
            if (this != &url)
            {
                delete _path;
                _path = new vector<string>(move(*url._path));
                delete _scheme;
                _scheme = new string(move(*url._scheme));
                delete _username;
                _username = new string(move(*url._username));
                delete _password;
                _password = new string(move(*url._password));
                _address = url._address;
                delete _queries;
                _queries = new vector<pair<string, string>>(move(*url._queries));
                delete _fragment;
                _fragment = new string(move(*url._fragment));
            }
            return *this;
        }

        bool URL::operator==(const URL &url)
        {
            return *_scheme == (*url._scheme) && *_username == (*url._username) && *_password == (*url._password) &&
                   _address == url._address && *_path == (*url._path) && *_queries == (*url._queries) &&
                   *_fragment == (*url._fragment);
        }

        bool URL::operator!=(const URL &url)
        {
            return *_scheme != (*url._scheme) || *_username != (*url._username) || *_password != (*url._password) ||
                   _address != url._address || *_path != (*url._path) || *_queries != (*url._queries) ||
                   *_fragment != (*url._fragment);
        }

        URL LAMBDACOMMON_API fromFilePath(filesystem::FilePath path)
        {
            if (!path.isAbsolute())
                path = path.toAbsolute();

            return URL("file", "", "", Address::EMPTY, path.getPath());
        }

        int str2int(const string &str)
        {
            int i;
            try
            {
                i = stoi(str);
            } catch (invalid_argument e)
            {
                i = 0;
            }
            catch (out_of_range e)
            {
                i = 0;
            }
            return i;
        }

        URL LAMBDACOMMON_API fromString(const string &url)
        {
            if (url.empty())
                throw IllegalArgumentException("URL cannot be empty.");

            auto schemeSeparator = url.find_first_of("://");
            if (schemeSeparator == string::npos)
                throw ParseException("Cannot parse url '" + url + "': cannot find the scheme separator ('://')!");
            string scheme = url.substr(0, schemeSeparator);
            auto tmpUrl = url.substr(schemeSeparator + 3, url.size());

            auto authSeparator = tmpUrl.find_first_of('@');
            string username;
            string password;
            if (authSeparator != string::npos)
            {
                string userAndPwd = tmpUrl.substr(0, authSeparator);
                auto split = lambdastring::split(userAndPwd, ':');
                if (split.size() == 1)
                    throw ParseException("Cannot parse url '" + url + "': missing username or password!");
                username = split[0];
                password = split[1];

                tmpUrl = tmpUrl.substr(authSeparator + 1, url.size());
            }

            auto pathSeparator = tmpUrl.find_first_of('/');
            auto querySeparator = tmpUrl.find_first_of('?');
            auto fragmentSeparator = tmpUrl.find_first_of('#');

            if ((querySeparator != string::npos && pathSeparator > querySeparator) ||
                (fragmentSeparator != string::npos && pathSeparator > fragmentSeparator))
                throw ParseException(
                        "Cannot parse url '" + url + "', malformed URL: path is before query and fragment!");

            string tmpAddress;

            string path;
            if (pathSeparator != string::npos)
            {
                tmpAddress = tmpUrl.substr(0, pathSeparator);
                if (querySeparator != string::npos || fragmentSeparator != string::npos)
                {
                    if (querySeparator < fragmentSeparator || fragmentSeparator == string::npos)
                        path = tmpUrl.substr(pathSeparator + 1, (querySeparator - pathSeparator - 1));
                    else
                        path = tmpUrl.substr(pathSeparator + 1, (fragmentSeparator - pathSeparator - 1));
                }
                else
                    path = tmpUrl;
            }

            vector<pair<string, string>> queries;
            if (querySeparator != string::npos)
            {
                string strQueries;
                if (fragmentSeparator > querySeparator)
                {
                    strQueries = tmpUrl.substr(querySeparator + 1, (fragmentSeparator - querySeparator - 1));
                    if (tmpAddress.empty())
                        tmpAddress = tmpUrl.substr(0, querySeparator);
                }
                else
                    strQueries = tmpUrl.substr(querySeparator + 1, tmpUrl.size());

                if (!strQueries.empty())
                {
                    auto splittedQueries = lambdastring::split(strQueries, '&');
                    for (auto query : splittedQueries)
                    {
                        size_t j;
                        if ((j = query.find_first_of('=')) != string::npos)
                            queries.emplace_back(query.substr(0, j), query.substr(j + 1, query.size()));
                        else
                            queries.emplace_back(query, "");
                    }
                }
            }

            string fragment;
            if (fragmentSeparator != string::npos)
            {
                if (querySeparator > fragmentSeparator)
                {
                    fragment = tmpUrl.substr(fragmentSeparator + 1, (querySeparator - fragmentSeparator - 1));
                    if (tmpAddress.empty())
                        tmpAddress = tmpUrl.substr(0, fragmentSeparator);
                }
                else
                    fragment = tmpUrl.substr(fragmentSeparator + 1, tmpUrl.size());
            }

            Address address = Address::EMPTY;
            if (!tmpAddress.empty())
            {
                auto addressSeparator = tmpAddress.find_last_of(':');
                auto endIPv6 = tmpAddress.find_last_of(']');
                string host;
                if (endIPv6 != string::npos && endIPv6 > addressSeparator)
                    host = tmpAddress;
                else
                    host = tmpAddress.substr(0, addressSeparator);
                host = lambdastring::replaceAll(lambdastring::replaceAll(host, "[", ""), "]", "");
                port port = 0;
                if (addressSeparator != string::npos && addressSeparator > endIPv6)
                {
                    if ((port = static_cast<lambdacommon::port>(str2int(
                            tmpAddress.substr(addressSeparator + 1, tmpAddress.size())))) == 0)
                        throw ParseException("Cannot parse url '" + url + "': invalid port!");
                }
                address = {host, port};
            }

            URL resultURL{scheme, username, password, address, serializable::tokenize(path, "/"), queries, fragment};

            if (resultURL.getAddress().isEmpty() && isSchemeTypeNonFileSpecial(resultURL.getSchemeType()))
                throw ParseException("Cannot parse url '" + url + "': the scheme doesn't support empty address!");

            return resultURL;
        }
    }
}
