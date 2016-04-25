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

#ifndef __I_PHYSICS_OBJECT_HPP__
#define __I_PHYSICS_OBJECT_HPP__

#include "libmath/CVector.hpp"
#include "libmath/CMatrix.hpp"
#include "libmath/CQuaternion.hpp"
#include "sbndengine/iRef.hpp"
#include "sbndengine/engine/iObject.hpp"
#include <iostream>

/**
 * \brief physics object handler
 *
 * storage for physics object.
 * this class stores:
 *  - the physical relevant parameters
 *  - the current state of the object
 *  - the accumulators for the integrator regarding the object
 */
class iPhysicsObject	:	public iBase
{
	friend class CCollision;
	friend class iPhysicsEngine;
	friend class iPhysicsConstraintSpring;
	friend class cPhysicsEngine_Private;

public:
	/**
	 * reference to the iObject
	 */
	iRef<iObject> object;

	/**
	 * the linear velocity of the object
	 */
	CVector<3,float> velocity;

	/**
	 * the angular speed and angular speed alignment of the object - given in world-space
	 *
	 * the length of this vector specifies the amount of speed
	 */
	CVector<3,float> angular_velocity;

	/**
	 * integrator variable: the linear acceleration for which the object is exerted for one frame
	 */
	CVector<3,float> linear_acceleration_accumulator;

	/**
	 * integrator variable: the linear acceleration for which the object is exerted for one frame
	 */
	CVector<3,float> torque_accumulator;

	/**
	 * this flag controls how to apply impulses for collision handling.
	 *
	 * e. g. it is easier to apply impulses if the object is invariant to rotations
	 */
	bool no_rotations_and_frictions;

	/**
	 * movable is set to false to avoid any movements of the object.
	 * this can also be done by setting inv_mass to zero
	 */
	bool movable;

	/**
	 * inverse mass
	 *
	 * storing the inverse mass has some specific advantages since most of the time the inverse
	 * mass is more frequently used and the equations using 'only' the mass can be rewritten to
	 * utilize the inverse mass
	 */
	float inv_mass;

	/**
	 * coefficient of restitution (COF) controls the amount of separating velocity depending
	 * on the closing velocity
	 *
	 * the COF for 2 objects is computed as the average COF of both
	 */
	float restitution_coefficient;

	/**
	 * static friction coefficient
	 */
	float friction_static_coefficient;

	/**
	 * dynamic friction coefficient
	 */
	float friction_dynamic_coefficient;

	/**
	 * flag to disable friction handling at all
	 */
	bool friction_disabled;

	/**
	 * the rotational inertia setup once from the factory
	 */
	CMatrix3<float> rotational_inertia;
	CMatrix3<float> rotational_inverse_inertia;

public:
	iPhysicsObject(
			const iRef<iObject> &p_object,
			float p_resitution_coefficient = 0.3,
			float p_friction_dynamic_coefficient = 0.3,
			float p_friction_static_coefficient = 0.4
		);

	void setInverseMass(float p_inv_mass);
	void setMass(float p_mass);
	void setCoefficientOfRestitution(float p_coefficient_of_restitution);

	void setMoveability(bool movable);

	/**
	 * set the acceleration and torque to zero
	 */
	void setZero();

	/**
	 * set the speed to the given value
	 */
	void setSpeed(const CVector<3,float> &p_speed);

	/**
	 * add p_speed the the current speed value
	 */
	void addSpeed(const CVector<3,float> &p_speed);

	/**
	 * set the angular speed to the given value
	 */
	void setAngularSpeed(const CVector<3,float> &p_angular_speed);

	/**
	 * add p_angular_speed the the current angular speed value
	 */
	void addAngularSpeed(const CVector<3,float> &p_angular_speed);

	/**
	 * add a torque to the existing one
	 */
	void addTorque(const CVector<3,float> &p_torque);

	/**
	 * set the torque value to p_torque
	 */
	void setTorque(const CVector<3,float> &p_torque);

#if 0
	/**
	 * add a rotational force in the direction p_force at the point p_point
	 *
	 * (torque += p_point % p_force)
	 */
	void addRotationalForce(
			const CVector<3,float> &p_point,
			const CVector<3,float> &p_force
		);

	/**
	 * set the rotational force in the direction p_force at the point p_point
	 *
	 * (torque = p_point % p_force)
	 */
	void setRotationalForce(
			const CVector<3,float> &p_point,
			const CVector<3,float> &p_force
		);
#endif
	/**
	 * return true, if the inverse mass is not 0 (mass != infinite)
	 */
	bool isMovable();

	/*
	 * set the rotation flag.
	 *
	 * if this flag is set, e. g. no rotation is done on the object and only
	 * linear impulses acting directly on the center are applied to the object
	 */
	void setDisableCollisionRotationAndFrictionFlag(bool p_flag);

	virtual ~iPhysicsObject();
};

#endif
