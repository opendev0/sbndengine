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

#include "sbndengine/physics/iPhysicsObject.hpp"

iPhysicsObject::iPhysicsObject(
		const iRef<iObject> &p_object,
		float p_resitution_coefficient,
		float p_friction_dynamic_coefficient,
		float p_friction_static_coefficient
		)	:
	object(p_object)
{
	// setup physics object to be fixed
	inv_mass = object->objectFactory->getInverseMass();
	if (inv_mass != 0.0)
	{
		rotational_inertia = object->objectFactory->getRotationalInertia();
		rotational_inverse_inertia = rotational_inertia.getInverse();
	}
	else
	{
		rotational_inertia.setZero();
		rotational_inverse_inertia.setZero();
	}
	movable = true;
	object->physics_engine_ptr = this;
	no_rotations_and_frictions = false;
	friction_disabled = false;
	restitution_coefficient = p_resitution_coefficient;

	friction_dynamic_coefficient = p_friction_dynamic_coefficient;
	friction_static_coefficient = p_friction_static_coefficient;
}


void iPhysicsObject::setInverseMass(float p_inv_mass)
{
	inv_mass = p_inv_mass;
	if (inv_mass == 0)
	{
		setMoveability(false);
		// if the object has infinite mass, it's usually not rotated
//		setDisableCollisionRotationAndFrictionFlag(true);
	}
}


bool iPhysicsObject::isMovable()
{
	return inv_mass != 0.0f;
}

void iPhysicsObject::setMass(float p_mass)
{
	inv_mass = 1.0f/p_mass;
}

void iPhysicsObject::setMoveability(bool p_movable)
{
	movable = p_movable;
}

void iPhysicsObject::setCoefficientOfRestitution(float p_restitution_coefficient)
{
	restitution_coefficient = p_restitution_coefficient;
}

void iPhysicsObject::setZero()
{
	velocity.setZero();
	angular_velocity.setZero();
}

void iPhysicsObject::setSpeed(const CVector<3,float> &p_velocity)
{
	velocity = p_velocity;
}

void iPhysicsObject::addSpeed(const CVector<3,float> &p_velocity)
{
	velocity += p_velocity;
}

void iPhysicsObject::setAngularSpeed(const CVector<3,float> &p_angular_velocity)
{
	angular_velocity = p_angular_velocity;
}

void iPhysicsObject::addAngularSpeed(const CVector<3,float> &p_angular_velocity)
{
	angular_velocity += p_angular_velocity;
}

void iPhysicsObject::setDisableCollisionRotationAndFrictionFlag(bool p_flag)
{
	no_rotations_and_frictions = p_flag;
}

iPhysicsObject::~iPhysicsObject()
{
}
