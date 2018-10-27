/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_DIMENSIONS_H
#define LAMBDACOMMON_DIMENSIONS_H

#include "../../serializable.h"

namespace lambdacommon
{
	namespace maths
	{
		namespace geometry
		{
			using namespace std::rel_ops;

			class Dimension : public Serializable
			{
			public:
				/*!
				 * Checks whether the dimension is null.
				 * @return True if the dimension is null, else false.
				 */
				virtual bool is_null() const = 0;
			};

			template<typename T>
			class Dimension2D : public Dimension
			{
			protected:
				/*!
				 * The X coordinate.
				 */
				T width;
				/*!
				 * The Y coordinate.
				 */
				T height;

			public:
				Dimension2D(const T width, const T height) : width(width), height(height)
				{}

				/*!
				 * The width of the dimension.
				 * @return The width.
				 */
				T get_width() const
				{
					return width;
				}

				/*!
				 * Gets the height of the dimension.
				 * @return The height.
				 */
				T get_height() const
				{
					return height;
				}

				/*!
				 * Gets the dimension as a surface.
				 * @return The surface.
				 */
				T get_surface() const
				{
					return get_width() * get_height();
				}

				bool is_null() const override
				{
					return width == 0 && height == 0;
				}

				std::string to_string() const override
				{
					return R"({"width":)" + std::to_string(width) + R"(,"height":)" + std::to_string(height) + "}";
				}

				bool operator==(const Dimension2D<T> &other) const
				{
					return this->width == other.width && this->height == other.height;
				}

				bool operator<(const Dimension2D<T> &other) const
				{
					return this->get_surface() < other.get_surface();
				}

				virtual Dimension2D<T> &operator+=(T n)
				{
					this->width += n;
					this->height += n;
					return *this;
				}

				virtual Dimension2D<T> &operator-=(T n)
				{
					this->width -= n;
					this->height -= n;
					return *this;
				}

				virtual Dimension2D<T> &operator*=(T n)
				{
					this->width *= n;
					this->height *= n;
					return *this;
				}

				virtual Dimension2D<T> &operator/=(T n)
				{
					this->width /= n;
					this->height /= n;
					return *this;
				}

				virtual Dimension2D<T> &operator%=(T n)
				{
					this->width %= n;
					this->height %= n;
					return *this;
				}

				virtual Dimension2D<T> &operator^=(T n)
				{
					this->width ^= n;
					this->height ^= n;
					return *this;
				}

				virtual Dimension2D<T> &operator+=(const Dimension2D<T> &other)
				{
					this->width += other.width;
					this->height += other.height;
					return *this;
				}

				virtual Dimension2D<T> &operator-=(const Dimension2D<T> &other)
				{
					this->width -= other.width;
					this->height -= other.height;
					return *this;
				}

				virtual Dimension2D<T> &operator*=(const Dimension2D<T> &other)
				{
					this->width *= other.width;
					this->height *= other.height;
					return *this;
				}

				virtual Dimension2D<T> &operator/=(const Dimension2D<T> &other)
				{
					this->width /= other.width;
					this->height /= other.height;
					return *this;
				}

				virtual Dimension2D<T> &operator%=(const Dimension2D<T> &other)
				{
					this->width %= other.width;
					this->height %= other.height;
					return *this;
				}

				virtual Dimension2D<T> &operator^=(const Dimension2D<T> &other)
				{
					this->width ^= other.width;
					this->height ^= other.height;
					return *this;
				}

				friend const Dimension2D<T> operator+(Dimension2D<T> self, T n)
				{
					self += n;
					return self;
				}

				friend const Dimension2D<T> operator-(Dimension2D<T> self, T n)
				{
					self -= n;
					return self;
				}

				friend const Dimension2D<T> operator*(Dimension2D<T> self, T n)
				{
					self *= n;
					return self;
				}

				friend const Dimension2D<T> operator/(Dimension2D<T> self, T n)
				{
					self /= n;
					return self;
				}

				friend const Dimension2D<T> operator%(Dimension2D<T> self, T n)
				{
					self %= n;
					return self;
				}

				friend const Dimension2D<T> operator^(Dimension2D<T> self, T n)
				{
					self ^= n;
					return self;
				}

				friend const Dimension2D<T> operator+(Dimension2D<T> self, const Dimension2D<T> &other)
				{
					self += other;
					return self;
				}

				friend const Dimension2D<T> operator-(Dimension2D<T> self, const Dimension2D<T> &other)
				{
					self -= other;
					return self;
				}

				friend const Dimension2D<T> operator*(Dimension2D<T> self, const Dimension2D<T> &other)
				{
					self *= other;
					return self;
				}

				friend const Dimension2D<T> operator/(Dimension2D<T> self, const Dimension2D<T> &other)
				{
					self /= other;
					return self;
				}

				friend const Dimension2D<T> operator%(Dimension2D<T> self, const Dimension2D<T> &other)
				{
					self %= other;
					return self;
				}

				friend const Dimension2D<T> operator^(Dimension2D<T> self, const Dimension2D<T> &other)
				{
					self ^= other;
					return self;
				}
			};

			template<typename T>
			class Dimension3D : public Dimension2D<T>
			{
			protected:
				/*!
				 * The Z coordinate.
				 */
				T depth;

			public:
				Dimension3D(const T width, const T height, const T depth = 0) : Dimension2D<T>(width, height), depth(depth)
				{}

				/*!
				 * Gets the depth of the dimension.
				 * @return The depth.
				 */
				T get_depth() const
				{
					return depth;
				}

				/*!
				 * Transforms the 3D dimension into a 2D dimension.
				 * @return The 2D dimension from a 3D dimension.
				 */
				Dimension2D<T> to_2d() const
				{
					return {this->get_width(), this->get_height()};
				}

				/*!
				 * Gets the volume of the dimension.
				 * @return The volume.
				 */
				T get_volume() const
				{
					return this->get_surface() * get_depth();
				}

				bool is_null() const override
				{
					return Dimension2D<T>::is_null() && depth == 0;
				}

				std::string to_string() const override
				{
					return R"({"width":)" + std::to_string(this->get_width()) + R"(,"height":)" +
						   std::to_string(this->get_height()) + R"(,"depth":)" + std::to_string(depth) + "}";
				}

				bool operator==(const Dimension3D<T> &other) const
				{
					return this->width == other.width && this->height == other.height && this->depth = other.depth;
				}

				bool operator<(const Dimension3D<T> &other) const
				{
					return this->get_volume() < other.get_volume();
				}

				Dimension3D<T> &operator+=(T n) override
				{
					this->width += n;
					this->height += n;
					this->depth += n;
					return *this;
				}

				Dimension3D<T> &operator-=(T n) override
				{
					this->width -= n;
					this->height -= n;
					this->depth -= n;
					return *this;
				}

				Dimension3D<T> &operator*=(T n) override
				{
					this->width *= n;
					this->height *= n;
					this->depth *= n;
					return *this;
				}

				Dimension3D<T> &operator/=(T n) override
				{
					this->width /= n;
					this->height /= n;
					this->depth /= n;
					return *this;
				}

				Dimension3D<T> &operator%=(T n) override
				{
					this->width %= n;
					this->height %= n;
					this->depth %= n;
					return *this;
				}

				Dimension3D<T> &operator^=(T n) override
				{
					this->width ^= n;
					this->height ^= n;
					this->depth ^= n;
					return *this;
				}

				Dimension3D<T> &operator+=(const Dimension2D<T> &other) override
				{
					this->width += other.get_width();
					this->height += other.get_height();
					return *this;
				}

				Dimension3D<T> &operator-=(const Dimension2D<T> &other) override
				{
					this->width -= other.get_width();
					this->height -= other.get_height();
					return *this;
				}

				Dimension3D<T> &operator*=(const Dimension2D<T> &other) override
				{
					this->width *= other.get_width();
					this->height *= other.get_height();
					return *this;
				}

				Dimension3D<T> &operator/=(const Dimension2D<T> &other) override
				{
					this->width /= other.get_width();
					this->height /= other.get_height();
					return *this;
				}

				Dimension3D<T> &operator%=(const Dimension2D<T> &other) override
				{
					this->width %= other.get_width();
					this->height %= other.get_height();
					return *this;
				}

				Dimension3D<T> &operator^=(const Dimension2D<T> &other) override
				{
					this->width ^= other.get_width();
					this->height ^= other.get_height();
					return *this;
				}

				Dimension3D<T> &operator+=(const Dimension3D<T> &other)
				{
					this->width += other.width;
					this->height += other.height;
					this->depth += other.depth;
					return *this;
				}

				Dimension3D<T> &operator-=(const Dimension3D<T> &other)
				{
					this->width -= other.width;
					this->height -= other.height;
					this->depth -= other.depth;
					return *this;
				}

				Dimension3D<T> &operator*=(const Dimension3D<T> &other)
				{
					this->width *= other.width;
					this->height *= other.height;
					this->depth *= other.depth;
					return *this;
				}

				Dimension3D<T> &operator/=(const Dimension3D<T> &other)
				{
					this->width /= other.width;
					this->height /= other.height;
					this->depth /= other.depth;
					return *this;
				}

				Dimension3D<T> &operator%=(const Dimension3D<T> &other)
				{
					this->width %= other.width;
					this->height %= other.height;
					this->depth %= other.depth;
					return *this;
				}

				Dimension3D<T> &operator^=(const Dimension3D<T> &other)
				{
					this->width ^= other.width;
					this->height ^= other.height;
					this->depth ^= other.depth;
					return *this;
				}

				friend const Dimension3D<T> operator+(Dimension3D<T> self, T n)
				{
					self += n;
					return self;
				}

				friend const Dimension3D<T> operator-(Dimension3D<T> self, T n)
				{
					self -= n;
					return self;
				}

				friend const Dimension3D<T> operator*(Dimension3D<T> self, T n)
				{
					self *= n;
					return self;
				}

				friend const Dimension3D<T> operator/(Dimension3D<T> self, T n)
				{
					self /= n;
					return self;
				}

				friend const Dimension3D<T> operator%(Dimension3D<T> self, T n)
				{
					self %= n;
					return self;
				}

				friend const Dimension3D<T> operator^(Dimension3D<T> self, T n)
				{
					self ^= n;
					return self;
				}

				friend const Dimension3D<T> operator+(Dimension3D<T> self, const Dimension2D<T> &other)
				{
					self += other;
					return self;
				}

				friend const Dimension3D<T> operator-(Dimension3D<T> self, const Dimension2D<T> &other)
				{
					self -= other;
					return self;
				}

				friend const Dimension3D<T> operator*(Dimension3D<T> self, const Dimension2D<T> &other)
				{
					self *= other;
					return self;
				}

				friend const Dimension3D<T> operator/(Dimension3D<T> self, const Dimension2D<T> &other)
				{
					self /= other;
					return self;
				}

				friend const Dimension3D<T> operator%(Dimension3D<T> self, const Dimension2D<T> &other)
				{
					self %= other;
					return self;
				}

				friend const Dimension3D<T> operator^(Dimension3D<T> self, const Dimension2D<T> &other)
				{
					self ^= other;
					return self;
				}

				friend const Dimension3D<T> operator+(Dimension3D<T> self, const Dimension3D<T> &other)
				{
					self += other;
					return self;
				}

				friend const Dimension3D<T> operator-(Dimension3D<T> self, const Dimension3D<T> &other)
				{
					self -= other;
					return self;
				}

				friend const Dimension3D<T> operator*(Dimension3D<T> self, const Dimension3D<T> &other)
				{
					self *= other;
					return self;
				}

				friend const Dimension3D<T> operator/(Dimension3D<T> self, const Dimension3D<T> &other)
				{
					self /= other;
					return self;
				}

				friend const Dimension3D<T> operator%(Dimension3D<T> self, const Dimension3D<T> &other)
				{
					self %= other;
					return self;
				}

				friend const Dimension3D<T> operator^(Dimension3D<T> self, const Dimension3D<T> &other)
				{
					self ^= other;
					return self;
				}
			};
		}
	}

	typedef maths::geometry::Dimension2D<uint32_t> Dimension2D_u32;
	typedef maths::geometry::Dimension2D<int32_t> Dimension2D_i32;
	typedef maths::geometry::Dimension3D<uint32_t> Dimension3D_u32;
	typedef maths::geometry::Dimension3D<int32_t> Dimension3D_i32;
}

#endif //LAMBDACOMMON_DIMENSIONS_H