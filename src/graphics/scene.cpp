/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../../include/lambdacommon/graphics/scene.h"

namespace lambdacommon
{
	namespace graphics
	{
		Scene::Scene(uint32_t id) : id(id)
		{}

		uint32_t Scene::get_id() const
		{
			return id;
		}

		bool Scene::operator==(const Scene &other) const
		{
			return id == other.id;
		}

		bool Scene::operator<(const Scene &other) const
		{
			return id < other.id;
		}

		uint32_t last_scene2d_id = 1;

		Scene2D::Scene2D(const Size2D_u32 &size) : Scene(last_scene2d_id++), size(size)
		{}

		const Size2D_u32 &Scene2D::get_size() const
		{
			return size;
		}

		void Scene2D::set_size(const Size2D_u32 &size)
		{
			Scene2D::size = size;
		}
	}
}