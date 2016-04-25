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

#ifndef GM_SPHERE_HPP__
#define GM_SPHERE_HPP__

template <typename T>
class CSphere
{
public:
	CVector<3,T> position;
	T radius;

	inline CSphere()
	{

	}

	inline CSphere(const CVector<3,T> &p_position, T p_radius)
	{
		setup(p_position, p_radius);
	}

	inline void setup(const CVector<3,T> &p_position, T p_radius)
	{
		position = p_position;
		radius = p_radius;
	}
};

#endif
