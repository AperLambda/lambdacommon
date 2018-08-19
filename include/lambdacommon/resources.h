/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_RESOURCES_H
#define LAMBDACOMMON_RESOURCES_H

#include "lstring.h"
#include <utility>
#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4251)
#endif

namespace lambdacommon
{
	using namespace std::rel_ops;

	class LAMBDACOMMON_API ResourceName
	{
	private:
		std::string _domain;
		std::string _path;

	public:
		ResourceName(const std::string &name);

		ResourceName(std::string domain, std::string path) noexcept;

		ResourceName(const ResourceName &resourceName);

		ResourceName(ResourceName &&resourceName) noexcept;

		/*!
		 * Gets the domain of the resource.
		 * @return The domain.
		 */
		const std::string &getDomain() const;

		/*!
		 * Gets the name of the resource.
		 * @return The name of the resource.
		 */
		const std::string &getName() const;

		/*!
		 * Creates a new {@code ResourceName} from this resource name.
		 * @param path The path to append.
		 * @return The new {@code ResourceName} with the appended path.
		 */
		ResourceName sub(const std::string &path) const;

		/*!
		 * Creates a new {@code ResourceName} from this resource name.
		 * @param path The path to append.
		 * @return The new {@code ResourceName} with the appended path.
		 */
		inline ResourceName operator/(const std::string &path) const
		{
			return sub(path);
		}

		ResourceName &operator=(const ResourceName &resourceName);

		ResourceName &operator=(ResourceName &&resourceName) noexcept;

		bool operator==(const ResourceName &other) const;

		bool operator<(const ResourceName &other) const;
	};
}

#ifdef LAMBDA_WINDOWS
#  pragma warning(pop)
#endif

#endif //LAMBDACOMMON_RESOURCES_H