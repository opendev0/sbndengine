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


#include "sbndengine/iRef.hpp"
#include "sbndengine/physics/iPhysicsHardConstraint.hpp"
#include "sbndengine/physics/cPhysicsHardConstraintRope.hpp"
#include "sbndengine/physics/iPhysics.hpp"
#include "cPhysicsIntersections.hpp"
#include <list>
#include "worksheets_precompiler.hpp"

cPhysicsHardConstraintRope::cPhysicsHardConstraintRope(
		iPhysicsObject &p_physics_object1,
		iPhysicsObject &p_physics_object2,
		float p_equilibrium_length,
		float p_coefficient_of_restitution
	)	:
	physics_object1(p_physics_object1),
	physics_object2(p_physics_object2),

	equilibrium_length(p_equilibrium_length),
	coefficient_of_restitution(p_coefficient_of_restitution)
{
}


cPhysicsHardConstraintRope::cPhysicsHardConstraintRope(
		iRef<iPhysicsObject> &p_physics_object1,
		iRef<iPhysicsObject> &p_physics_object2,
		float p_equilibrium_length,
		float p_coefficient_of_restitution
	)	:
	physics_object1(p_physics_object1),
	physics_object2(p_physics_object2),

	equilibrium_length(p_equilibrium_length),
	coefficient_of_restitution(p_coefficient_of_restitution)
{
}

/**
 * WORKSHEET 3, ASSIGNMENT 3
 */
bool cPhysicsHardConstraintRope::updateHardConstraintsCollisions(class CPhysicsCollisionData &c)
{
#if WORKSHEET_3

#else
	return false;
#endif
}

cPhysicsHardConstraintRope::~cPhysicsHardConstraintRope()
{
}
