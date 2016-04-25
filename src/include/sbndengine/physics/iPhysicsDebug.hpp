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

#ifndef __CPHYSICS_ENGINE_DEBUG_HPP__
#define __CPHYSICS_ENGINE_DEBUG_HPP__

#include "sbndengine/engine/iObject.hpp"
#include "sbndengine/physics/iPhysicsObject.hpp"
#include <list>

class iPhysics;

/**
 * \brief physics debug handler
 *
 * physics debugging is achieved by storing the physics parameters for every
 * frame into a dataset if physics debugging is activated.
 */
class iPhysicsDebug
{
	friend class iPhysicsEngine_Private;

	class iState
	{
	public:
		std::string identifier_string;

		CQuaternion<float> rotation;
		CVector<3,float> position;

		CVector<3,float> angular_velocity;
		CVector<3,float> velocity;

		CVector<3,float> linear_acceleration_accumulator;
		CVector<3,float> torque_accumulator;
	};

	std::list< std::list<iState> >	frames_state_list;

	iPhysics *physics;

	void saveState();

public:
	bool active;
	bool paused;
	bool save_this_state;

	void setup(iPhysics *p_physics);

	/*
	 * start physics debugging and pause the simulation
	 *
	 * after this call, all object states are logged to reverse the simulation
	 */
	void start();

	/*
	 * stop the physics debugging
	 */
	void stop();


	/*
	 * continue running the simulation after it was paused
	 */
	void play();

	/*
	 * pauses the simulation
	 */
	void pause();

	/*
	 * to on for on frame - do one simulation step
	 */
	void forwardFrame();

	/*
	 * go back for one frame in the recordings
	 */
	void backwardFrame();


	void simulationTimestep(double p_elapsed_time_seconds);
};

#endif
