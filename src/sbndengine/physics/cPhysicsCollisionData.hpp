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

#ifndef CPHYSICS_COLLISION_DATA_HPP
#define CPHYSICS_COLLISION_DATA_HPP

class CPhysicsCollisionData	: public iBase
{
public:
	iPhysicsObject *physics_object1;
	iPhysicsObject *physics_object2;

	CVector<3,float> collision_point1;
	CVector<3,float> collision_point2;

	// collision normal aims from o1 to o2
	CVector<3,float> collision_normal;

	// the interpenetration depth
	float interpenetration_depth;

	CPhysicsCollisionData()
	{
	}

	/**
	 * create a new collision dataset
	 *
	 * \param p_physics_object1		the first object involved in the collision
	 * \param p_physics_object2		the second object involved in the collision
	 * \param p_collision_point1	the collision coordinate of the first object
	 * \param p_collision_point2	the collision coordinate of the second object
	 * \param p_collision_normal	the collision normal
	 * \param p_interpenetration_depth	the interpenetration depth
	 */
	CPhysicsCollisionData(	iPhysicsObject *p_physics_object1,
						iPhysicsObject *p_physics_object2,
						const CVector<3,float> &p_collision_point1,
						const CVector<3,float> &p_collision_point2,
						const CVector<3,float> &p_collision_normal,
						float p_interpenetration_depth
						)
	{
#ifdef DEBUG
		if (!(p_interpenetration_depth >= 0))
		{
			p_interpenetration_depth -= 1;	// placeholder instruction for debug break point
		}
		assert(p_interpenetration_depth >= 0);
		assert(!CMath<float>::isNan(p_collision_normal.data[0]));
#else
		if (CMath<float>::isNan(p_collision_normal.data[0]))
			return;
#endif
		physics_object1 = p_physics_object1;
		physics_object2 = p_physics_object2;

		collision_point1 = p_collision_point1;
		collision_point2 = p_collision_point2;
		collision_normal = p_collision_normal;
		interpenetration_depth = p_interpenetration_depth;
	}
};

#endif
