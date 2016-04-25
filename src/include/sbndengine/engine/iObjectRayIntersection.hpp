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

#ifndef __I_OBJECT_RAY_INTERSECTION_HPP__
#define __I_OBJECT_RAY_INTERSECTION_HPP__

#include "sbndengine/iBase.hpp"
#include "iObject.hpp"
#include "iObjectRayIntersection.hpp"

/**
 * class to store the data for an intersection test between a ray and an object
 */
class iObjectRayIntersectionData	: public iBase
{
public:
	iRef<iObject> collidingObject;

	// triangle nr of intersection point
	int triangle_nr;

	// triangle coordinates
	float u, v;

	// intersection_point = start_pos + t * direction;
	float t;
};

class iObjectRayIntersection
{
private:
	float nearest_distance;

	CVector<3,float> world_start_pos;
	CVector<3,float> world_direction;

	class cPrivateObjectRayIntersectionData *privateObjectRayIntersectionData;

public:

	iRef<iObjectRayIntersectionData> intersectionData;

	/**
	 * setup new intersection test
	 */
	void setup(
			const CVector<3,float> &p_world_start_pos,
			const CVector<3,float> &p_world_direction
			);

	void computeRayIntersection(iObject &object);

	iObjectRayIntersection();
	virtual ~iObjectRayIntersection();
};

#endif // __I_OBJECT_RAY_INTERSECTION_HPP__
