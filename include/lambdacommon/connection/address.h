/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_ADDRESS_H
#define LAMBDACOMMON_ADDRESS_H

#include "../serializable.h"
#include <array>

namespace lambdacommon
{
    typedef std::string host;
    typedef uint16_t port;

    enum AddressType
    {
        DOMAIN_NAME,
        IPv4,
        IPv6,
        EMPTY,
        INVALID
    };

    class LAMBDACOMMON_API Address : public Serializable
    {
    protected:
        host *_host;
        port _port;

    public:
        Address(const host &host, port port = 0);

        Address(const Address &address);

        Address(Address &&address);

        ~Address();

        host getHost() const;

        port getPort() const;

        bool isIPv4() const;

        bool isIPv6() const;

        bool isValidDomain() const;

        bool isEmpty() const;

        AddressType getType() const;

        std::string toString() const;

        Address &operator=(const Address &address);

        Address &operator=(Address &&address) noexcept;

        bool operator==(const Address &address);

        bool operator!=(const Address &address);

        static Address EMPTY;
    };
}

#endif //LAMBDACOMMON_ADDRESS_H