
/*
 * Copyright 2010 Martin Schreiber
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * a small math lib with some useful functions and a c++ abstraction layer
 */
#ifndef CMATH_HPP
#define CMATH_HPP

#if defined(WIN32) || defined(_WIN32)
#include "CMathWin.hpp"
#else

#include <iostream>
#include <limits>

#include <cmath>

#include <stdlib.h>


/**
 * \brief	math handler to use same function names for different types
 */
template <typename T>
class CMath
{
public:
	/// return PI
	static inline T PI();

	/// return absolute value
	static inline T abs(T a);
	/// return power of value
	static inline T pow(T base, T exp);
	/// return floor value
	static inline T floor(T a);
	/// return ceil value
	static inline T ceil(T a);

	/// return ceiled value in binary system
	static inline T ceil2(T a);
	/// return sqrt value
	static inline T sqrt(T a);
	/// return rounded value
	static inline T round(T a);

	/// return digits of a in binary system
	static inline T digits2(T a);

	/// return exp value
	static inline T exp(T a);

	/// return sin value
	static inline T sin(T a);
	/// return cos value
	static inline T cos(T a);
	/// return tan value
	static inline T tan(T a);

	static inline T clamp(T a, T min, T max);

	/// return sin value
	static inline T asin(T a);
	/// return cos value
	static inline T acos(T a);
	/// return tan value
	static inline T atan(T a);

	/// return max of both values
	static inline T max(T a, T b)		{	return (a < b ? b : a);	}
	/// return min of both values
	static inline T min(T a, T b)		{	return (a > b ? b : a);	}

	/// return value of type T for string s
	static inline T aton(const char *s);

	/// return maximum available finite number
	static inline T max();
	/// return minimum available finite number (positive number closest to 0)
	static inline T min();
	/// return the value for infinity
	static inline T inf();

	/// return true if the value is nan
	static inline bool isNan(T a)		{	return a != a;	}

	/**
	 * greatest common divisor
	 * http://en.wikipedia.org/wiki/Euclidean_algorithm
	 */
	static inline T gcd(T a, T b)
	{
		if (a == 0)	return b;
		while (b != 0)
		{
			if (a > b)
				a -= b;
			else
				b -= a;
		}
		return a;
	}

	/// return the sign of a number (-1 or +1)
	static inline T	sign(T a)			{	return (a < (T)0 ? (T)-1 : (T)+1);	}
};


/** return PI */
template <>	inline float	CMath<float>::PI()
{
	return M_PI;
}

/** return PI of type double */
template <>	inline double	CMath<double>::PI()				{	return M_PI;		}

/** return absolute value of a */
template <>	inline float	CMath<float>::abs(float a)		{	return std::abs(a);	}
/** return absolute value of a */
template <>	inline double	CMath<double>::abs(double a)	{	return std::abs(a);	}
/** return absolute value of a */
template <>	inline int		CMath<int>::abs(int a)			{	return std::abs(a);	}

/** return the power of 'base' to 'exp' */
template <>	inline float	CMath<float>::pow(float base, float exp)	{	return std::pow(base, exp);	}
/** return the power of 'base' to 'exp' */
template <>	inline double	CMath<double>::pow(double base, double exp)	{	return std::pow(base, exp);	}
/** return the power of 'base' to 'exp' */
//template <>	inline int		CMath<int>::pow(int base, int exp)			{	return ::powl(base, exp);	}

/** return clamped value of a */
template <>	inline int	CMath<int>::clamp(int a, int min, int max)
		{	if (a < min)	return min;
			if (a > max)	return max;
			return a;
		}

/** return floored value of a */
template <>	inline float	CMath<float>::floor(float a)	{	return std::floor(a);	}
/** return floored value of a */
template <>	inline double	CMath<double>::floor(double a)	{	return std::floor((long double)a);	}
/** return floored value of a */
template <>	inline int		CMath<int>::floor(int a)		{	return a;			}

/** return ceiled value of a */
template <>	inline float	CMath<float>::ceil(float a)		{	return std::ceil(a);	}
/** return ceiled value of a */
template <>	inline double	CMath<double>::ceil(double a)	{	return std::ceil((long double)a);	}
/** return ceiled value of a */
template <>	inline int		CMath<int>::ceil(int a)			{	return a;			}


/** return maximum finite value of a */
template <>	inline float	CMath<float>::max()				{	return std::numeric_limits<float>::max();		}
/** return maximum finite value of a */
template <>	inline double	CMath<double>::max()			{	return std::numeric_limits<double>::max();		}
/** return maximum finite value of a */
template <>	inline int		CMath<int>::max()				{	return std::numeric_limits<int>::max();			}

/** return minimum available finite number (positive number closest to 0) */
template <>	inline float	CMath<float>::min()				{	return std::numeric_limits<float>::min();		}
/** return minimum available finite number (positive number closest to 0) */
template <>	inline double	CMath<double>::min()			{	return std::numeric_limits<double>::min();		}
/** return minimum available finite number (positive number closest to 0) */
template <>	inline int		CMath<int>::min()				{	return std::numeric_limits<int>::min();			}

/** return minimum available finite number (positive number closest to 0) */
template <>	inline float	CMath<float>::inf()				{	return std::numeric_limits<float>::infinity();		}
/** return minimum available finite number (positive number closest to 0) */
template <>	inline double	CMath<double>::inf()			{	return std::numeric_limits<double>::infinity();		}


/**
 * return ceiled value of a in binary system
 *
 * safety check only valid for unsigned data
 */
template <typename T>	inline T		CMath<T>::ceil2(T a)
		{
			if (a > ((T)1<<(sizeof(T)*8-2)))	return 0;

			T r = 1;
			while (r < a)
			{
				r <<= 1;
			}
			return r;
		}

/**
 * return digits of a in binary system
 *
 * a must not be larger than 2^30!!!
 */
template <>	inline int		CMath<int>::digits2(int a)
		{
			if (a > 0x40000000)	return 0;

			if (a == 0)		return 0;

			int r = 1;
			int c = 1;

			while (r < a)
			{
				r <<= 1;
				c++;
			}
			return c;
		}


/** return square root of a */
template <>	inline float	CMath<float>::sqrt(float a)		{	return std::sqrt(a);	}
/** return square root of a */
template <>	inline double	CMath<double>::sqrt(double a)	{	return std::sqrt((long double)a);	}
/** return square root of a */
template <>	inline int		CMath<int>::sqrt(int a)			{	return std::sqrt((unsigned long)a);	}


/** return rounded value of a */
template <>	inline float	CMath<float>::round(float a)	{	return std::floor(a+0.5f);	}
/** return rounded value of a */
template <>	inline double	CMath<double>::round(double a)	{	return std::floor((long double)(a+0.5));	}
/** return rounded value of a */
template <>	inline int		CMath<int>::round(int a)		{	return a;			}

/** convert string to type float */
template <>	inline float	CMath<float>::aton(const char *s)	{	return ::atof(s);	}
/** convert string to type double */
template <>	inline double	CMath<double>::aton(const char *s)	{	return ::atof(s);	}
/** convert string to type int */
template <>	inline int		CMath<int>::aton(const char *s)		{	return ::atoi(s);	}

/** sinus of s */
template <>	inline float	CMath<float>::sin(float s)	{	return std::sin(s);	}
/** cosinus of s */
template <>	inline float	CMath<float>::cos(float s)	{	return std::cos(s);	}
/** tangens of s */
template <>	inline float	CMath<float>::tan(float s)	{	return std::tan(s);	}

/** inverse sinus of s */
template <>	inline float	CMath<float>::asin(float s)	{	return std::asin(s);	}
/** inverse cosinus of s */
template <>	inline float	CMath<float>::acos(float s)	{	return std::acos(s);	}
/** inverse tangens of s */
template <>	inline float	CMath<float>::atan(float s)	{	return std::atan(s);	}

/** exponential of s */
template <>	inline float	CMath<float>::exp(float s)	{	return std::exp(s);	}


template <typename T>
::std::ostream&
operator<<(::std::ostream& os, const CMath<T> &lm)
{
	return os << lm.value;
}


#endif
#endif
