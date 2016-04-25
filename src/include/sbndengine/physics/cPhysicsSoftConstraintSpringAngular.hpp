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

#ifndef __I_PHYSICS_SOFT_CONSTRAINT_SPRING_ANGULAR_HPP__
#define __I_PHYSICS_SOFT_CONSTRAINT_SPRING_ANGULAR_HPP__

#include "sbndengine/physics/iPhysicsSoftConstraint.hpp"
#include "sbndengine/physics/iPhysicsObject.hpp"
#include "sbndengine/iRef.hpp"

/**
 * spring force which also affects the angular acceleration
 */
class cPhysicsSoftConstraintSpringAngular	: public iPhysicsSoftConstraint
{
	iRef<iPhysicsObject> physics_object1;
	CVector<3,float> object_point1;
	iRef<iPhysicsObject> physics_object2;
	CVector<3,float> object_point2;

	float spring_constant;
	float equilibrium_length;
	float damping;

	bool active;

	bool no_pushing_force;

public:
	cPhysicsSoftConstraintSpringAngular(
			iRef<iPhysicsObject> &p_physics_object1,	///< 1st object which is affected by the spring
			const CVector<3,float> &p_object_point1,	///< the point in object space of the 1st object where the spring is attached
			iRef<iPhysicsObject> &p_physics_object2,	///< 2nd object which is affected by the spring
			const CVector<3,float> &p_object_point2,	///< the point in object space of the 2nd object where the spring is attached
			float p_equilibrium_length = 1,
			float p_spring_constant = 5,
			float p_damping = 0.05,
			bool p_no_pushing_force = true
		);

	cPhysicsSoftConstraintSpringAngular(
			iPhysicsObject &p_physics_object1,			///< 1st object which is affected by the spring
			const CVector<3,float> &p_object_point1,	///< the point in object space of the 1st object where the spring is attached
			iPhysicsObject &p_physics_object2,			///< 2nd object which is affected by the spring
			const CVector<3,float> &p_object_point2,	///< the point in object space of the 2nd object where the spring is attached
			float p_equilibrium_length = 1,
			float p_spring_constant = 5,
			float p_damping = 0.05,
			bool p_no_pushing_force = true
		);

	void updateAcceleration(double frame_elapsed_seconds);

	void activate();
	void deactivate();
};

#endif
