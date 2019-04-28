/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_VECTOR_H
#define LAMBDACOMMON_VECTOR_H

#include "point.h"
#include "../../maths.h"
#include <cmath>

namespace lambdacommon
{
    using namespace std::rel_ops;

    /*!
     * Represents a vector in a 1D coordinate system.
     * @tparam T Type of the coordinates
     */
    template<typename T>
    class Vector1D : public Object, public Nullable
    {
    protected:
        T x;

    public:
        explicit Vector1D(T x) : x(x) {}

        /*!
         * Gets the X coordinate.
         * @return The X coordinate.
         */
        T get_x() const {
            return x;
        }

        /*!
         * Sets the X coordinate.
         * @param x The X coordinate.
         */
        void set_x(T x) {
            this->x = x;
        }

        /*!
         * Gets the standard of the vector.
         * @return The standard of the vector.
         */
        virtual T get_standard() const {
            return maths::abs(this->x);
        }

        bool is_null() const override {
            return this->x == 0;
        }

        std::string to_string() const override {
            return std::move('(' + std::to_string(this->x) + ')');
        }

        bool operator==(const Vector1D<T>& other) const {
            return this->x == other.x;
        }

        bool operator<(const Vector1D<T>& other) const {
            return this->x < other.x;
        }

        Vector1D<T>& operator+=(Vector1D<T> vec) {
            this->x += vec.x;
            return *this;
        }

        Vector1D<T>& operator-=(Vector1D<T> vec) {
            this->x -= vec.x;
            return *this;
        }

        friend const Vector1D<T> operator+(Vector1D<T> self, Vector1D<T> vec) {
            self += vec;
            return self;
        }

        friend const Vector1D<T> operator-(Vector1D<T> self, Vector1D<T> vec) {
            self -= vec;
            return self;
        }

        template<std::size_t N>
        decltype(auto) get() const {
            if constexpr (N == 0) return this->x;
        }
    };

    /*!
     * Represents a vector in a 2D coordinate system.
     * @tparam T Type of the coordinates
     */
    template<typename T>
    class Vector2D : public Vector1D<T>
    {
    protected:
        T y;

    public:
        Vector2D(T x, T y) : Vector1D<T>(x), y(y) {}

        /*!
         * Gets the Y coordinate.
         * @return The Y coordinate.
         */
        T get_y() const {
            return y;
        }

        /*!
         * Sets the Y coordinate.
         * @param y The Y coordinate.
         */
        void set_y(T y) {
            this->y = y;
        }

        T get_standard() const override {
            return static_cast<T>(sqrt(pow(this->x, 2) + pow(this->y, 2)));
        }

        bool is_null() const override {
            return Vector1D<T>::is_null() && y == 0;
        }

        std::string to_string() const override {
            return '(' + std::to_string(this->x) + ';' + std::to_string(this->y) + ')';
        }

        bool operator==(const Vector2D<T>& other) const {
            return this->x == other.x && this->y == other.y;
        }

        bool operator<(const Vector2D<T>& other) const {
            return std::tie(this->x, this->y) < std::tie(other.x, other.y);
        }

        Vector2D<T>& operator+=(Vector2D<T> vec) {
            this->x += vec.x;
            this->y += vec.y;
            return *this;
        }

        Vector2D<T>& operator-=(Vector2D<T> vec) {
            this->x -= vec.x;
            this->y -= vec.y;
            return *this;
        }

        friend const Vector2D<T> operator+(Vector2D<T> self, Vector2D<T> vec) {
            self += vec;
            return self;
        }

        friend const Vector2D<T> operator-(Vector2D<T> self, Vector2D<T> vec) {
            self -= vec;
            return self;
        }

        template<std::size_t N>
        decltype(auto) get() const {
            if constexpr (N == 0) return this->x;
            else if constexpr (N == 1) return this->y;
        }
    };

    /*!
     * Represents a vector in space.
     * @tparam T Type of the coordinates
     */
    template<typename T>
    class Vector3D : public Vector2D<T>
    {
    private:
        T z;

    public:
        Vector3D(T x, T y, T z) : Vector2D<T>(x, y), z(z) {}

        /*!
         * Gets the Z coordinate.
         * @return The Z coordinate.
         */
        T get_z() const {
            return z;
        }

        /*!
         * Sets the Z coordinate.
         * @param y The Z coordinate.
         */
        void set_z(T z) {
            this->z = z;
        }

        /*!
         * Gets the cross product between this and another vector.
         * @param v The other vector.
         * @return The cross product.
         */
        Vector3D<T> cross_product(const Vector3D<T> v) {
            return (*this) * v;
        }

        T get_standard() const override {
            return static_cast<T>(sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2)));
        }

        bool is_null() const override {
            return Vector2D<T>::is_null() && this->z == 0;
        }

        std::string to_string() const override {
            return '(' + std::to_string(this->x) + ';' + std::to_string(this->y) + ';' + std::to_string(this->z) + ')';
        }

        bool operator==(const Vector3D<T>& other) const {
            return this->x == other.x && this->y == other.y && this->z == other.z;
        }

        bool operator<(const Vector3D<T>& other) const {
            return std::tie(this->x, this->y, this->z) < std::tie(other.x, other.y, other.z);
        }

        Vector3D<T>& operator+=(Vector3D<T> vec) {
            this->x += vec.x;
            this->y += vec.y;
            this->z += vec.z;
            return *this;
        }

        Vector3D<T>& operator-=(Vector3D<T> vec) {
            this->x -= vec.x;
            this->y -= vec.y;
            this->z -= vec.z;
            return *this;
        }

        Vector3D<T>& operator*=(Vector3D<T> vec) {
            this->x = this->y * vec.z - this->z * vec.y;
            this->y = this->z * vec.x - this->x * vec.z;
            this->z = this->x * vec.y - this->y * vec.x;
            return *this;
        }

        friend const Vector3D<T> operator+(Vector3D<T> self, Vector3D<T> vec) {
            self += vec;
            return self;
        }

        friend const Vector3D<T> operator-(Vector3D<T> self, Vector3D<T> vec) {
            self -= vec;
            return self;
        }

        friend const Vector3D<T> operator*(Vector3D<T> self, Vector3D<T> vec) {
            self *= vec;
            return self;
        }

        template<std::size_t N>
        decltype(auto) get() const {
            if constexpr (N == 0) return this->x;
            else if constexpr (N == 1) return this->y;
            else if constexpr (N == 2) return this->z;
        }
    };

    /*!
     * Casts the given vector's values to the specified type.
     * @tparam T The wanted output type.
     * @tparam I The input type.
     * @param vector_1d The vector to cast.
     * @return A new casted vector.
     */
    template<typename T, typename I>
    constexpr Vector1D<T> vector1d_cast(const Vector1D<I>& vector_1d) {
        return Vector1D<T>(static_cast<T>(vector_1d.get_x()));
    }

    /*!
     * Casts the given vector's values to the specified type.
     * @tparam T The wanted output type.
     * @tparam I The input type.
     * @param vector_2d The vector to cast.
     * @return A new casted vector.
     */
    template<typename T, typename I>
    constexpr Vector2D<T> vector2d_cast(const Vector2D<I>& vector_2d) {
        return Vector2D<T>(static_cast<T>(vector_2d.get_x()), static_cast<T>(vector_2d.get_y()));
    }

    /*!
     * Casts the given vector's values to the specified type.
     * @tparam T The wanted output type.
     * @tparam I The input type.
     * @param vector_3d The vector to cast.
     * @return A new casted vector.
     */
    template<typename T, typename I>
    constexpr Vector3D<T> vector3d_cast(const Vector3D<I>& vector_3d) {
        return Vector3D<T>(static_cast<T>(vector_3d.get_x()), static_cast<T>(vector_3d.get_y()), static_cast<T>(vector_3d.get_z()));
    }

    namespace Vector
    {
        /*!
         * Creates a new vector from two given points.
         * @tparam T The type of the coordinates.
         * @param a The first point.
         * @param b The second point.
         * @return A new vector defined by the two given points.
         */
        template<typename T>
        Vector1D<T> from_points(const Point1D <T>& a, const Point1D <T>& b) {
            return Vector1D<T>(b.get_x() - a.get_x());
        }

        /*!
         * Creates a new vector from two given points.
         * @tparam T The type of the coordinates.
         * @param a The first point.
         * @param b The second point.
         * @return A new vector defined by the two given points.
         */
        template<typename T>
        Vector2D<T> from_points(const Point2D <T>& a, const Point2D <T>& b) {
            return {b.get_x() - a.get_x(), b.get_y() - a.get_y()};
        }

        /*!
         * Creates a new vector from two given points in space.
         * @tparam T The type of the coordinates.
         * @param a The first point in space.
         * @param b The second point in space.
         * @return A new vector in space defined by the two given points.
         */
        template<typename T>
        Vector3D<T> from_points(const Point3D <T>& a, const Point3D <T>& b) {
            return {b.get_x() - a.get_x(), b.get_y() - a.get_y(), b.get_z() - a.get_z()};
        }

        /*!
         * Gets the scalar product between two vectors.
         * @tparam T The type of the values.
         * @param u A vector in 1D.
         * @param v A vector in 1D.
         * @return The scalar product.
         */
        template<typename T>
        T scalar_product(const Vector1D<T>& u, const Vector1D<T>& v) {
            return u.get_x() * v.get_x();
        }

        /*!
         * Gets the scalar product between two vectors.
         * @tparam T The type of the values.
         * @param u A vector in 2D.
         * @param v A vector in 2D.
         * @return The scalar product.
         */
        template<typename T>
        T scalar_product(const Vector2D<T>& u, const Vector2D<T>& v) {
            return u.get_x() * v.get_x() + u.get_y() * v.get_y();
        }

        /*!
         * Gets the scalar product between two vectors in space.
         * @tparam T The type of the values.
         * @param u A vector in space.
         * @param v A vector in space.
         * @return The scalar product.
         */
        template<typename T>
        T scalar_product(const Vector3D<T>& u, const Vector3D<T>& v) {
            return u.get_x() * v.get_x() + u.get_y() * v.get_y() + u.get_z() * v.get_z();
        }
    }
}

// Structured bindings for lambdacommon::Vector1D, lambdacommon::Vector2D and lambdacommon::Vector3D.
namespace std
{
    template<typename T>
    struct tuple_size<lambdacommon::Vector1D<T>> : std::integral_constant<std::size_t, 1>
    {
    };

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(0, lambdacommon::Vector1D<T>, T)

    template<typename T>
    struct tuple_size<lambdacommon::Vector2D<T>> : std::integral_constant<std::size_t, 2>
    {
    };

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(0, lambdacommon::Vector2D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(1, lambdacommon::Vector2D<T>, T)

    template<typename T>
    struct tuple_size<lambdacommon::Vector3D<T>> : std::integral_constant<std::size_t, 3>
    {
    };

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(0, lambdacommon::Vector3D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(1, lambdacommon::Vector3D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(2, lambdacommon::Vector3D<T>, T)
}

#endif //LAMBDACOMMON_VECTOR_H
