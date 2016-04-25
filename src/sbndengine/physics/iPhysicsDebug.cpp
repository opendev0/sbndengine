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

#include "sbndengine/physics/iPhysicsDebug.hpp"
#include "sbndengine/physics/iPhysics.hpp"
#include "cPhysicsEngine_Private.hpp"


void iPhysicsDebug::setup(iPhysics *p_physics)
{
	physics = p_physics;
	active = false;
	paused = false;
}


void iPhysicsDebug::start()
{
	active = true;
	paused = true;
	save_this_state = true;
}

void iPhysicsDebug::pause()
{
	paused = true;
}


void iPhysicsDebug::play()
{
	paused = false;
}


void iPhysicsDebug::stop()
{
	frames_state_list.clear();
	active = false;
}

void iPhysicsDebug::simulationTimestep(double p_elapsed_time_seconds)
{
	if (paused)
		return;

	if (save_this_state)
	{
		saveState();
		save_this_state = false;
	}

	if (physics->privateClass->simulationTimestep(p_elapsed_time_seconds))
		save_this_state = true;
}

void iPhysicsDebug::saveState()
{
	// store current state
	frames_state_list.push_back(std::list<iState>());

	std::list<iState> &state_list = frames_state_list.back();


	for (std::list<iRef<iPhysicsObject> >::iterator i1 = physics->privateClass->object_list.begin(); i1 != physics->privateClass->object_list.end(); i1++)
	{
		iPhysicsObject &o = **i1;

		state_list.push_back(iState());
		iState &s = state_list.back();

		if (o.object->identifier_string == "")
			std::cout << "WARNING: empty identifier string!" << std::endl;

		s.identifier_string = o.object->identifier_string;
		s.rotation = o.object->rotation;
		s.position = o.object->position;

		s.angular_velocity = o.angular_velocity;
		s.velocity = o.velocity;
		s.linear_acceleration_accumulator = o.linear_acceleration_accumulator;
		s.torque_accumulator = o.torque_accumulator;
	}
}

/*
 * to on for on frame - do one simulation step
 */
void iPhysicsDebug::forwardFrame()
{
	if (!active)	return;

	saveState();

	// update with the seconds elapsed during the last simulation step
	physics->privateClass->simulationTimestep(physics->privateClass->elapsed_time + physics->privateClass->simulation_timestep_size + 0.000000001);
}

/*
 * go back for one frame in the recordings
 */
void iPhysicsDebug::backwardFrame()
{
	if (!active)	return;

	if (frames_state_list.empty())
	{
		std::cerr << "ERROR: No more saved frames!!!" << std::endl;
		return;
	}

	std::list<iState> &state_list = frames_state_list.back();

	for (std::list<iState>::iterator i1 = state_list.begin(); i1 != state_list.end(); i1++)
	{
		iState &s = *i1;

		if (s.identifier_string == "")
		{
			std::cout << "WARNING: empty identifier string!" << std::endl;
			continue;
		}

		iRef<iPhysicsObject> po = physics->findPhysicsObjectByIdentifierString(s.identifier_string);

		if (po.isNull())
		{
			std::cout << "Object with identifier string " << s.identifier_string << " not found anymore!" << std::endl;
			continue;
		}
		po->object->rotation = s.rotation;
		po->object->position = s.position;

		po->angular_velocity = s.angular_velocity;
		po->velocity = s.velocity;
		po->linear_acceleration_accumulator = s.linear_acceleration_accumulator;
		po->object->updateModelMatrix();
	}


	// store current state
	frames_state_list.pop_back();


}
