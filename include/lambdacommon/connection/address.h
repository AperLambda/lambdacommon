/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
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

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4251)
#endif

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

	/*!
	 * Represents a network address.
	 */
	class LAMBDACOMMON_API Address : public Object, public Nullable
	{
	protected:
		host   _host;
		port_t _port;

	public:
		Address(host host, port_t port = 0);

		Address(const Address &address);

		Address(Address &&address) noexcept;

		~Address();

		/*!
		 * Gets the host of the address.
		 * @return The host of the address.
		 */
		const host &get_host() const;

		/*!
		 * Gets the port of the address.
		 * @return The port of the address.
		 */
		port_t get_port() const;

		/*!
		 * Checks whether the address is an IPv4 address.
		 * @return True if the address is an IPv4 address, else false.
		 */
		bool is_ipv4() const;

		/*!
		 * Checks whether the address is an IPv6 address.
		 * @return True if the address is an IPv6 address, else false.
		 */
		bool is_ipv6() const;

		bool is_domain_valid() const;

		/*!
		 * Checks whether the address is empty.
		 * @return True if the address is empty, else false.
		 */
		bool is_empty() const;

		/*!
		 * Gets the type of address.
		 * @return The type of address.
		 */
		AddressType get_type() const;

		bool is_null() const override;

		std::string to_string() const override;

		Address &operator=(const Address &other);

		Address &operator=(Address &&other) noexcept;

		bool operator==(const Address &other) const;

		bool operator<(const Address &other) const;

		template<std::size_t N>
		decltype(auto) get() const
		{
			if constexpr (N == 0) return this->_host;
			else if constexpr (N == 1) return this->_port;
		}

		static Address EMPTY;
	};
}


// Structured bindings for lambdacommon::Address.
namespace std
{
	template<>
	struct tuple_size<lambdacommon::Address> : std::integral_constant<std::size_t, 2>
	{};

	template<std::size_t N>
	struct tuple_element<N, lambdacommon::Address>
	{
		using type = decltype(std::declval<lambdacommon::Address>().get<N>());
	};
}

#ifdef LAMBDA_WINDOWS
#  pragma warning(pop)
#endif

#endif //LAMBDACOMMON_ADDRESS_H
