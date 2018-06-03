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

#include "string.h"

namespace lambdacommon
{
	class ResourceName
	{
	private:
		std::string _domain;
		std::string _path;

	public:
		ResourceName(const std::string &name);

		ResourceName(std::string domain, std::string path);

		/*!
		 * Gets the domain of the resource.
		 * @return The domain.
		 */
		std::string getDomain();

		/*!
		 * Gets the name of the resource.
		 * @return The name of the resource.
		 */
		std::string getName();

		/*!
		 * Creates a new {@code ResourceName} from this resource name.
		 * @param path The path to append.
		 * @return The new {@code ResourceName} with the appended path.
		 */
		ResourceName sub(const std::string &path);

		/*!
		 * Creates a new {@code ResourceName} from this resource name.
		 * @param path The path to append.
		 * @return The new {@code ResourceName} with the appended path.
		 */
		ResourceName operator/(const std::string &path)
		{
			return sub(path);
		}
	};
}

#endif //LAMBDACOMMON_RESOURCES_H
