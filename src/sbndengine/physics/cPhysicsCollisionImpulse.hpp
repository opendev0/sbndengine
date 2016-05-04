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

#ifndef CPHYSICS_COLLISION_IMPULSE_HPP
#define CPHYSICS_COLLISION_IMPULSE_HPP

#include "cPhysicsCollisionData.hpp"
#include "worksheets_precompiler.hpp"

/*
 * this class handles the change of object velocity (linear and angular)
 * by applying an impulse
 */
class CPhysicsCollisionImpulse
{
public:
	static inline void applyCollisionImpulse(CPhysicsCollisionData &c, double frame_elapsed_time)
	{
#if WORKSHEET_6

#endif

		if (	(c.physics_object1->no_rotations_and_frictions && c.physics_object2->no_rotations_and_frictions)
#if !WORKSHEET_6
				|| 1
#endif
			)
		{
#if WORKSHEET_3
			//no friction or rotations, apply linear impulse

			//velocities in direction of collision normal
			float collision_velocity1 = (-c.collision_normal).dotProd(c.physics_object1->velocity);
			float collision_velocity2 = (-c.collision_normal).dotProd(c.physics_object2->velocity);
			float closing_velocity = (collision_velocity1 - collision_velocity2);

			float coefficient_of_restitution = (c.physics_object1->restitution_coefficient * c.physics_object2->restitution_coefficient)/2.0;

			c.physics_object1->velocity = -c.collision_normal*closing_velocity*(c.physics_object1->inv_mass/(c.physics_object1->inv_mass + c.physics_object2->inv_mass))*(-(1+coefficient_of_restitution)) + c.physics_object1->velocity;
			c.physics_object2->velocity = c.collision_normal*closing_velocity*(c.physics_object2->inv_mass/(c.physics_object1->inv_mass + c.physics_object2->inv_mass))*(-(1+coefficient_of_restitution)) + c.physics_object2->velocity;
#endif
		}
		else if ((c.physics_object1->friction_disabled && c.physics_object2->friction_disabled)
#if !WORKSHEET_7
|| 1
#endif
				)
		{
#if WORKSHEET_6

#endif
		}
		else
		{
#if WORKSHEET_7

#endif
		}


// DAMPING TEST
#if 0
		c.physics_object1->velocity -= c.physics_object1->velocity*frame_elapsed_time*0.5;
		c.physics_object2->velocity -= c.physics_object2->velocity*frame_elapsed_time*0.5;
		c.physics_object1->angular_velocity -= c.physics_object1->angular_velocity*frame_elapsed_time*0.5;
		c.physics_object2->angular_velocity -= c.physics_object2->angular_velocity*frame_elapsed_time*0.5;
#endif

#if 0
		if (c.physics_object1->velocity.getLength() < 0.5)	c.physics_object1->velocity.setZero();
		if (c.physics_object2->velocity.getLength() < 0.5)	c.physics_object2->velocity.setZero();

		if (c.physics_object1->angular_velocity.getLength() < 0.2)	c.physics_object1->angular_velocity.setZero();
		if (c.physics_object2->angular_velocity.getLength() < 0.2)	c.physics_object2->angular_velocity.setZero();
#endif
	}

};

#endif
