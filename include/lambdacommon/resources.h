/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
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
		std::string getDomain() const;

		/*!
		 * Gets the name of the resource.
		 * @return The name of the resource.
		 */
		std::string getName() const;

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

#endif //LAMBDACOMMON_RESOURCES_H
