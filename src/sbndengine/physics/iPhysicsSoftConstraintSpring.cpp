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

#include "sbndengine/physics/cPhysicsSoftConstraintSpring.hpp"
#include "cPhysicsIntersections.hpp"
#include "worksheets_precompiler.hpp"


cPhysicsSoftConstraintSpring::cPhysicsSoftConstraintSpring(
		iRef<iPhysicsObject> &p_physics_object1,
		iRef<iPhysicsObject> &p_physics_object2,
		float p_equilibrium_length,
		float p_spring_constant,
		float p_damping,
		bool p_no_pushing_force
	)	:
	physics_object1(p_physics_object1),
	physics_object2(p_physics_object2),

	spring_constant(p_spring_constant),
	equilibrium_length(p_equilibrium_length),
	damping(p_damping),
	no_pushing_force(p_no_pushing_force)
{

}


cPhysicsSoftConstraintSpring::cPhysicsSoftConstraintSpring(
		iPhysicsObject &p_physics_object1,
		iPhysicsObject &p_physics_object2,
		float p_equilibrium_length,
		float p_spring_constant,
		float p_damping,
		bool p_no_pushing_force
	)	:
	physics_object1(p_physics_object1),
	physics_object2(p_physics_object2),

	spring_constant(p_spring_constant),
	equilibrium_length(p_equilibrium_length),
	damping(p_damping),
	no_pushing_force(p_no_pushing_force)
{

}


/**
 * WORKSHEET 3, ASSIGNMENT 1
 *
 * update the acceleration of an object connected by a spring
 *
 * The force directly depends on the length and a spring coefficient
 *
 * Then the acceleration is modified depending on the elapsed seconds during the last frame and the mass of the object
 */
void cPhysicsSoftConstraintSpring::updateAcceleration(double frame_elapsed_seconds)
{
#if WORKSHEET_3
	CVector<3, float> distance = (physics_object2->object->position - physics_object1->object->position);
	CVector<3, float> normal = distance.getNormalized();
	float force = (equilibrium_length - distance.getLength()) * spring_constant;
	physics_object1->linear_acceleration_accumulator -= normal * force * physics_object1->inv_mass;
	physics_object2->linear_acceleration_accumulator += normal * force * physics_object2->inv_mass;
#endif
}
