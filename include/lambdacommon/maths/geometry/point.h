/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_POINT_H
#define LAMBDACOMMON_POINT_H

#include "../../object.h"
#include <tuple>
#include <utility>

namespace lambdacommon
{
    using namespace std::rel_ops;

    /*!
     * Represents a point in a 1D coordinate system.
     * @tparam T Type of the coordinates
     */
    template<typename T>
    class Point1D : public Object, public Nullable
    {
    protected:
        T _x;

    public:
        explicit Point1D(T x) : _x(x)
        {}

        /*!
         * Gets the X coordinate.
         * @return The X coordinate.
         */
        T get_x() const
        {
            return _x;
        }

        /*!
         * Sets the X coordinate.
         * @param x The X coordinate.
         */
        void set_x(const T x)
        {
            _x = x;
        }

        bool is_null() const override
        {
            return this->_x == 0;
        }

        std::string to_string() const override
        {
            return std::move("{\"x\":" + std::to_string(_x) + '}');
        }

        bool operator==(const Point1D<T> &other) const
        {
            return this->_x == other._x;
        }

        bool operator<(const Point1D<T> &other) const
        {
            return this->_x < other._x;
        }

        template<std::size_t N>
        decltype(auto) get() const
        {
            if constexpr (N == 0) return this->_x;
        }
    };

    /*!
     * Represents a point in a 2D coordinate system.
     * @tparam T Type of the coordinates.
     */
    template<typename T>
    class Point2D : public Point1D<T>
    {
    protected:
        T _y;

    public:
        Point2D(T x, T y) : Point1D<T>(x), _y(y)
        {}

        /*!
         * Gets the Y coordinate.
         * @return The Y coordinate.
         */
        T get_y() const
        {
            return _y;
        }

        /*!
         * Sets the Y coordinate.
         * @param y The Y coordinate.
         */
        void set_y(const T y)
        {
            _y = y;
        }

        bool is_null() const override
        {
            return Point1D<T>::is_null() && this->_y == 0;
        }

        std::string to_string() const override
        {
            return std::move("{\"x\":" + std::to_string(this->_x) + ",\"y\":" + std::to_string(this->_y) + '}');
        }

        bool operator==(const Point2D<T> &other) const
        {
            return this->_x == other._x && this->_y == other._y;
        }

        bool operator<(const Point2D<T> &other) const
        {
            return std::tie(this->_x, this->_y) < std::tie(other._x, other._y);
        }

        template<std::size_t N>
        decltype(auto) get() const
        {
            if constexpr (N == 0) return this->_x;
            else if constexpr (N == 1) return this->_y;
        }
    };

    /*!
     * Represents a point in space.
     * @tparam T Type of the coordinates.
     */
    template<typename T>
    class Point3D : public Point2D<T>
    {
    protected:
        T _z;

    public:
        Point3D(T x, T y, T z = 0) : Point2D<T>(x, y), _z(z)
        {}

        /*!
         * Gets the Z coordinate.
         * @return The Z coordinate.
         */
        T get_z() const
        {
            return _z;
        }

        /*!
         * Sets the Z coordinate.
         * @param z The Z coordinate.
         */
        void set_z(const T z)
        {
            _z = z;
        }

        bool is_null() const override
        {
            return Point2D<T>::is_null() && this->_z == 0;
        }

        std::string to_string() const override
        {
            return std::move("{\"x\":" + std::to_string(this->_x) + ",\"y\":" + std::to_string(this->_y) + ",\"z\":" + std::to_string(this->_z) + '}');
        }

        bool operator==(const Point3D<T> &other) const
        {
            return this->_x == other._x && this->_y == other._y && this->_z == other._z;
        }

        bool operator<(const Point3D<T> &other) const
        {
            return std::tie(this->_x, this->_y, this->_z) < std::tie(other._x, other._y, other._z);
        }

        template<std::size_t N>
        decltype(auto) get() const
        {
            if constexpr (N == 0) return this->_x;
            else if constexpr (N == 1) return this->_y;
            else if constexpr (N == 2) return this->_z;
        }
    };

    template<typename T, typename I>
    Point1D<T> point1d_cast(const Point1D<I> &point_1d)
    {
        return Point1D<T>(static_cast<T>(point_1d.get_x()));
    }

    template<typename T, typename I>
    Point2D<T> point2d_cast(const Point2D<I> &point_2d)
    {
        return {static_cast<T>(point_2d.get_x()), static_cast<T>(point_2d.get_y())};
    }

    template<typename T, typename I>
    Point3D<T> point3d_cast(const Point3D<I> &point_3d)
    {
        return {static_cast<T>(point_3d.get_x()), static_cast<T>(point_3d.get_y()), static_cast<T>(point_3d.get_z())};
    }
}

// Structured bindings for lambdacommon::Point1D, lambdacommon::Point2D and lambdacommon::Point3D.
namespace std
{
    template<typename T>
    struct tuple_size<lambdacommon::Point1D<T>> : std::integral_constant<std::size_t, 1>
    {
    };

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(0, lambdacommon::Point1D<T>, T)

    template<typename T>
    struct tuple_size<lambdacommon::Point2D<T>> : std::integral_constant<std::size_t, 2>
    {
    };

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(0, lambdacommon::Point2D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(1, lambdacommon::Point2D<T>, T)

    template<typename T>
    struct tuple_size<lambdacommon::Point3D<T>> : std::integral_constant<std::size_t, 3>
    {
    };

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(0, lambdacommon::Point3D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(1, lambdacommon::Point3D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(2, lambdacommon::Point3D<T>, T)
}

#endif //LAMBDACOMMON_POINT_H
