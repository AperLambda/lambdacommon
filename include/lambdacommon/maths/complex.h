/*
 * Copyright © 2019 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_COMPLEX_H
#define LAMBDACOMMON_COMPLEX_H

#include "../object.h"
#include "../maths.h"
#include <sstream>
#include <ostream>
#include <tuple>
#include <utility>
#include <cmath>

namespace lambdacommon
{
	using namespace std::rel_ops;
	
	template<typename T>
	class ComplexNumber : public Object
	{
	private:
		T real;
		T imaginary;

	public:
		ComplexNumber(T real, T imaginary) : real(real), imaginary(imaginary)
		{}

		/*!
 		 * Gets the real part of the complex number.
 		 * @return The real part.
 		 */
		T get_real() const
		{
			return this->real;
		}

		/*!
		 * Sets the real part of the complex number.
		 * @param real The real part.
		 */
		void set_real(T real)
		{
			this->real = real;
		}

		/*!
 		 * Gets the imaginary part of the complex number.
 		 * @return The imaginary part.
 		 */
		T get_imaginary() const
		{
			return this->imaginary;
		}

		/*!
		 * Sets the imaginary part of the complex number.
		 * @param imaginary The imaginary part.
		 */
		void set_imaginary(T imaginary)
		{
			this->imaginary = imaginary;
		}

		/*!
		 * Gets the modulus of the complex number.
		 * @return The modulus.
		 */
		T get_modulus() const
		{
			return static_cast<T>(sqrt(pow(this->real, 2) + pow(this->imaginary, 2)));
		}

		std::string to_string() const override
		{
			std::stringstream result;
			if (this->real != 0) result << std::to_string(this->real);
			if (this->imaginary != 0)
			{
				if (this->imaginary > 0) result << '+' << std::to_string(this->imaginary) << 'i';
				else result << "-" <<  std::to_string(maths::abs(this->imaginary)) << 'i';
			}
			return result.str();
		}

		bool operator==(const ComplexNumber<T> &other) const
		{
			return this->real == other.real && this->imaginary == other.imaginary;
		}

		bool operator<(const ComplexNumber<T> &other) const
		{
			return std::tie(this->real, this->imaginary) < std::tie(other.real, other.imaginary);
		}

		ComplexNumber<T> &operator+=(ComplexNumber<T> other)
		{
			this->real += other.real;
			this->imaginary += other.imaginary;
			return *this;
		}

		ComplexNumber<T> &operator-=(ComplexNumber<T> other)
		{
			this->real -= other.real;
			this->imaginary -= other.imaginary;
			return *this;
		}

		friend const ComplexNumber<T> operator+(ComplexNumber<T> self, ComplexNumber<T> other)
		{
			self += other;
			return self;
		}

		friend const ComplexNumber<T> operator-(ComplexNumber<T> self, ComplexNumber<T> other)
		{
			self -= other;
			return self;
		}

		template<std::size_t N>
		decltype(auto) get() const
		{
			if constexpr (N == 0) return this->real;
			else if constexpr (N == 1) return this->imaginary;
		}
	};
}

#endif //LAMBDACOMMON_COMPLEX_H
