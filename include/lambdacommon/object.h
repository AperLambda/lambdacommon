/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_OBJECT_H
#define LAMBDACOMMON_OBJECT_H

#include "lambdacommon.h"

namespace lambdacommon
{
	/*!
	 * Represents an object.
	 */
	class LAMBDACOMMON_API Object
	{
	public:
		/*!
		 * Gets the object as a string.
		 * @return The object as a string.
		 */
		virtual std::string to_string() const = 0;
	};
}

#endif //LAMBDACOMMON_OBJECT_H