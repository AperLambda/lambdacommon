/*
 * Copyright © 2018 AperLambda <aperlambda@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_MATHS_H
#define LAMBDACOMMON_MATHS_H

#include "lambdacommon.h"

namespace lambdacommon
{
	namespace maths
	{
		/*!
		 * Calculates the absolute value of a number (effectively removes any negative sign).
		 * @tparam N The number type.
		 * @param number The number to get the absolute value of.
		 * @return The absolute value.
		 */
		template<typename N>
		N abs(N number)
		{
			return number < 0 ? -number : number;
		}

		/*!
		 * Returns the smallest value of all arguments.
		 * @tparam N The number type.
		 * @param a One of the numbers to get the smallest from.
		 * @param b One of the numbers to get the smallest from.
		 * @return The smallest number.
		 */
		template<typename N>
		N min(N a, N b)
		{
			return a < b ? a : b;
		}

		/*!
		 * Returns the smallest value of all arguments.
		 * @tparam N The number type.
		 * @param numbers Numbers to get the smallest from.
		 * @return The smallest number.
		 */
		template<typename N>
		N min(const std::initializer_list<N> &numbers)
		{
			if (numbers.size() == 0)
				return 0;

			bool first = true;
			N minN;
			for (auto i : numbers)
				if (first)
				{
					minN = i;
					first = false;
				}
				else
					minN = min(minN, i);
			return minN;
		}

		/*!
		 * Returns the largest value of all arguments.
		 * @tparam N The number type.
		 * @param a One of the numbers to get the largest from.
		 * @param b One of the numbers to get the largest from.
		 * @return The largest number.
		 */
		template<typename N>
		N max(N a, N b)
		{
			return a > b ? a : b;
		}

		/*!
		 * Returns the largest value of all arguments.
		 * @tparam N The number type.
		 * @param numbers Numbers to get the largest from.
		 * @return The largest number.
		 */
		template<typename N>
		N max(const std::initializer_list<N> &numbers)
		{
			if (numbers.size() == 0)
				return 0;

			bool first = true;
			N maxN;
			for (auto i : numbers)
				if (first)
				{
					maxN = i;
					first = false;
				}
				else
					maxN = max(maxN, i);
			return maxN;
		}

		/*!
		 * Clamps a number between a minimum and maximum value.
		 * @tparam N The number type.
		 * @param number The number to clamp.
		 * @param min The minimum value, this function will never return a number less than this.
		 * @param max The maximum value, this function will never return a number greater than this.
		 * @return The clamped value.
		 */
		template<typename N>
		N clamp(N number, N min, N max)
		{
			return maths::min(maths::max(number, min), max);
		}
	}
}

#endif //LAMBDACOMMON_MATHS_H