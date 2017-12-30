/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_UTILS_H
#define LAMBDACOMMON_UTILS_H

#include "lambdacommon.h"
#include <vector>

namespace lambdacommon
{
    namespace utils
    {
        extern std::string LAMBDACOMMON_API toStringForVector(const std::vector<std::string> &vec);

        extern std::ostream LAMBDACOMMON_API &
        operator<<(std::ostream &stream, const std::vector<std::string> &vectorToPrint);
    }
}

#endif //LAMBDACOMMON_UTILS_H
