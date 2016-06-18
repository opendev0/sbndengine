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

#include "sbndengine/physics/iPhysics.hpp"

#include "sbndengine/engine/cObjectFactorySphere.hpp"
#include "sbndengine/engine/cObjectFactoryPlane.hpp"
#include "sbndengine/engine/cObjectFactoryBox.hpp"

#include <assert.h>


#include "cPhysicsEngine_Private.hpp"


void iPhysics::reset()
{
	privateClass->reset();
}

iPhysics::iPhysics()
{
	debug.setup(this);
	privateClass = new cPhysicsEngine_Private;
}

iPhysics::~iPhysics()
{
	delete privateClass;
}

void iPhysics::addObject(const iRef<iPhysicsObject> &physicsObject)
{
	privateClass->object_list.push_back(physicsObject);
}

void iPhysics::addSoftConstraint(const iRef<iPhysicsSoftConstraint> &physicsSoftConstraint)
{
	privateClass->soft_constraint_list.push_back(physicsSoftConstraint);
}

void iPhysics::removeSoftConstraint(const iRef<iPhysicsSoftConstraint> &physicsSoftConstraint)
{
	privateClass->soft_constraint_list.remove(physicsSoftConstraint);
}


void iPhysics::addHardConstraint(const iRef<iPhysicsHardConstraint> &physicsHardConstraint)
{
	privateClass->hard_constraint_list.push_back(physicsHardConstraint);
}

void iPhysics::removeHardConstraint(const iRef<iPhysicsHardConstraint> &physicsHardConstraint)
{
	privateClass->hard_constraint_list.remove(physicsHardConstraint);
}

void iPhysics::setGravitation(const CVector<3,float> &p_gravitation_vector)
{
	privateClass->gravitation_vector = p_gravitation_vector;
}

void iPhysics::setMaximumIterations(int p_max_global_iterations, int p_max_local_iterations)
{
	privateClass->setMaximumIterations(p_max_global_iterations, p_max_local_iterations);
}

void iPhysics::simulationTimestep(double p_elapsed_seconds)
{
	if (debug.active)
		debug.simulationTimestep(p_elapsed_seconds);
	else
		privateClass->simulationTimestep(p_elapsed_seconds);
}

void iPhysics::addImpulseToObjectAtPoint(
		iPhysicsObject &physicsObject,					///< the object itself
		const CVector<3,float> &world_impulse_point,	///< intersection point in world space coordinates
		const CVector<3,float> &world_impulse			///< directed impulse
		)
{
	privateClass->addImpulseToObjectAtPoint(physicsObject, world_impulse_point, world_impulse);
}



void iPhysics::setUpdateInterval(double p_update_time_interval, double p_elapsed_seconds)
{
	privateClass->setUpdateInterval(p_update_time_interval, p_elapsed_seconds);
}

iRef<iPhysicsObject> iPhysics::findPhysicsObjectByIdentifierString(std::string &identifier_string)
{
	return privateClass->findPhysicsObjectByIdentifierString(identifier_string);
}

void iPhysics::detectAndResolveInterpenetrations()
{
	privateClass->detectAndResolveInterpenetrations();
}

std::list<CPhysicsCollisionData> iPhysics::getCollisions()
{
	return privateClass->collisions;
}