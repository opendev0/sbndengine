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

#ifndef GM_CAABOXORIGIN_HPP__
#define GM_CAABOXORIGIN_HPP__

/**
 * axis aligned box with center at origin
 */
template <typename T>
class CBoxAAOrigin
{
public:
	CVector<3,T>	size;
	CVector<3,T>	half_size;

	CBoxAAOrigin(
			const CVector<3,T> &p_size
			)
	{
		size = p_size;
		half_size = size*0.5;
	}
};


template <class T>
inline
::std::ostream&
operator<<(::std::ostream &co, const CBoxAAOrigin<T> &b)
{
	return co << b.size;
}


#endif
