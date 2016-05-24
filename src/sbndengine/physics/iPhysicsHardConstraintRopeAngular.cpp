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
#include "sbndengine/physics/cPhysicsHardConstraintRopeAngular.hpp"
#include "sbndengine/physics/iPhysics.hpp"
#include "cPhysicsIntersections.hpp"
#include <list>
#include "worksheets_precompiler.hpp"

cPhysicsHardConstraintRopeAngular::cPhysicsHardConstraintRopeAngular(
		iRef<iPhysicsObject> &p_physics_object1,	///< 1st object which is affected by the spring
		const CVector<3,float> &p_object_point1,	///< the point in object space of the 1st object where the spring is attached
		iRef<iPhysicsObject> &p_physics_object2,	///< 2nd object which is affected by the spring
		const CVector<3,float> &p_object_point2,	///< the point in object space of the 2nd object where the spring is attached
		float p_equilibrium_length,
		float p_coefficient_of_restitution
	)	:
	physics_object1(p_physics_object1),
	object_point1(p_object_point1),
	physics_object2(p_physics_object2),
	object_point2(p_object_point2),

	equilibrium_length(p_equilibrium_length),
	coefficient_of_restitution(p_coefficient_of_restitution)
{
}


cPhysicsHardConstraintRopeAngular::cPhysicsHardConstraintRopeAngular(
		iPhysicsObject &p_physics_object1,
		const CVector<3,float> &p_object_point1,	///< the point in object space of the 1st object where the spring is attached
		iPhysicsObject &p_physics_object2,
		const CVector<3,float> &p_object_point2,	///< the point in object space of the 2nd object where the spring is attached
		float p_equilibrium_length,
		float p_coefficient_of_restitution
	)	:
	physics_object1(p_physics_object1),
	object_point1(p_object_point1),
	physics_object2(p_physics_object2),
	object_point2(p_object_point2),

	equilibrium_length(p_equilibrium_length),
	coefficient_of_restitution(p_coefficient_of_restitution)
{
}

bool cPhysicsHardConstraintRopeAngular::updateHardConstraintsCollisions(class CPhysicsCollisionData &c)
{
#if WORKSHEET_3
    CVector<3, float> world_point1 = physics_object1->object->model_matrix * object_point1;
    CVector<3, float> world_point2 = physics_object2->object->model_matrix * object_point2;
    CVector<3, float> dist = (world_point2 - world_point1);

	if (dist.getLength() < equilibrium_length) {
		return false;
	}

	c.physics_object1 = &physics_object1.getClass();
	c.physics_object2 = &physics_object2.getClass();
	c.collision_normal = dist.getNormalized();
	c.collision_point1 = world_point1;
	c.collision_point2 = world_point2;
	c.interpenetration_depth = equilibrium_length - dist.getLength();

	return true;
#else
	return false;
#endif
}

cPhysicsHardConstraintRopeAngular::~cPhysicsHardConstraintRopeAngular()
{
}
