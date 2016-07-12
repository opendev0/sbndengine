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

		if ((c.physics_object1->no_rotations_and_frictions && c.physics_object2->no_rotations_and_frictions)
#if !WORKSHEET_6
				|| 1
#endif
			)
		{
#if WORKSHEET_3
			//no friction or rotations, apply linear impulse

#ifdef DEBUG
			float eKin1 = 0;
			if (c.physics_object1->inv_mass > 0) {
				eKin1 += 0.5f / c.physics_object1->inv_mass * c.physics_object1->velocity.getLength2();
			}
			if (c.physics_object2->inv_mass > 0) {
				eKin1 += 0.5f / c.physics_object2->inv_mass * c.physics_object2->velocity.getLength2();
			}
#endif




			//velocities in direction of collision normal
			float collision_velocity1 = (-c.collision_normal).dotProd(c.physics_object1->velocity);
			float collision_velocity2 = (-c.collision_normal).dotProd(c.physics_object2->velocity);
			CVector<3,float> closing_velocity = c.collision_normal*(collision_velocity1 - collision_velocity2);

			float c_r = (c.physics_object1->restitution_coefficient + c.physics_object2->restitution_coefficient)/2.0;

			c.physics_object1->velocity -= closing_velocity*(c.physics_object1->inv_mass/(c.physics_object1->inv_mass + c.physics_object2->inv_mass))*(-(1+c_r));
			c.physics_object2->velocity += closing_velocity*(c.physics_object2->inv_mass/(c.physics_object1->inv_mass + c.physics_object2->inv_mass))*(-(1+c_r));





#ifdef DEBUG
			// Check sum of all forces = 0
			if (c.physics_object1->inv_mass > 0 && c.physics_object2->inv_mass > 0) {
				float force1 = (-c.collision_normal.dotProd(c.physics_object1->velocity) - collision_velocity1) / (c.physics_object1->inv_mass * frame_elapsed_time);
				float force2 = (-c.collision_normal.dotProd(c.physics_object2->velocity) - collision_velocity2) / (c.physics_object2->inv_mass * frame_elapsed_time);
				
				if (force1 + force2 > EPSILON * CMath<float>::max(1, CMath<float>::max(fabs(force1), fabs(force2)))) {
					std::cout << "Sum of all forces is not 0 but " << force1 + force2 << "N!" << std::endl;
				}
			}
			
			// Check loss in kinetic energy = expected loss due to Cr
			float energyLoss = 0.5f * (CMath<float>::pow(c_r, 2) - 1) * CMath<float>::pow(collision_velocity1 - collision_velocity2, 2) / (c.physics_object1->inv_mass + c.physics_object2->inv_mass);
			float eKin2 = 0;
			if (c.physics_object1->inv_mass > 0) {
				eKin2 += 0.5f / c.physics_object1->inv_mass * c.physics_object1->velocity.getLength2();
			}
			if (c.physics_object2->inv_mass > 0) {
				eKin2 += 0.5f / c.physics_object2->inv_mass * c.physics_object2->velocity.getLength2();
			}

			// Combination of absolute and relative error check to compensate big and small values of energies
			if (fabs(eKin2 - eKin1 - energyLoss) > EPSILON * CMath<float>::max(1, CMath<float>::max(fabs(energyLoss), fabs(eKin2 - eKin1)))) {
				std::cout << "ENERGY CONSERVATION ERROR" << std::endl;
				std::cout << "Lost energy should be " << energyLoss << "J but is " << (eKin2 - eKin1) << "J!" << std::endl;
				std::cout << "Difference: " << fabs(eKin2 - eKin1) - fabs(energyLoss) << "J" << std::endl;
			}
#endif
#endif
		}
		else if ((c.physics_object1->friction_disabled && c.physics_object2->friction_disabled)
#if !WORKSHEET_7
|| 1
#endif
				)
		{
#if WORKSHEET_6
            CVector<3,float> lever1 = c.collision_point1 - c.physics_object1->object->position;
            CVector<3,float> lever2 = c.collision_point2 - c.physics_object2->object->position;
            
            
            CMatrix4<float> inertia_to_world1 =   c.physics_object1->object->inverse_model_matrix.getTranspose3x3()    //M^(-T)
                                                * c.physics_object1->rotational_inverse_inertia                         //I^(-1)
                                                * c.physics_object1->object->model_matrix.getTranspose3x3();           //M^( T)
                                                
            CMatrix4<float> inertia_to_world2 =   c.physics_object2->object->inverse_model_matrix.getTranspose3x3()    //M^(-T)
                                                * c.physics_object2->rotational_inverse_inertia                         //I^(-1)
                                                * c.physics_object2->object->model_matrix.getTranspose3x3();           //M^( T)
                                                
            float c_r = CMath<float>::min(c.physics_object1->restitution_coefficient, c.physics_object2->restitution_coefficient);
            
            
            
            
            //closing velocities
            CVector<3,float> closing_velocity1 = c.physics_object1->velocity + (c.physics_object1->angular_velocity % lever1);
            CVector<3,float> closing_velocity2 = c.physics_object2->velocity + (c.physics_object2->angular_velocity % lever2);
            
            float closing_velocity = c.collision_normal.dotProd(closing_velocity1 - closing_velocity2);
            
    
            
            
            //seperating velocities
            CVector<3,float> seperating_linear_velocity1 = c.collision_normal*c.physics_object1->inv_mass;
            CVector<3,float> seperating_angular_velocity1 = inertia_to_world1 * (lever1 % c.collision_normal);
            CVector<3,float> seperating_velocity1 = seperating_linear_velocity1 + (seperating_angular_velocity1 % lever1);
            
            CVector<3,float> seperating_linear_velocity2 = c.collision_normal*c.physics_object2->inv_mass;
            CVector<3,float> seperating_angular_velocity2 = inertia_to_world2 * (lever2 % c.collision_normal);
            CVector<3,float> seperating_velocity2 = seperating_linear_velocity2 + (seperating_angular_velocity2 % lever2);
            
            float delta_seperating_velocity = c.collision_normal.dotProd(seperating_velocity1 + seperating_velocity2);
            
            
            
            
            //velocities must fullfil seperating_velocity = -c_r * closing_velocity = frac * seperating_velocity + closing_velocity
            float frac = ((1+c_r)*closing_velocity)/delta_seperating_velocity;
            
            
            
            
            //apply calculated impulse to objects
            c.physics_object1->velocity -= seperating_linear_velocity1 * frac;
            c.physics_object1->angular_velocity -= seperating_angular_velocity1 * frac;
            
            c.physics_object2->velocity += seperating_linear_velocity2 * frac;
            c.physics_object2->angular_velocity += seperating_angular_velocity2 * frac;
#endif
		}
		else
		{
#if WORKSHEET_7            
            
#endif
		}


// DAMPING TEST
#if 1
		c.physics_object1->velocity -= c.physics_object1->velocity*frame_elapsed_time*0.5;
		c.physics_object2->velocity -= c.physics_object2->velocity*frame_elapsed_time*0.5;
		c.physics_object1->angular_velocity -= c.physics_object1->angular_velocity*frame_elapsed_time*0.5;
		c.physics_object2->angular_velocity -= c.physics_object2->angular_velocity*frame_elapsed_time*0.5;
#endif

#if 1
		if (c.physics_object1->velocity.getLength() < 0.01)	c.physics_object1->velocity.setZero();
		if (c.physics_object2->velocity.getLength() < 0.01)	c.physics_object2->velocity.setZero();

		if (c.physics_object1->angular_velocity.getLength() < 0.2)	c.physics_object1->angular_velocity.setZero();
		if (c.physics_object2->angular_velocity.getLength() < 0.2)	c.physics_object2->angular_velocity.setZero();
#endif
	}


};
#endif
