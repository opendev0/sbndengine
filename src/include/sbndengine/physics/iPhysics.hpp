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

#ifndef __I_PHYSICS_HPP__
#define __I_PHYSICS_HPP__

#include "sbndengine/iRef.hpp"
#include "iPhysicsObject.hpp"
#include "iPhysicsSoftConstraint.hpp"
#include "iPhysicsHardConstraint.hpp"
#include <list>
#include "libmath/CVector.hpp"
#include "sbndengine/iTime.hpp"
#include "sbndengine/physics/iPhysicsDebug.hpp"

class iPhysicsDebug;

/**
 * \brief main physics interface
 *
 * this class represents the interface to the physics engine
 */
class iPhysics
{
	friend class iPhysicsDebug;

	class cPhysicsEngine_Private *privateClass;

public:
	class iPhysicsDebug debug;

	iPhysics();
	~iPhysics();

	/**
	 * add a new object to the physics engine
	 */
	void addObject(const iRef<iPhysicsObject> &physicsObject);

	/**
	 * removes an object from the physics engine
	 */
	void removeObject(const iRef<iPhysicsObject> &physicsObject);

	/**
	 * apply an impulse (e. g. some bullet which hits the object) to an object
	 */
	void addImpulseToObjectAtPoint(
			iPhysicsObject &physicsObject,			///< the object itself
			const CVector<3,float> &world_point,	///< intersection point in world space coordinates
			const CVector<3,float> &impulse			///< directed impulse
			);

	/**
	 * add a new soft constraint (e. g. a spring)
	 *
	 * the difference to hard constraints is, that the acceleration is modified.
	 * hard constraints modify the velocity by applying an impulse.
	 */
	void addSoftConstraint(const iRef<iPhysicsSoftConstraint> &physicsSoftConstraint);

	/**
	 * remove a soft constraint
	 */
	void removeSoftConstraint(const iRef<iPhysicsSoftConstraint> &physicsSoftConstraint);

	/**
	 * add a new hard constraint (e. g. a rod or rope)
	 */
	void addHardConstraint(const iRef<iPhysicsHardConstraint> &physicsHardConstraint);

	/**
	 * remove a soft constraint
	 */
	void removeHardConstraint(const iRef<iPhysicsHardConstraint> &physicsHardConstraint);

	/**
	 * set the gravitation vector
	 */
	void setGravitation(const CVector<3,float> &p_gravitation_vector);

	/**
	 * do one timestep
	 *
	 * \param p_elapsed_seconds	elapsed seconds since start of simulation
	 */
	void simulationTimestep(double p_elapsed_seconds);

	/**
	 * reset the whole class to a virgin state
	 */
	void reset();

	/**
	 * set maximum values for iterations
	 *
	 * global iterations are used to solve interpenetrations between all objects.
	 * local iterations are used to solve interpenetrations between a pair of objects.
	 */
	void setMaximumIterations(int p_max_global_iterations, int p_max_local_iterations);


	/**
	 * sets the time interval in seconds which have to be gone until one simulation step is done.
	 * set to zero to update depending on the fps
	 *
	 * \param p_update_time_interval	the time interval for one physic timestep
	 * \param p_elapsed_seconds			the elapsed seconds
	 */
	void setUpdateInterval(
			double p_update_time_interval,
			double p_elapsed_seconds
		);

	/**
	 * return a reference to the physics object if one exists with the given
	 * identifier string
	 */
	iRef<iPhysicsObject> findPhysicsObjectByIdentifierString(std::string &identifier_string);


	/**
	 * resolve all interpenetrations without applying any impulses
	 */
	void detectAndResolveInterpenetrations();
};
#endif
