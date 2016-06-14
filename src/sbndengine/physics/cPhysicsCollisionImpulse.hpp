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

			// Closing velocity in direction of collision normal
			float closing_velocity = c.collision_normal.dotProd(c.physics_object2->velocity - c.physics_object1->velocity);

			// Useful factors
			float m1_frac = (c.physics_object1->inv_mass / (c.physics_object1->inv_mass + c.physics_object2->inv_mass));
			float cor_factor = 1 + (c.physics_object1->restitution_coefficient + c.physics_object2->restitution_coefficient) / 2.0f;

			c.physics_object1->velocity += c.collision_normal * closing_velocity * cor_factor * m1_frac;
			c.physics_object2->velocity -= c.collision_normal * closing_velocity * cor_factor * (1 - m1_frac);

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
			float energyLoss = 0.5f * (CMath<float>::pow(coefficient_of_restitution, 2) - 1) * CMath<float>::pow(collision_velocity1 - collision_velocity2, 2) / (c.physics_object1->inv_mass + c.physics_object2->inv_mass);
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
			// Calculate some useful factors
			float cor_factor = 1 + (c.physics_object1->restitution_coefficient + c.physics_object2->restitution_coefficient) / 2.0f;
			CVector<3, float> lever1 = c.collision_point1 - c.physics_object1->object->position;
			CVector<3, float> lever2 = c.collision_point2 - c.physics_object2->object->position;

			// Calculate closing velocity of the two collision points (linear and angular)
			CVector<3, float> collision_velocity1 = c.physics_object1->velocity + (c.physics_object1->angular_velocity % lever1);
			CVector<3, float> collision_velocity2 = c.physics_object2->velocity + (c.physics_object2->angular_velocity % lever2);
			float closing_velocity = c.collision_normal.dotProd(collision_velocity2 - collision_velocity1);

			// Calculate change in translational velocity if unit impulse would be applied
			CVector<3, float> linear_velocity_change1 = c.collision_normal * c.physics_object1->inv_mass;
			CVector<3, float> linear_velocity_change2 = c.collision_normal * c.physics_object2->inv_mass;

			// Calculate changes in rotational velocity if unit impulse would be applied
			CMatrix4<float> inertia_to_world1 =
				c.physics_object1->object->inverse_model_matrix.getTranspose3x3() *
				c.physics_object1->rotational_inverse_inertia *
				c.physics_object1->object->model_matrix.getTranspose3x3();
			CVector<3, float> rot_velocity_change1 = inertia_to_world1 * (lever1 % c.collision_normal);

			CMatrix4<float> inertia_to_world2 =
				c.physics_object2->object->inverse_model_matrix.getTranspose3x3() *
				c.physics_object2->rotational_inverse_inertia *
				c.physics_object2->object->model_matrix.getTranspose3x3();
			CVector<3, float> rot_velocity_change2 = inertia_to_world2 * (lever2 % c.collision_normal);

			// Calculate total changes in velocities if unit impulse would be applied
			CVector<3, float> velocity_change1 = linear_velocity_change1 + rot_velocity_change1 % lever1;
			CVector<3, float> velocity_change2 = linear_velocity_change2 + rot_velocity_change2 % lever2;

			if (CMath<float>::abs(c.collision_normal.dotProd(velocity_change1 + velocity_change2)) < 0.01) return;

			// No minus sign, because the signs at the application of velocity change are also inverted
			float f = (closing_velocity * cor_factor) / c.collision_normal.dotProd(velocity_change1 + velocity_change2);

			std::cout << "Object 1: " << c.physics_object1->object->identifier_string << std::endl;
			std::cout << "Object 2: " << c.physics_object2->object->identifier_string << std::endl;
			std::cout << "closing velocity: " << closing_velocity << std::endl;
			std::cout << "ω1: " << c.physics_object1->angular_velocity << std::endl;
			std::cout << "ω2: " << c.physics_object2->angular_velocity << std::endl;
			std::cout << "lever1: " << lever1 << std::endl;
			std::cout << "lever2: " << lever2 << std::endl;
			std::cout << "ω1 x lever1: " << (c.physics_object1->angular_velocity % lever1) << std::endl;
			std::cout << "ω2 x lever2: " << (c.physics_object2->angular_velocity % lever2) << std::endl;
			std::cout << "collision normal: " << c.collision_normal << std::endl;
			std::cout << "Δv1: " << velocity_change1 << std::endl;
			std::cout << "Δv2: " << velocity_change2 << std::endl;
			std::cout << "Δω1: " << rot_velocity_change1 << std::endl;
			std::cout << "Δω2: " << rot_velocity_change2 << std::endl;
			std::cout << "f: " << f << std::endl << std::endl;

			c.physics_object1->velocity += linear_velocity_change1 * f;
			c.physics_object2->velocity -= linear_velocity_change2 * f;
			c.physics_object1->angular_velocity += rot_velocity_change1 * f;
			c.physics_object2->angular_velocity -= rot_velocity_change2 * f;
#endif
		}
		else
		{
#if WORKSHEET_7
			// Calculate some useful factors
			float cor_factor = 1 + (c.physics_object1->restitution_coefficient + c.physics_object2->restitution_coefficient) / 2.0f;
			float stat_fric_factor = (c.physics_object1->friction_static_coefficient + c.physics_object2->friction_static_coefficient) / 2.0f;
			float dyn_fric_factor = (c.physics_object1->friction_dynamic_coefficient + c.physics_object2->friction_dynamic_coefficient) / 2.0f;
			CVector<3, float> lever1 = c.collision_point1 - c.physics_object1->object->position;
			CVector<3, float> lever2 = c.collision_point2 - c.physics_object2->object->position;
			CMatrix3<float> lever_matrix1 = CMatrix3<float>().setupCrossProduct(lever1);
			CMatrix3<float> lever_matrix2 = CMatrix3<float>().setupCrossProduct(lever2);

			// Calculate closing velocity of the two collision points (linear and angular)
			CVector<3, float> collision_velocity1 = c.physics_object1->velocity + (c.physics_object1->angular_velocity % lever1);
			CVector<3, float> collision_velocity2 = c.physics_object2->velocity + (c.physics_object2->angular_velocity % lever2);
			CVector<3, float> closing_velocity = collision_velocity2 - collision_velocity1;

			float closing_velocity_normal = c.collision_normal.dotProd(closing_velocity);
			CVector<3, float> planar_velocity = closing_velocity - c.collision_normal * closing_velocity_normal;

			// Define standard basis as orthonormal basis in collision space
			CMatrix3<float> collision_basis; // Identity matrix

			// Calculate change in linear velocity if unit impulse would be applied
			CMatrix3<float> linear_velocity_change1 = collision_basis * c.physics_object1->inv_mass;
			CMatrix3<float> linear_velocity_change2 = collision_basis * c.physics_object2->inv_mass;

			// Calculate change in rotational velocity if unit impulse would be applied
			// ΔL = r x Δp (Δp: unit impulse)
			CMatrix3<float> angular_impulse_change1 = lever_matrix1 * collision_basis;
			CMatrix3<float> angular_impulse_change2 = lever_matrix2 * collision_basis;

			// ΔL = I * Δω
			CMatrix3<float> rot_velocity_change1 =
				c.physics_object1->object->inverse_model_matrix.getTranspose() *
				c.physics_object1->rotational_inverse_inertia *
				c.physics_object1->object->model_matrix.getTranspose() *
				angular_impulse_change1;
			CMatrix3<float> rot_velocity_change2 =
				c.physics_object2->object->inverse_model_matrix.getTranspose() *
				c.physics_object2->rotational_inverse_inertia *
				c.physics_object2->object->model_matrix.getTranspose() *
				angular_impulse_change2;

			// Velocity change in collision space
			CMatrix3<float> velocity_change1 = linear_velocity_change1 + rot_velocity_change1 * lever_matrix1;
			CMatrix3<float> velocity_change2 = linear_velocity_change2 + rot_velocity_change2 * lever_matrix2;

			// Transform velocity change to world space
			CMatrix3<float> contact_velocity_change1 = collision_basis.getInverse() * velocity_change1;
			CMatrix3<float> contact_velocity_change2 = collision_basis.getInverse() * velocity_change2;

			//float f = (contact_velocity_change1 + contact_velocity_change2).getInverse() * closing_velocity_normal * cor_factor;
			//CVector<3, float> vec_linear_velocity_change = -c.collision_normal * f;
			CVector<3, float> vec_velocity_change = (contact_velocity_change1 + contact_velocity_change2) * -c.collision_normal;

			if (planar_velocity.getLength() <= CMath<float>::abs(closing_velocity_normal) * stat_fric_factor) {
				closing_velocity -= planar_velocity;
			} else {
				closing_velocity -= planar_velocity * dyn_fric_factor;
			}

			//CVector<3, float> f = (contact_velocity_change1 + contact_velocity_change2).getInverse() * (closing_velocity * -cor_factor);

			// v_s = -Cr * closing_velocity = closing_velocity + f * (contact_velocity_change1 + contact_velocity_change1)
			CVector<3, float> f = (contact_velocity_change1 + contact_velocity_change2).getInverse() * (closing_velocity * -cor_factor);
			//CVector<3, float> f = (contact_velocity_change1 + contact_velocity_change2).getInverse() * (vec_velocity_change * -cor_factor);

			//std::cout << "impulse: " << p << std::endl;

			c.physics_object1->velocity += linear_velocity_change1 * f;
			c.physics_object2->velocity -= linear_velocity_change2 * f;

			c.physics_object1->angular_velocity += rot_velocity_change1 * f;
			c.physics_object2->angular_velocity -= rot_velocity_change2 * f;
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
