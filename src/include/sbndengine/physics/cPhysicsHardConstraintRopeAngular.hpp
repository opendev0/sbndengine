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

#ifndef __I_PHYSICS_HARD_CONSTRAINT_ROPE_ANGULAR_HPP__
#define __I_PHYSICS_HARD_CONSTRAINT_ROPE_ANGULAR_HPP__

#include "sbndengine/iRef.hpp"
#include "sbndengine/physics/iPhysicsHardConstraint.hpp"
#include "sbndengine/physics/iPhysicsObject.hpp"


/**
 * \brief rope connecting 2 objects by a hard constraint
 *
 * the impulse is applied with rotational forces
 */
class cPhysicsHardConstraintRopeAngular	: public iPhysicsHardConstraint
{
	iRef<iPhysicsObject> physics_object1;
	CVector<3,float> object_point1;
	iRef<iPhysicsObject> physics_object2;
	CVector<3,float> object_point2;

	float equilibrium_length;
	float coefficient_of_restitution;

	bool active;

public:
	cPhysicsHardConstraintRopeAngular(
			iRef<iPhysicsObject> &p_physics_object1,
			const CVector<3,float> &p_object_point1,	///< the point in object space of the 1st object where the spring is attached
			iRef<iPhysicsObject> &p_physics_object2,
			const CVector<3,float> &p_object_point2,	///< the point in object space of the 2nd object where the spring is attached
			float p_equilibrium_length = 3,
			float p_coefficient_of_restitution = 0.7
		);

	cPhysicsHardConstraintRopeAngular(
			iPhysicsObject &p_physics_object1,
			const CVector<3,float> &p_object_point1,	///< the point in object space of the 1st object where the spring is attached
			iPhysicsObject &p_physics_object2,
			const CVector<3,float> &p_object_point2,	///< the point in object space of the 2nd object where the spring is attached
			float p_equilibrium_length = 3,
			float p_coefficient_of_restitution = 0.7
		);
		
	void setCoefficientOfRestitution(float c_r);

	virtual ~cPhysicsHardConstraintRopeAngular();

	bool updateHardConstraintsCollisions(class CPhysicsCollisionData &c);
};

#endif
