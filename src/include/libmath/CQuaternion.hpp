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

#ifndef __CQUATERNION_HPP__
#define __CQUATERNION_HPP__

#include "worksheets_precompiler.hpp"

/**
 * implementation of a quaternion for rotations without gimbal lock
 */
template<typename T>
class CQuaternion
{
public:
	T i, j, k, w;

	CQuaternion()
	{
		reset();
	}


	/**
	 * constructor
	 *
	 * \param axix	the axis of the initial rotation
	 * \param angle	the angle of rotation around axis
	 */
	CQuaternion(const CVector<3,T> &axis, T angle)
	{
		reset();
		rotatePost(axis, angle);
	}


	/**
	 * constructor
	 *
	 * \param angular_rotation	this parameter specifies the axis of rotation
	 * and also the amount of rotation by its length
	 */
	CQuaternion(const CVector<3,T> &angular_rotation)
	{
		float length = angular_rotation.getLength();
		if (length <= 0)
		{
			reset();
			return;
		}

		setRotation(angular_rotation * (1.0/length), length);
	}


	CQuaternion(T p_i, T p_j, T p_k, T p_w)
	{
		i = p_i;
		j = p_j;
		k = p_k;

		w = p_w;
	}

	void applyAngularRotation(const CVector<3,T> &angular_rotation)
	{
#if 0
		// not working for unknown reasons
		CQuaternion n = (*this)*CQuaternion(angular_rotation.data[0], angular_rotation.data[1], angular_rotation.data[2], 0);

		i += n.i*0.5;
		j += n.j*0.5;
		k += n.k*0.5;
		w += n.w*0.5;

		normalize();
#else
		float angle = angular_rotation.getLength();
		if (angle <= 0)
		{
			return;
		}

		this->rotatePost(angular_rotation * (1.0f/angle), angle);
#endif
	}


	/**
	 * reset quaternion to 0 degree rotation
	 */
	void reset()
	{
		i = j = k = (T)0;
		w = (T)1.0;
	}


	/**
	 * LAB WORKSHEET 3, ASSIGNMENT 4
	 */
	/**
	 * apply an existing quaternion rotation to this one
	 */
	CQuaternion<T> &operator*=(const CQuaternion<T> &q)
	{
#if WORKSHEET_3a
        *this = *this * q;
#endif

		return *this;
	}

	/**
	 * LAB WORKSHEET 3, ASSIGNMENT 4
	 */
	/**
	 * apply an existing quaternion rotation to this one
	 */
	CQuaternion<T> operator*(const CQuaternion<T> &q)
	{
		CQuaternion<T> p;

#if WORKSHEET_3a
        p.w = w * q.w - (i * q.i + j * q.j + k * q.k);
        p.i = w * q.i + i * q.w + j * q.k - k * q.j;
        p.j = w * q.j + j * q.w + k * q.i - i * q.k;
        p.k = w * q.k + k * q.w + i * q.j - j * q.i;
#else
		p = q;
#endif
		return p;
	}

	CQuaternion<T>& operator+=(const CQuaternion<T> &q)
	{
		w += q.w;
		i += q.i;
		j += q.j;
		k += q.k;

		return *this;
	}

	CQuaternion<T> operator+(const CQuaternion<T> &q)
	{
		return CQuaternion(w+q.w, i+q.i, j+q.j, k+q.k);
	}


	/**
	 * apply the rotation from the "right side".
	 *
	 * thus first the point is rotated with the rotation specified by the
	 * parameters, then the point is rotated with the quaternion stored
	 * in this class before this method was called.
	 */
	CQuaternion<T> &rotatePost(
			const CVector<3,float> &p_axis,
			T p_angle
	)
	{
		/*
		 * rotation is computed by converting the angular rotation into a quaternion.
		 * then the rotation is applied by multiplication of both quaternions.
		 */
		CQuaternion q;
		q.setRotation(p_axis, p_angle);
		*this *= q;

		return *this;
	}


	/**
	 * LAB WORKSHEET 3, ASSIGNMENT 2
	 *
	 * compute the intersection between a plane and a box
	 */
	/**
	 * set the quaternion to a rotation around axis
	 * \param angle	given in radians
	 * \param axis	normalized axis of rotation with length 1
	 */
	CQuaternion<T> &setRotation(const CVector<3,float> &axis, T angle)
	{
#if WORKSHEET_3a
        T c = CMath<T>::cos(angle * static_cast<T>(0.5));
        T s = CMath<T>::sin(angle * static_cast<T>(0.5));
        
        w = c;
        i = axis.data[0] * s;
        j = axis.data[1] * s;
        k = axis.data[2] * s;
#endif
		return *this;
	}



	/**
	 * return the angular vector representing the rotation.
	 * the direction of the vector represents the rotation axis.
	 * the length of the vector gives the amount of rotation.
	 */
#if 0
	// TODO: test sign of axis (maybe flip it)
	CVector<3,T> getRotationVector()
	{
		/*
		 * do the inverse of this:
		 *
			const CVector<3,float> &axis; T angle;
			T sinhalf = CMath<T>::sin(angle*(T)0.5);
			i = sinhalf*axis.data[0];
			j = sinhalf*axis.data[1];
			k = sinhalf*axis.data[2];
			w = CMath<T>::cos(angle*(T)0.5);
		 */
		CVector<3,T> t(i,j,k);

		T sinhalf = t.getLength();
		T angle = CMath<T>::asin(sinhalf)*2.0;

		t *= angle;

		return t;
	}
#endif


	/**
	 * LAB WORKSHEET 3
	 *
	 * compute the intersection between a plane and a box
	 */
	/**
	 * return the rotation matrix which describes the rotation of this
	 * quaternion.
	 */
	CMatrix3<T> getRotationMatrix()
	{
#if WORKSHEET_3a
        CMatrix3<T> m;
        m[0][0] = 1 - 2 * (j * j + k * k);
        m[0][1] = 2 * (i * j + k * w);
        m[0][2] = 2 * (i * k - j * w);
        
        m[1][0] = 2 * (i * j - k * w);
        m[1][1] = 1 - 2 * (i * i + k * k);
        m[1][2] = 2 * (j * k + i * w);
        
        m[2][0] = 2 * (i * k + j * w);
        m[2][1] = 2 * (j * k - i * w);
        m[2][2] = 1 - 2 * (i * i + j * j);
        
        return m;
#else
		CMatrix3<T> m;
		m.loadIdentity();
		return m;
#endif
	}

	/**
	 * due to the finite accuracy of the float numbers, it's good to
	 * normalize the quaternion sometimes. to normalize the quaternion
	 * values, just call this function.
	 */
	void normalize()
	{
#if WORKSHEET_3a
        T length = CMath<T>::sqrt(w * w + i * i + j * j + k * k);
        w /= length;
        i /= length;
        j /= length;
        k /= length;
#endif
	}

};



template <class T>
inline
std::ostream&
operator<<(std::ostream &co, const CQuaternion<T> &q)
{
	return co	<< "[" << q.i << ", " << q.j << ", " << q.k << ", " << q.w << "]";
}


#endif
