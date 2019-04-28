/*
 * Copyright © 2019 LambdAurora <aurora42lambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_DIMENSIONS_H
#define LAMBDACOMMON_DIMENSIONS_H

#include "object.h"
#include <utility>

namespace lambdacommon
{
    using namespace std::rel_ops;

    /*!
     * Represents a size in 2 dimensions.
     * @tparam T Type of the values.
     */
    template<typename T>
    class Size2D : public Object, public Nullable
    {
    protected:
        /*!
         * The X coordinate.
         */
        T _width;
        /*!
         * The Y coordinate.
         */
        T _height;

    public:
        Size2D() : Size2D<T>(0, 0) {}

        Size2D(T width, T height) : _width(width), _height(height) {}

        /*!
         * Gets the width of the size.
         * @return The width.
         */
        T get_width() const {
            return _width;
        }

        /*!
         * Sets the width of the size.
         * @param width The width.
         */
        void set_width(T width) {
            Size2D::_width = width;
        }

        /*!
         * Gets the height of the size.
         * @return The height.
         */
        T get_height() const {
            return _height;
        }

        /*!
         * Sets the height of the size.
         * @param height The height.
         */
        void set_height(T height) {
            Size2D::_height = height;
        }

        /*!
         * Gets the size as a surface.
         * @return The surface.
         */
        T get_surface() const {
            return get_width() * get_height();
        }

        bool is_null() const override {
            return _width == 0 && _height == 0;
        }

        std::string to_string() const override {
            return R"({"width":)" + std::to_string(_width) + R"(,"height":)" + std::to_string(_height) + "}";
        }

        bool operator==(const Size2D<T>& other) const {
            return this->_width == other._width && this->_height == other._height;
        }

        bool operator<(const Size2D<T>& other) const {
            return this->get_surface() < other.get_surface();
        }

        template<std::size_t N>
        decltype(auto) get() const {
            if constexpr (N == 0) return this->_width;
            else if constexpr (N == 1) return this->_height;
        }

        virtual Size2D<T>& operator+=(T n) {
            this->_width += n;
            this->_height += n;
            return *this;
        }

        virtual Size2D<T>& operator-=(T n) {
            this->_width -= n;
            this->_height -= n;
            return *this;
        }

        virtual Size2D<T>& operator*=(T n) {
            this->_width *= n;
            this->_height *= n;
            return *this;
        }

        virtual Size2D<T>& operator/=(T n) {
            this->_width /= n;
            this->_height /= n;
            return *this;
        }

        virtual Size2D<T>& operator%=(T n) {
            this->_width %= n;
            this->_height %= n;
            return *this;
        }

        virtual Size2D<T>& operator^=(T n) {
            this->_width ^= n;
            this->_height ^= n;
            return *this;
        }

        virtual Size2D<T>& operator+=(const Size2D<T>& other) {
            this->_width += other._width;
            this->_height += other._height;
            return *this;
        }

        virtual Size2D<T>& operator-=(const Size2D<T>& other) {
            this->_width -= other._width;
            this->_height -= other._height;
            return *this;
        }

        virtual Size2D<T>& operator*=(const Size2D<T>& other) {
            this->_width *= other._width;
            this->_height *= other._height;
            return *this;
        }

        virtual Size2D<T>& operator/=(const Size2D<T>& other) {
            this->_width /= other._width;
            this->_height /= other._height;
            return *this;
        }

        virtual Size2D<T>& operator%=(const Size2D<T>& other) {
            this->_width %= other._width;
            this->_height %= other._height;
            return *this;
        }

        virtual Size2D<T>& operator^=(const Size2D<T>& other) {
            this->_width ^= other._width;
            this->_height ^= other._height;
            return *this;
        }

        friend const Size2D<T> operator+(Size2D<T> self, T n) {
            self += n;
            return self;
        }

        friend const Size2D<T> operator-(Size2D<T> self, T n) {
            self -= n;
            return self;
        }

        friend const Size2D<T> operator*(Size2D<T> self, T n) {
            self *= n;
            return self;
        }

        friend const Size2D<T> operator/(Size2D<T> self, T n) {
            self /= n;
            return self;
        }

        friend const Size2D<T> operator%(Size2D<T> self, T n) {
            self %= n;
            return self;
        }

        friend const Size2D<T> operator^(Size2D<T> self, T n) {
            self ^= n;
            return self;
        }

        friend const Size2D<T> operator+(Size2D<T> self, const Size2D<T>& other) {
            self += other;
            return self;
        }

        friend const Size2D<T> operator-(Size2D<T> self, const Size2D<T>& other) {
            self -= other;
            return self;
        }

        friend const Size2D<T> operator*(Size2D<T> self, const Size2D<T>& other) {
            self *= other;
            return self;
        }

        friend const Size2D<T> operator/(Size2D<T> self, const Size2D<T>& other) {
            self /= other;
            return self;
        }

        friend const Size2D<T> operator%(Size2D<T> self, const Size2D<T>& other) {
            self %= other;
            return self;
        }

        friend const Size2D<T> operator^(Size2D<T> self, const Size2D<T>& other) {
            self ^= other;
            return self;
        }
    };

    /*!
     * Represents a size in 3 dimensions.
     * @tparam T Type of the values.
     */
    template<typename T>
    class Size3D : public Size2D<T>
    {
    protected:
        /*!
         * The Z coordinate.
         */
        T _depth;

    public:
        Size3D() : Size3D<T>(0, 0) {}

        Size3D(T width, T height, T depth = 0) : Size2D<T>(width, height), _depth(depth) {}

        /*!
         * Gets the depth of the size.
         * @return The depth.
         */
        T get_depth() const {
            return _depth;
        }

        /*!
         * Sets the depth of the size.
         * @param depth The depth.
         */
        void set_depth(T depth) {
            Size3D::_depth = depth;
        }

        /*!
         * Transforms the 3D size into a 2D size.
         * @return The 2D size from a 3D size.
         */
        Size2D<T> to_2d() const {
            return {this->get_width(), this->get_height()};
        }

        /*!
         * Gets the volume of the size.
         * @return The volume.
         */
        T get_volume() const {
            return this->get_surface() * get_depth();
        }

        bool is_null() const override {
            return Size2D<T>::is_null() && _depth == 0;
        }

        std::string to_string() const override {
            return R"({"width":)" + std::to_string(this->get_width()) + R"(,"height":)" + std::to_string(this->get_height()) + R"(,"depth":)" + std::to_string(_depth) + "}";
        }

        bool operator==(const Size3D<T>& other) const {
            return this->_width == other._width && this->_height == other._height && this->_depth == other._depth;
        }

        bool operator<(const Size3D<T>& other) const {
            return this->get_volume() < other.get_volume();
        }

        template<std::size_t N>
        decltype(auto) get() const {
            if constexpr (N == 0) return this->_width;
            else if constexpr (N == 1) return this->_height;
            else if constexpr (N == 2) return this->_depth;
        }

        Size3D<T>& operator+=(T n) override {
            this->_width += n;
            this->_height += n;
            this->_depth += n;
            return *this;
        }

        Size3D<T>& operator-=(T n) override {
            this->_width -= n;
            this->_height -= n;
            this->_depth -= n;
            return *this;
        }

        Size3D<T>& operator*=(T n) override {
            this->_width *= n;
            this->_height *= n;
            this->_depth *= n;
            return *this;
        }

        Size3D<T>& operator/=(T n) override {
            this->_width /= n;
            this->_height /= n;
            this->_depth /= n;
            return *this;
        }

        Size3D<T>& operator%=(T n) override {
            this->_width %= n;
            this->_height %= n;
            this->_depth %= n;
            return *this;
        }

        Size3D<T>& operator^=(T n) override {
            this->_width ^= n;
            this->_height ^= n;
            this->_depth ^= n;
            return *this;
        }

        Size3D<T>& operator+=(const Size2D<T>& other) override {
            this->_width += other.get_width();
            this->_height += other.get_height();
            return *this;
        }

        Size3D<T>& operator-=(const Size2D<T>& other) override {
            this->_width -= other.get_width();
            this->_height -= other.get_height();
            return *this;
        }

        Size3D<T>& operator*=(const Size2D<T>& other) override {
            this->_width *= other.get_width();
            this->_height *= other.get_height();
            return *this;
        }

        Size3D<T>& operator/=(const Size2D<T>& other) override {
            this->_width /= other.get_width();
            this->_height /= other.get_height();
            return *this;
        }

        Size3D<T>& operator%=(const Size2D<T>& other) override {
            this->_width %= other.get_width();
            this->_height %= other.get_height();
            return *this;
        }

        Size3D<T>& operator^=(const Size2D<T>& other) override {
            this->_width ^= other.get_width();
            this->_height ^= other.get_height();
            return *this;
        }

        Size3D<T>& operator+=(const Size3D<T>& other) {
            this->_width += other._width;
            this->_height += other._height;
            this->_depth += other._depth;
            return *this;
        }

        Size3D<T>& operator-=(const Size3D<T>& other) {
            this->_width -= other._width;
            this->_height -= other._height;
            this->_depth -= other._depth;
            return *this;
        }

        Size3D<T>& operator*=(const Size3D<T>& other) {
            this->_width *= other._width;
            this->_height *= other._height;
            this->_depth *= other._depth;
            return *this;
        }

        Size3D<T>& operator/=(const Size3D<T>& other) {
            this->_width /= other._width;
            this->_height /= other._height;
            this->_depth /= other._depth;
            return *this;
        }

        Size3D<T>& operator%=(const Size3D<T>& other) {
            this->_width %= other._width;
            this->_height %= other._height;
            this->_depth %= other._depth;
            return *this;
        }

        Size3D<T>& operator^=(const Size3D<T>& other) {
            this->_width ^= other._width;
            this->_height ^= other._height;
            this->_depth ^= other._depth;
            return *this;
        }

        friend const Size3D<T> operator+(Size3D<T> self, T n) {
            self += n;
            return self;
        }

        friend const Size3D<T> operator-(Size3D<T> self, T n) {
            self -= n;
            return self;
        }

        friend const Size3D<T> operator*(Size3D<T> self, T n) {
            self *= n;
            return self;
        }

        friend const Size3D<T> operator/(Size3D<T> self, T n) {
            self /= n;
            return self;
        }

        friend const Size3D<T> operator%(Size3D<T> self, T n) {
            self %= n;
            return self;
        }

        friend const Size3D<T> operator^(Size3D<T> self, T n) {
            self ^= n;
            return self;
        }

        friend const Size3D<T> operator+(Size3D<T> self, const Size2D<T>& other) {
            self += other;
            return self;
        }

        friend const Size3D<T> operator-(Size3D<T> self, const Size2D<T>& other) {
            self -= other;
            return self;
        }

        friend const Size3D<T> operator*(Size3D<T> self, const Size2D<T>& other) {
            self *= other;
            return self;
        }

        friend const Size3D<T> operator/(Size3D<T> self, const Size2D<T>& other) {
            self /= other;
            return self;
        }

        friend const Size3D<T> operator%(Size3D<T> self, const Size2D<T>& other) {
            self %= other;
            return self;
        }

        friend const Size3D<T> operator^(Size3D<T> self, const Size2D<T>& other) {
            self ^= other;
            return self;
        }

        friend const Size3D<T> operator+(Size3D<T> self, const Size3D<T>& other) {
            self += other;
            return self;
        }

        friend const Size3D<T> operator-(Size3D<T> self, const Size3D<T>& other) {
            self -= other;
            return self;
        }

        friend const Size3D<T> operator*(Size3D<T> self, const Size3D<T>& other) {
            self *= other;
            return self;
        }

        friend const Size3D<T> operator/(Size3D<T> self, const Size3D<T>& other) {
            self /= other;
            return self;
        }

        friend const Size3D<T> operator%(Size3D<T> self, const Size3D<T>& other) {
            self %= other;
            return self;
        }

        friend const Size3D<T> operator^(Size3D<T> self, const Size3D<T>& other) {
            self ^= other;
            return self;
        }
    };
}

// Structured bindings for lambdacommon::Size2D and lambdacommon::Size3D.
namespace std
{
    template<typename T>
    struct tuple_size<lambdacommon::Size2D<T>> : std::integral_constant<std::size_t, 2>
    {
    };

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(0, lambdacommon::Size2D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(1, lambdacommon::Size2D<T>, T)

    template<typename T>
    struct tuple_size<lambdacommon::Size3D<T>> : std::integral_constant<std::size_t, 3>
    {
    };

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(0, lambdacommon::Size3D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(1, lambdacommon::Size3D<T>, T)

    template<typename T>
    LCOMMON_DEFINE_TUPLE_ELEMENT(2, lambdacommon::Size3D<T>, T)
}

#endif //LAMBDACOMMON_DIMENSIONS_H
