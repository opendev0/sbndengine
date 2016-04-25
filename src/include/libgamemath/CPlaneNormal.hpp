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

#ifndef GM_CPLANENORMAL_HPP__
#define GM_CPLANENORMAL_HPP__

template <typename T>
class CPlaneNormal
{
public:
	CVector<3,T> normal;
	T d;

	inline CPlaneNormal(const CVector<3,T> &p0, const CVector<3,T> &p1, const CVector<3,T> &p2)
	{
		setup(p0, p1, p2);
	}

	/*
	 * setup a normalized plane
	 */
	inline void setup(const CVector<3,T> &p0, const CVector<3,T> &p1, const CVector<3,T> &p2)
	{
		CVector<3,T> d0, d1;
		d0 = p1-p0;
		d1 = p2-p0;
		normal = (p2-p0)%(p1-p0);

		// create normalized plane
		normal.normalize();
		d = normal.dotProd(p0);
	}
};


template <class T>
inline
::std::ostream&
operator<<(::std::ostream &co, const CPlaneNormal<T> &p)
{
	return co << "[" << p.normal.data[0] << ", " << p.normal.data[1] << ", " << p.normal.data[2] << " |" << p.d << "]";
}

#endif
