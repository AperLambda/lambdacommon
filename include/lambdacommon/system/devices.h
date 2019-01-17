/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_DEVICES_H
#define LAMBDACOMMON_DEVICES_H

#include "../lambdacommon.h"

namespace lambdacommon
{
	/*!
	 * Represents the current status of a device.
	 */
	enum DeviceStatus
	{
		CONNECTED    = true,
		DISCONNECTED = false,
		WAITING      = 2
	};
}

#endif //LAMBDACOMMON_DEVICES_H
