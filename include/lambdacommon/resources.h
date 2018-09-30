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

#include "system/fs/filesystem.h"
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
		 * @return The domain of the resource.
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
		 * Gets the resource name as a string.
		 * @return The resource name as a string.
		 */
		std::string toString() const;

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

	class LAMBDACOMMON_API ResourcesManager
	{
	private:
		lambdacommon::fs::FilePath _workingDirectory;

	public:
		ResourcesManager(const lambdacommon::fs::FilePath &workingDirectory = fs::getCurrentWorkingDirectory());

		ResourcesManager(const ResourcesManager &resourcesManager);

		ResourcesManager(ResourcesManager &&resourcesManager) noexcept;

		/*! @brief Gets the working directory of the resource manager.
		 *
		 * @return The working directory.
		 */
		const lambdacommon::fs::FilePath &getWorkingDirectory() const;

		/*! @brief Checks whether the resource exists or not.
		 *
		 * @param resourceName The resource to check.
		 * @param extension The extension of the resource file.
		 * @return True if the resource exists else false.
		 */
		bool doesResourceExist(const ResourceName &resourceName, const std::string &extension) const;

		lambdacommon::fs::FilePath
		getResourcePath(const ResourceName &resourceName, const std::string &extension) const;

		/*! @brief Loads the resource content into a string value.
		 *
		 * @param resourceName The resource to load.
		 * @param extension The extension of the resource file.
		 * @return The resource content if successfully loaded else an empty string.
		 */
		std::string loadResource(const ResourceName &resourceName, const std::string &extension) const;

		ResourcesManager &operator=(const ResourcesManager &resourcesManager);

		ResourcesManager &operator=(ResourcesManager &&resourcesManager) noexcept;

		bool operator==(const ResourcesManager &other) const;
	};
}

#ifdef LAMBDA_WINDOWS
#  pragma warning(pop)
#endif

#endif //LAMBDACOMMON_RESOURCES_H