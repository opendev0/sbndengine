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

#include "sbndengine/physics/cPhysicsSoftConstraintSpringAngular.hpp"
#include "cPhysicsIntersections.hpp"
#include "worksheets_precompiler.hpp"


cPhysicsSoftConstraintSpringAngular::cPhysicsSoftConstraintSpringAngular(
		iRef<iPhysicsObject> &p_physics_object1,
		const CVector<3,float> &p_object_point1,
		iRef<iPhysicsObject> &p_physics_object2,
		const CVector<3,float> &p_object_point2,
		float p_equilibrium_length,
		float p_spring_constant,
		float p_damping,
		bool p_no_pushing_force
	)	:
	physics_object1(p_physics_object1),
	object_point1(p_object_point1),
	physics_object2(p_physics_object2),
	object_point2(p_object_point2),

	spring_constant(p_spring_constant),
	equilibrium_length(p_equilibrium_length),
	damping(p_damping),
	no_pushing_force(p_no_pushing_force)
{

}


cPhysicsSoftConstraintSpringAngular::cPhysicsSoftConstraintSpringAngular(
		iPhysicsObject &p_physics_object1,
		const CVector<3,float> &p_object_point1,
		iPhysicsObject &p_physics_object2,
		const CVector<3,float> &p_object_point2,
		float p_equilibrium_length,
		float p_spring_constant,
		float p_damping,
		bool p_no_pushing_force
	)	:
	physics_object1(p_physics_object1),
	object_point1(p_object_point1),
	physics_object2(p_physics_object2),
	object_point2(p_object_point2),

	spring_constant(p_spring_constant),
	equilibrium_length(p_equilibrium_length),
	damping(p_damping),
	no_pushing_force(p_no_pushing_force)
{

}

void cPhysicsSoftConstraintSpringAngular::updateAcceleration(double frame_elapsed_seconds)
{
#if WORKSHEET_3
    CVector<3, float> world_point1 = physics_object1->object->model_matrix * object_point1;
    CVector<3, float> world_point2 = physics_object2->object->model_matrix * object_point2;
    CVector<3, float> dist = (world_point2 - world_point1);
	CVector<3, float> force = (dist.getLength() - equilibrium_length) * (-spring_constant);
    
    CVector<3, float> lever1 = world_point1 - physics_object1->object->position;
    CVector<3, float> lever2 = world_point2 - physics_object2->object->position;
    
    float factor_torque = 1;
    
    physics_object1->linear_acceleration_accumulator += (dist.getNormalized() * force * physics_object1->inv_mass) * (1 - factor_torque);
    physics_object2->linear_acceleration_accumulator -= (dist.getNormalized() * force * physics_object2->inv_mass) * (1 - factor_torque);
    
    physics_object1->torque_accumulator -= (lever1 % (dist.getNormalized() * force)) * factor_torque;
    physics_object2->torque_accumulator += (lever2 % (dist.getNormalized() * force)) * factor_torque;
#endif
}
