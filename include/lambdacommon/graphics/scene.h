/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_SCENE_H
#define LAMBDACOMMON_SCENE_H

#include "../types.h"
#include <utility>

namespace lambdacommon
{
    namespace graphics
    {
        using namespace std::rel_ops;

        class Scene
        {
        protected:
            const uint32_t id;

        public:
            Scene(uint32_t id);

            /*!
             * Gets the ID which identify the Scene.
             * @return The ID.
             */
            uint32_t get_id() const;

            /*!
             * Updates every component of the Scene.
             */
            virtual void update() = 0;

            bool operator==(const Scene &other) const;

            bool operator<(const Scene &other) const;
        };

        class Scene2D : public Scene
        {
        protected:
            Size2D_u32 size;

        public:
            Scene2D(const Size2D_u32 &size);

            const Size2D_u32 &get_size() const;

            void set_size(const Size2D_u32 &size);
        };
    }
}

#endif //LAMBDACOMMON_SCENE_H
