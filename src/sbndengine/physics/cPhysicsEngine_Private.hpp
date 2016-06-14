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

#ifndef CPHYSICS_ENGINE_PRIVATE_HPP
#define CPHYSICS_ENGINE_PRIVATE_HPP

#include <list>
#include "cPhysicsIntersections.hpp"
#include "sbndengine/physics/iPhysicsHardConstraint.hpp"
#include "sbndengine/physics/iPhysicsObject.hpp"
#include "sbndengine/physics/iPhysicsSoftConstraint.hpp"
#include "libmath/CVector.hpp"
#include "libmath/CMath.hpp"
#include <assert.h>



class cPhysicsEngine_Private
{
	friend class iPhysics;
	friend class iPhysicsDebug;

	/// the time interval when the next simulation step is done
	double update_time_interval;

	/// the timestamp when the next physic engine timestep is done
	double timestamp_for_next_update;

	/// current value of elapsed seconds
	double elapsed_time;

	/**
	 * maximum number of global iterations to solve the collisions
	 *
	 * this are the collisions after each round of interpenetration solving
	 * if all local iterations between objects have been solved
	 */
	int max_global_collision_solving_iterations;

	/**
	 * maximum number of local iterations to solve the collisions
	 *
	 * this are the maximum number of iterations to solve local
	 * collisions
	 */
	int max_local_collision_solving_iterations;

	std::list<CPhysicsCollisionData> list_colliding_objects;

    public: std::list<CPhysicsCollisionData> collisions;


	/**
	 * list with objects which are simulated with the physics engine
	 */
	std::list<iRef<iPhysicsObject> > object_list;

	/**
	 * list with soft contact constraints
	 */
	std::list<iRef<iPhysicsSoftConstraint> > soft_constraint_list;

	/**
	 * list with hard contact constraints
	 */
	std::list<iRef<iPhysicsHardConstraint> > hard_constraint_list;

	CVector<3,float> gravitation_vector;

	/**
	 * dampings
	 */
	double angular_damping_threshold;
	double angular_damping_factor;

	void explicitEulerTimestep(iPhysicsObject &);
	void explicitEulerTimestep2(iPhysicsObject &);

public:
	/// elapsed seconds in the last frame
	double simulation_timestep_size;

	cPhysicsEngine_Private();

	bool getCollisions_objectMultiplexer(iPhysicsObject &physics_object1, iPhysicsObject &physics_object2, CPhysicsCollisionData &collisionData);

	/**
	 * updates the soft constraints for all physical objects
	 */
	void updateSoftConstraints();


	/**
	 * load the collision dataset for the hard constraints
	 */
	void getHardConstraintCollisions();

	/**
	 * get a list of collisions
	 */
	void emptyAndGetCollisions();

	/**
	 * updates the acceleration data of all physical objects
	 */
	void updateConstantAcceleration();

	/**
	 * update the object position and rotations
	 */
	void integrator();

	/**
	 * apply the collision impulses
	 */
	void applyCollisionImpulse();

	/**
	 * resolve the interpenetration of objects
	 */
	void resolveInterpenetrations();

	void clear();

	/**
	 * reset physics engine.
	 *
	 * reset physical parameters and  delete all objects and datasets
	 */
	void reset();

	/**
	 * setup the update interval time
	 */
	void setUpdateInterval(
			double p_update_time_interval,	///< the fixed timevalue when a simulation step is done
			double p_elapsed_seconds = -1		///< the elapsed seconds so far
			);

	bool updateElapsedTime(double p_elapsed_time);

	/**
	 * do one timestep
	 *
	 * \param p_elapsed_seconds	elapsed seconds since start of simulation
	 *
	 * \return	true, if a simulation step was done
	 */
	bool simulationTimestep(double p_elapsed_seconds);

	void setMaximumIterations(int p_max_global_iterations, int p_max_local_iterations);

	void addImpulseToObjectAtPoint(
			iPhysicsObject &physicsObject,					///< the object itself
			const CVector<3,float> &world_impulse_point,	///< intersection point in world space coordinates
			const CVector<3,float> &world_impulse			///< directed impulse
			);


	iRef<iPhysicsObject> findPhysicsObjectByIdentifierString(std::string &identifier_string);


	void detectAndResolveInterpenetrations();
};

#endif
