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

#ifndef GM_CLINEPP_HPP__
#define GM_CLINEPP_HPP__

/**
 * line specified by 2 points (PP)
 */
template <typename T>
class CLinePP
{
public:
	CVector<3,T> v0;
	CVector<3,T> v1;

	inline CLinePP(const CVector<3,T> &p_v0, const CVector<3,T> &p_v1)
	{
		setup(p_v0, p_v1);
	}

	/*
	 * setup a normalized plane
	 */
	inline void setup(const CVector<3,T> &p_v0, const CVector<3,T> &p_v1)
	{
		v0 = p_v0;
		v1 = p_v1;
	}
};


template <class T>
inline
::std::ostream&
operator<<(::std::ostream &co, const CLinePP<T> &p)
{
	return co << "[" << p.v0 << ", " << p.v1 << "]";
}

#endif
