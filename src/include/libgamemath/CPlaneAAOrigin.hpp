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

#ifndef GM_CAAPLANEORIGIN_HPP__
#define GM_CAAPLANEORIGIN_HPP__

/**
 * axis aligned plane with limitations in x and z direction and
 * the center at origin
 */
template <typename T>
class CPlaneAAOrigin
{
public:
	T size_x, size_z;
	T half_size_x, half_size_z;

	CPlaneAAOrigin(
			T p_size_x,
			T p_size_z
			)
	{
		size_x = p_size_x;
		half_size_x = size_x*0.5f;
		size_z = p_size_z;
		half_size_z = size_z*0.5f;
	}
};


template <class T>
inline
::std::ostream&
operator<<(::std::ostream &co, const CPlaneAAOrigin<T> &p)
{
	return co << p.size;
}


#endif
