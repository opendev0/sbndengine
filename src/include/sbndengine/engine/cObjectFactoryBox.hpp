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

#ifndef __C_OBJECT_FACTORY_BOX_HPP__
#define __C_OBJECT_FACTORY_BOX_HPP__

#include "libmath/CVector.hpp"
#include "libmath/CMatrix.hpp"
#include "iObjectFactory.hpp"

/**
 * this class implements a factory for a box primitive
 */
class cObjectFactoryBox	:
	public iObjectFactory
{
	friend class iPhysicsEngine;
	friend class cPhysicsEngine_Private;
	friend class CPhysicsIntersections;

	CVector<3,float> size;
	public: CVector<3,float> half_size;

public:
	/**
	 * return the rotational inertia of this object
	 */
	CMatrix3<float> getRotationalInertia();

	/**
	 * get the inverse mass value for this object
	 */
	float getInverseMass();

	/**
	 * setup the box factory
	 */
	cObjectFactoryBox(
			float p_size_x = 1.0,
			float p_size_y = 1.0,
			float p_size_z = 1.0
		);

	/**
	 * resize the object
	 */
	void resizeBox(
			float p_size_x = 1.0,
			float p_size_y = 1.0,
			float p_size_z = 1.0
		);

	/**
	 * set the mass
	 */
	void setMass(
			float p_mass = 1.0
		);
};

#endif // __I_OBJECT_FACTORY_BOX_HPP__
