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

#ifndef __C_OBJECT_FACTORY_SPHERE_HPP__
#define __C_OBJECT_FACTORY_SPHERE_HPP__

#include "libmath/CVector.hpp"
#include "iObjectFactory.hpp"

/**
 * this class implements a factory for a sphere primitive
 */
class cObjectFactorySphere :
	public iObjectFactory
{
	friend class iPhysicsEngine;
	friend class cPhysicsEngine_Private;
	friend class CPhysicsIntersections;

	float radius;

public:
//	float getRadius() const	{	return radius;	}

	CMatrix3<float> getRotationalInertia();
	float getInverseMass();

	cObjectFactorySphere(
			float p_radius = 1.0,
			int segments_horizontal = 20,
			int segments_vertical= 10
		);

	void resizeSphere(
			float p_radius = 1.0,
			int segments_horizontal = 20,
			int segments_vertical= 10
		);
};

#endif // __I_OBJECT_FACTORY_SPHERE_HPP__
