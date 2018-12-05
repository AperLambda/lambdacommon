/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_ADDRESS_H
#define LAMBDACOMMON_ADDRESS_H

#include "../serializable.h"
#include "../types.h"
#include <array>

namespace lambdacommon
{
	typedef std::string host;

	enum AddressType
	{
		DOMAIN_NAME,
		IPv4,
		IPv6,
		EMPTY,
		INVALID
	};

	class LAMBDACOMMON_API Address : public Object
	{
	protected:
		host *_host;
		port_t _port;

	public:
		Address(const host &host, port_t port = 0);

		Address(const Address &address);

		Address(Address &&address) noexcept;

		~Address();

		const host &get_host() const;

		port_t get_port() const;

		bool is_ipv4() const;

		bool is_ipv6() const;

		bool is_domain_valid() const;

		bool is_empty() const;

		AddressType get_type() const;

		std::string to_string() const;

		Address &operator=(const Address &other);

		Address &operator=(Address &&other) noexcept;

		bool operator==(const Address &other);

		bool operator!=(const Address &other);

		static Address EMPTY;
	};
}

#endif //LAMBDACOMMON_ADDRESS_H