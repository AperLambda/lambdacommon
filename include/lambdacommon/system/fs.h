/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_FS_H
#define LAMBDACOMMON_FS_H

#include "../lambdacommon.h"

namespace lambdacommon
{
	namespace fs
	{
		/*!
		 * Gets the current working directory as a wide string.
		 * @return The current working directory as a wide string.
		 */
		extern std::wstring LAMBDACOMMON_API get_cwd_wstr();

		/*!
		 * Gets the current working directory as a string.
		 * @return The current working directory as a string.
		 */
		extern std::string LAMBDACOMMON_API get_cwd_str();
	}
}

#endif //LAMBDACOMMON_FS_H