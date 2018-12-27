/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_PATH_H
#define LAMBDACOMMON_PATH_H

#include "object.h"
#include "serializable.h"

#ifdef LAMBDA_WINDOWS
#  pragma warning(push)
#  pragma warning(disable:4251)
#endif

namespace lambdacommon
{
	/*!
	 * Path
	 *
	 * Represents a path.
	 */
	class LAMBDACOMMON_API Path : public Object
	{
	protected:
		std::vector<std::string> _path;

	public:
		Path();

		Path(const std::vector<std::string> &path);

		Path(const Path &path);

		Path(Path &&path) noexcept;

		virtual ~Path();

		/*!
		 * Gets the path value.
		 * @return The path value.
		 */
		const std::vector<std::string> &get_path() const;

		virtual std::string to_string() const;

		virtual std::string to_string(char delimiter = '/') const;
	};
}

#endif //LAMBDACOMMON_PATH_H