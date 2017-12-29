/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_PATH_H
#define LAMBDACOMMON_PATH_H

#include "serializable.h"

namespace lambdacommon
{
    class LAMBDACOMMON_API Path : public Serializable
    {
    protected:
        std::vector<std::string> *_path;

    public:
        Path();

        Path(const std::vector<std::string> &path);

        virtual ~Path();

        std::vector<std::string> getPath() const;

        virtual std::string toString() const;

        virtual std::string toString(char delimiter = '/') const;
    };
}

#endif //LAMBDACOMMON_PATH_H