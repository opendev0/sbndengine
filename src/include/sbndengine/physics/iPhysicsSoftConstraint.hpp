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

#ifndef __I_PHYSICS_SOFT_CONSTRAINT_HPP__
#define __I_PHYSICS_SOFT_CONSTRAINT_HPP__

#include "sbndengine/iBase.hpp"


/**
 * \brief interface description for soft constraints like springs
 *
 * 2 soft constraints (apart from rigid body collisions) have been implemented so far:
 * cPhysicsSoftConstraintSpring
 * cPhysicsSoftConstraintSpringAngular
 */
class iPhysicsSoftConstraint	: public iBase
{
public:
	virtual void updateAcceleration(double frame_elapsed_seconds) = 0;
};

#endif
