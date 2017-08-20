/*
 * Copyright © 2017 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/lambdacommon/path.h"
#include <sstream>

using namespace std;

namespace lambdacommon
{
    Path::Path() : _path(new vector<string>())
    {}

    Path::Path(const vector<string> &path) : _path(new vector<string>(path))
    {}

    Path::~Path()
    {
        delete _path;
    }

    vector<string> Path::getPath() const
    {
        return *_path;
    }

    string Path::toString() const
    {
        ostringstream oss;
        for (size_t i = 0; i < _path->size(); i++)
        {
            oss << (*_path)[i];
            if (i + 1 < _path->size())
                oss << '/';
        }
        return oss.str();
    }
}