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

	/*!
	 * ResourceName
	 *
	 * Represents a resource identifier.
	 */
	class LAMBDACOMMON_API ResourceName
	{
	private:
		std::string _domain;
		std::string _path;

	public:
		ResourceName(const std::string &name);

		ResourceName(const std::string &domain, const std::string &path) noexcept;

		ResourceName(const ResourceName &other);

		ResourceName(ResourceName &&other) noexcept;

		/*!
		 * Gets the domain of the resource.
		 * @return The domain of the resource.
		 */
		const std::string &get_domain() const;

		/*!
		 * Gets the name of the resource.
		 * @return The name of the resource.
		 */
		const std::string &get_name() const;

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
		std::string to_string() const;

		/*!
		 * Creates a new {@code ResourceName} from this resource name.
		 * @param path The path to append.
		 * @return The new {@code ResourceName} with the appended path.
		 */
		inline ResourceName operator/(const std::string &path) const
		{
			return sub(path);
		}

		ResourceName &operator=(const ResourceName &other);

		ResourceName &operator=(ResourceName &&other) noexcept;

		bool operator==(const ResourceName &other) const;

		bool operator<(const ResourceName &other) const;
	};

	/*!
	 * ResourcesManager
	 *
	 * Represents a resources manager.
	 */
	class LAMBDACOMMON_API ResourcesManager
	{
	protected:
		uint32_t _id;

	public:
		ResourcesManager();

		ResourcesManager(const ResourcesManager &other);

		ResourcesManager(ResourcesManager &&other) noexcept;

		/*!
		 * Gets the identifier of the resources manager.
		 * @return The identifier.
		 */
		uint32_t get_id() const;

		/*! @brief Checks whether the resources exists or not.
		 *
		 * @param resource The resource to check.
		 * @return True of the resource exists, else false.
		 */
		virtual bool has_resource(const ResourceName &resource) const = 0;

		/*! @brief Checks whether the resource exists or not.
		 *
		 * @param resource The resource to check.
		 * @param extension The extension of the resource file.
		 * @return True if the resource exists, else false.
		 */
		virtual bool has_resource(const ResourceName &resource, const std::string &extension) const = 0;

		/*!
		 * Loads the resource content into a string value.
		 * @param resource The resource to load.
		 * @return The resource content if successfully loaded, else an empty string.
		 */
		virtual std::string load_resource(const ResourceName &resource) const = 0;

		/*! @brief Loads the resource content into a string value.
		 *
		 * @param resource The resource to load.
		 * @param extension The extension of the resource file.
		 * @return The resource content if successfully loaded,X else an empty string.
		 */
		virtual std::string load_resource(const ResourceName &resource, const std::string &extension) const = 0;

		bool operator==(const ResourcesManager &other) const;
	};

	class LAMBDACOMMON_API FileResourcesManager : public ResourcesManager
	{
	private:
		lambdacommon::fs::FilePath _working_directory;

	public:
		FileResourcesManager(const lambdacommon::fs::FilePath &working_directory = fs::get_current_working_directory());

		FileResourcesManager(const FileResourcesManager &other);

		FileResourcesManager(FileResourcesManager &&other) noexcept;

		/*! @brief Gets the working directory of the resource manager.
		 *
		 * @return The working directory.
		 */
		const lambdacommon::fs::FilePath &get_working_directory() const;

		bool has_resource(const ResourceName &resource) const override;

		bool has_resource(const ResourceName &resource, const std::string &extension) const override;

		lambdacommon::fs::FilePath
		get_resource_path(const ResourceName &resource, const std::string &extension) const;

		std::string load_resource(const ResourceName &resource) const override;

		std::string load_resource(const ResourceName &resource, const std::string &extension) const override;

		FileResourcesManager &operator=(const FileResourcesManager &other);

		FileResourcesManager &operator=(FileResourcesManager &&other) noexcept;

		bool operator==(const FileResourcesManager &other) const;
	};
}

#ifdef LAMBDA_WINDOWS
#  pragma warning(pop)
#endif

#endif //LAMBDACOMMON_RESOURCES_H