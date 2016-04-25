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

#ifndef CPHYSICS_INTERSECTIONS_HPP
#define CPHYSICS_INTERSECTIONS_HPP

#include <list>
#include "sbndengine/physics/iPhysicsObject.hpp"
#include "cPhysicsCollisionData.hpp"

/*
 * in the case that the normal is computed by the 2 "intersection points"
 * (usually there's only one intersection point, but 2 on each object due
 * to the interpenetration), there's the problem that a normal cannot be
 * computed if the collision distance is very small and thus the
 * intersection points almost coincide. therefore a small "epsilon" is used
 * to overcome this limitation and allow a small interpenetration for such cases.
 *
 * this is not necessary for specific interpenetration cases - e. g.
 * the interpenetration between a sphere and a plane since the collision normal
 * is taken as the normal of a plane.
 *
 * however, using this restriction is necessary since the normal has to be
 * normalized by the collision distance. therefore a sufficient length
 * is unalterable.
 *
 * !!!! MIN_COLLISION_DISTANCE has to be LARGER THAN ZERO !!!!
 */
#define MIN_COLLISION_DISTANCE	0.00001


class CPhysicsIntersections
{
public:
	static bool multiplexer(iPhysicsObject &physics_object1, iPhysicsObject &physics_object2, CPhysicsCollisionData &collisionData);

	static bool sphereSphere(iPhysicsObject &o1, iPhysicsObject &o2, CPhysicsCollisionData &physicsCollision);
	static bool spherePlane(iPhysicsObject &o1, iPhysicsObject &o2, CPhysicsCollisionData &physicsCollision);
	static bool sphereBox(iPhysicsObject &o1, iPhysicsObject &o2, CPhysicsCollisionData &physicsCollision);
	static bool planePlane(iPhysicsObject &o1, iPhysicsObject &o2, CPhysicsCollisionData &physicsCollision);
	static bool planeBox(iPhysicsObject &o1, iPhysicsObject &o2, CPhysicsCollisionData &physicsCollision);
	static bool boxBox(iPhysicsObject &o1, iPhysicsObject &o2, CPhysicsCollisionData &physicsCollision);
};

#endif
