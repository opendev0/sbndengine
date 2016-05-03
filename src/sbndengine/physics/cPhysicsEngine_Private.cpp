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


#include "cPhysicsEngine_Private.hpp"
#include "sbndengine/physics/iPhysicsObject.hpp"
#include "sbndengine/engine/cObjectFactorySphere.hpp"
#include "sbndengine/engine/cObjectFactoryPlane.hpp"
#include "sbndengine/engine/cObjectFactoryBox.hpp"
#include "libmath/CBinaryCNumbers.hpp"
#include "cPhysicsCollisionImpulse.hpp"
#include "worksheets_precompiler.hpp"


/**
 * there are different parameters available for the collision:
 */

/**
 * the relaxation parameter is an absolute value which specifies a distance which is added to each object as an extra distance
 */
//#define SOLVE_INTERPENETRATION_OVER_RELAXATION_PARAMETER	0.01f
//#define SOLVE_INTERPENETRATION_OVER_RELAXATION_PARAMETER	0.01f
//#define SOLVE_INTERPENETRATION_OVER_RELAXATION_PARAMETER	0.001f
//#define SOLVE_INTERPENETRATION_OVER_RELAXATION_PARAMETER	0.001f
#define SOLVE_INTERPENETRATION_OVER_RELAXATION_PARAMETER	0.0f

/**
 * the multiplier specifies how far the object is moved along the collision
 * normal.
 *
 * a value of 2 displaces the object to the position where it would be if it
 * bounced away with a coefficient of restitution of 1.
 *
 * also a value of 2 would place the object at a valid position if it's above
 * an object which stays still on the ground without being able to move in order
 * to solve the existing collision properly.
 *
 * if the opposite colliding object is e.~g. a floor, a value of 1 reduces
 * some flickering effects due to gravitation since the object is directly
 * placed on top of the floor.
 *
 * the best reason to set this value to 1 (which simulates a kind of
 * compressible object) is that this reduces also flickering effects when
 * putting boxes over each other next to a wall. then the box would separate,
 * collide with the wall, be bounced back and so on...
 */
//#define SOLVE_INTERPENETRATION_MULTIPLIER	1.0f
//#define SOLVE_INTERPENETRATION_MULTIPLIER	1.01f
#define SOLVE_INTERPENETRATION_MULTIPLIER	1.0f




void cPhysicsEngine_Private::reset()
{
	gravitation_vector = CVector<3,float>(0, -9.81f, 0);
	elapsed_time = -1;

	// set update interval to 50 times per second
	setUpdateInterval(1.0f/50.0f);

	setMaximumIterations(10, 10);
//	setMaximumIterations(5, 5);

	soft_constraint_list.clear();
	hard_constraint_list.clear();
	object_list.clear();
	list_colliding_objects.clear();
}


cPhysicsEngine_Private::cPhysicsEngine_Private()	:
		angular_damping_threshold(0.0005),
		angular_damping_factor(0.9)
{
	setUpdateInterval(1.0f/50.0f);
}


void cPhysicsEngine_Private::updateSoftConstraints()
{
	for (std::list<iRef<iPhysicsSoftConstraint> >::iterator i = soft_constraint_list.begin(); i != soft_constraint_list.end(); i++)
	{
		iPhysicsSoftConstraint &c = **i;
		c.updateAcceleration((float)simulation_timestep_size);
	}
}


void cPhysicsEngine_Private::emptyAndGetCollisions()
{
	list_colliding_objects.clear();

	/**
	 * first of all, we search for all collisions and store them into an array
	 */
	list_colliding_objects.push_front(CPhysicsCollisionData());
	CPhysicsCollisionData *cData = &list_colliding_objects.front();

	for (std::list<iRef<iPhysicsObject> >::iterator i1 = object_list.begin(); i1 != object_list.end(); i1++)
	{
		iPhysicsObject &o1 = **i1;

		std::list<iRef<iPhysicsObject> >::iterator i2 = i1;

		for (i2++; i2 != object_list.end(); i2++)
		{
			iPhysicsObject &o2 = **i2;

			/**
			 * first of all we check if the objects bounding spheres touch
			 */
			float quad_rad = o1.object->objectFactory->bounding_sphere_radius + o2.object->objectFactory->bounding_sphere_radius;
			quad_rad *= quad_rad;
			if ((o1.object->position - o2.object->position).getLength2() >= quad_rad)
				continue;

			/**
			 * next we compute any intersections based on the different kinds of objects
			 */

			if (!o1.isMovable() && !o2.isMovable())
				continue;

			if (CPhysicsIntersections::multiplexer(o1, o2, *cData))
			{
				list_colliding_objects.push_front(CPhysicsCollisionData());
				cData = &list_colliding_objects.front();
			}
		}
	}

	list_colliding_objects.pop_front();
}




void cPhysicsEngine_Private::getHardConstraintCollisions()
{
	/**
	 * iterate over all hard contraints (ropes, etc.)
	 */
	for (std::list<iRef<iPhysicsHardConstraint> >::iterator i = hard_constraint_list.begin(); i != hard_constraint_list.end(); i++)
	{
		iPhysicsHardConstraint &c = **i;

		CPhysicsCollisionData cData;

		if (c.updateHardConstraintsCollisions(cData))
			list_colliding_objects.push_back(cData);
	}
}

/**
 * WORKSHEET 2, ASSIGNMENT 3
 *
 * Resolve Interpenetrations
 */
void resolveInterpenetration_displace(CPhysicsCollisionData &c)
{
#if WORKSHEET_2
	float d2 = c.physics_object2->inv_mass / (c.physics_object2->inv_mass + c.physics_object1->inv_mass);
	c.physics_object1->object->translate(c.collision_normal * (d2 - 1) * c.interpenetration_depth);
	c.physics_object2->object->translate(c.collision_normal * d2 * c.interpenetration_depth);
	c.physics_object1->object->updateModelMatrix();
	c.physics_object2->object->updateModelMatrix();
#endif
}

/**
 * WORKSHEET 2, ASSIGNMENT 3
 *
 * Resolve Interpenetrations
 */
void cPhysicsEngine_Private::resolveInterpenetrations()
{
	CPhysicsCollisionData iter_cdata;

	// loop over all colliding objects computed during collision pass

	for (	std::list<CPhysicsCollisionData>::iterator i = list_colliding_objects.begin();
			i != list_colliding_objects.end();
			i++)
	{
		// WORKSHEET IMPLEMENTATION STARTS HERE
		CPhysicsCollisionData &c = *i;

		resolveInterpenetration_displace(c);

#if WORKSHEET_2
#ifdef NDEBUG
		// Check if collision was really solved
		float quad_rad = c.physics_object1->object->objectFactory->bounding_sphere_radius + c.physics_object2->object->objectFactory->bounding_sphere_radius;
		quad_rad *= quad_rad;
		
		if ((c.physics_object1->object->position - c.physics_object2->object->position).getLength2() < quad_rad) {
			if (CPhysicsIntersections::multiplexer(*c.physics_object1, *c.physics_object2, c)) {
				if (fabs(c.interpenetration_depth) > (1.0f/65536)) {
					std::cout << "Not resolved: " << c.physics_object1->object->identifier_string << " - " << c.physics_object2->object->identifier_string << " -> " << c.interpenetration_depth << std::endl;
				}
			}
		}
#endif
#endif
	}
}

void cPhysicsEngine_Private::updateConstantAcceleration()
{
	/**
	 * first of all, we apply the gravitational force to the objects
	 * this also initializes the acceleration for this simulation step
	 */
	for (std::list<iRef<iPhysicsObject> >::iterator i = object_list.begin(); i != object_list.end(); i++)
	{
		iPhysicsObject &o = **i;
		o.linear_acceleration_accumulator = gravitation_vector;
		o.torque_accumulator.setZero();
	}
}


void cPhysicsEngine_Private::setUpdateInterval(double p_update_time_interval, double p_elapsed_time)
{
	elapsed_time = p_elapsed_time;

	update_time_interval = p_update_time_interval;
	timestamp_for_next_update = elapsed_time + update_time_interval;
	simulation_timestep_size = update_time_interval;
}



/*
 * the seconds which elapsed during the last rendered frame are now stored to 'frame_elapsed_time'!
 */
bool cPhysicsEngine_Private::updateElapsedTime(double p_elapsed_time)
{
	if (elapsed_time < 0)
	{
		elapsed_time = p_elapsed_time;
		timestamp_for_next_update = elapsed_time;
		simulation_timestep_size = update_time_interval;
	}
	else
	{
	}

	elapsed_time = p_elapsed_time;

	if (update_time_interval >= 0.0)
	{
		if (timestamp_for_next_update > elapsed_time)
			return false;

		// set next time update for every second
		timestamp_for_next_update += update_time_interval;

		// if the additional second of the last step is already behind (in this case the program runs really slow), use current time `value+update_time_interval`
		if (timestamp_for_next_update < elapsed_time)
		{
/*
#ifndef NDEBUG
			std::cerr << "!!! Physic Engine time lack !!!" << std::endl;
#endif
*/
			timestamp_for_next_update = elapsed_time+update_time_interval;
		}

		simulation_timestep_size = update_time_interval;
	}
	else
	{
		simulation_timestep_size = p_elapsed_time - elapsed_time;
	}
	return true;
}

bool cPhysicsEngine_Private::simulationTimestep(double p_elapsed_time)
{
	// check if a physics update time-step is necessary
	if (!updateElapsedTime(p_elapsed_time))
		return false;


#if WORKSHEET_1
	updateConstantAcceleration();
#endif

#if WORKSHEET_3
	updateSoftConstraints();
#endif

#if WORKSHEET_1
	integrator();
#endif

#if WORKSHEET_2
	emptyAndGetCollisions();
#endif

#if WORKSHEET_3
	getHardConstraintCollisions();
#endif

#if WORKSHEET_3
	applyCollisionImpulse();
#endif


	int i = 1;
#if WORKSHEET_2
	while (!list_colliding_objects.empty() && i < max_global_collision_solving_iterations) {
		resolveInterpenetrations();
		emptyAndGetCollisions();
		i++;
	}
#endif

#if 1
#ifdef DEBUG
	if (i == max_global_collision_solving_iterations-1)
	{
		std::cout << "max global collision iterations (" << max_global_collision_solving_iterations << ") reached!" << std::endl;
	}
#endif
#endif

	return true;
}



void cPhysicsEngine_Private::integrator()
{
	for (std::list<iRef<iPhysicsObject> >::iterator i = object_list.begin(); i != object_list.end(); i++)
	{
		iPhysicsObject &o = **i;


		if (!o.isMovable())
			continue;

/**
 * WORKSHEET 1
 */
#if WORKSHEET_1
		//float e1 = o.velocity.getLength2() / 2 + gravitation_vector[1] * o.object->position[1];

		//explicitEulerTimestep(o);
		explicitEulerTimestep2(o);

		//float e2 = o.velocity.getLength2() / 2 + gravitation_vector[1] * o.object->position[1];
		//std::cout << "Energy difference: " << 1.0 / o.inv_mass * (e2 - e1) << std::endl;

		// TODO Assertions for position, velocity and energy
#endif


/**
 * WORKSHEET 6
 */
#if WORKSHEET_6

#endif
		o.object->updateModelMatrix();
	}
}


void cPhysicsEngine_Private::applyCollisionImpulse()
{
	for (std::list<CPhysicsCollisionData>::iterator i = list_colliding_objects.begin(); i != list_colliding_objects.end(); i++)
	{
		CPhysicsCollisionData &c = *i;

		CPhysicsCollisionImpulse::applyCollisionImpulse(c, simulation_timestep_size);
	}
}


void cPhysicsEngine_Private::addImpulseToObjectAtPoint(
		iPhysicsObject &physicsObject,					///< the object itself
		const CVector<3,float> &world_impulse_point,	///< intersection point in world space coordinates
		const CVector<3,float> &world_impulse			///< directed impulse
		)
{
	CVector<3,float> world_lever_arm = world_impulse_point - physicsObject.object->position;
	CVector<3,float> world_lever_arm_normalized = world_lever_arm.getNormalized();

	physicsObject.velocity += world_lever_arm_normalized*(world_lever_arm.getNormalized().dotProd(world_impulse)*physicsObject.inv_mass);

	/*
	 * to apply the angular speed, first the change of angular impulse is computed.
	 * then the angular impulse in projected to the object space to apply the inverse inertia.
	 * after projecting back to world space, the change in angular speed is applied.
	 */
	CMatrix3<float> m = physicsObject.object->model_matrix;

	physicsObject.angular_velocity +=
			m.getInverseTranspose() *
			physicsObject.rotational_inverse_inertia *
			m.getTranspose() *
			(world_lever_arm % world_impulse);
}


void cPhysicsEngine_Private::setMaximumIterations(int p_max_global_iterations, int p_max_local_iterations)
{
	max_global_collision_solving_iterations = p_max_global_iterations;
	max_local_collision_solving_iterations = p_max_local_iterations;
}


iRef<iPhysicsObject> cPhysicsEngine_Private::findPhysicsObjectByIdentifierString(std::string &identifier_string)
{

	for (std::list<iRef<iPhysicsObject> >::iterator i1 = object_list.begin(); i1 != object_list.end(); i1++)
	{
		iPhysicsObject &po = **i1;
		if (po.object->identifier_string == identifier_string)
			return po;
	}
	return iRef<iPhysicsObject>();
}


void cPhysicsEngine_Private::detectAndResolveInterpenetrations()
{
	emptyAndGetCollisions();
	resolveInterpenetrations();
}

void cPhysicsEngine_Private::explicitEulerTimestep(iPhysicsObject &o) {
	o.object->translate(o.velocity * simulation_timestep_size);
	o.addSpeed(o.linear_acceleration_accumulator * simulation_timestep_size);
}

void cPhysicsEngine_Private::explicitEulerTimestep2(iPhysicsObject &o) {
	o.addSpeed(o.linear_acceleration_accumulator * simulation_timestep_size);
	o.object->translate(o.velocity * simulation_timestep_size);
}
