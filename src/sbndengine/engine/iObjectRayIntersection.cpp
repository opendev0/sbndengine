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

#include "sbndengine/engine/iObjectRayIntersection.hpp"

class cPrivateObjectRayIntersectionData
{
public:
	// prepare distance to point computations: (http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html)
	float inv_quad_denominator;
	CVector<3,float> x2;
};

void iObjectRayIntersection::setup(
			const CVector<3,float> &p_world_start_pos,
			const CVector<3,float> &p_world_direction
		)
{
	world_start_pos = p_world_start_pos;
	world_direction = p_world_direction;

	if (intersectionData.isNotNull())
		intersectionData.release();

	/**
	 * prepare bounding sphere test
	 *
	 * distance(line [x1,x2], point x0) = |(x_0 - x_1)x(x_0 - x_2)| / |x_2 - x_1|
	 */
	// set inv_quad_denominator to |x_2 - x_1|^2
	privateObjectRayIntersectionData->inv_quad_denominator = 1.0f/p_world_direction.getLength2();
	privateObjectRayIntersectionData->x2 = p_world_start_pos + p_world_direction;
}



void iObjectRayIntersection::computeRayIntersection(iObject &object)
{
	/**
	 * 1) we check for intersection with bounding sphere
	 */

	// distance(line [x1,x2], point x0) = |(x_0 - x_1)x(x_0 - x_2)| / |x_2 - x_1|

	CVector<3,float> x0 = object.position;
	float quad_distance = ((x0 - world_start_pos)%(x0 - privateObjectRayIntersectionData->x2)).getLength2() * privateObjectRayIntersectionData->inv_quad_denominator;

//	std::cout << object.identifier_string << "      " << x0 << " | " << quad_distance << " > " << object.objectFactory->quad_bounding_sphere_radius << std::endl;
	if (quad_distance > object.objectFactory->quad_bounding_sphere_radius)
		return;

	/**
	 * 2) compute ray start position in object space
	 */
	CVector<3,float> object_start_pos = object.model_matrix.getInverse()*world_start_pos;
	CVector<3,float> object_direction = world_direction*object.model_matrix;

	iObjectFactory &fac = object.objectFactory.getClass();

	float *v = fac.vertices;

	for (int triangle_nr = 0; triangle_nr < fac.triangles_count; triangle_nr++)
	{
		CVector<3,float> v0(v);	v+=3;
		CVector<3,float> v1(v);	v+=3;

		CVector<3,float> v2(v);	v+=3;

		CVector<3,float> e1 = v1 - v0;
		CVector<3,float> e2 = v2 - v0;

		CVector<3,float> ret;	// [u,v,t]

		CVector<3,float> p = object_direction % e2;
		float tmp = p.dotProd(e1);

		/*
		// in the case that the ray hits the back surface, don't compute intersection
		if ((e1 % e2).dotProd(object_direction) > 0)
			continue
		// this can be also checked by the following line with already computed values!
		*/
		if (tmp < 0)	continue;

		CVector<3,float> t;

		if (tmp > 0)
		{
			t = object_start_pos - v0;
		}
		else
		{
			t = v0 - object_start_pos;
			tmp = -tmp;
		}

		if (tmp < 0.0000000001f)	continue;

		// U
		float u = p.dotProd(t);
		if (u > tmp || u < 0.0)	continue;

		// V
		CVector<3,float> q = t % e1;
		float v = q.dotProd(object_direction);
		if (u + v > tmp || v < 0.0)	continue;

		// T
		float tt = q.dotProd(e2);

		if (tt < 0)	continue;

		float inv_tmp = 1.0f/tmp;
		tt *= inv_tmp;

		if (intersectionData.isNotNull())
		{
			// check if the intersection point is closer than the existing
			if (intersectionData->t < tt)
				continue;
		}
		else
		{
			intersectionData = new iObjectRayIntersectionData;
		}

		// reuse intersection data

		intersectionData->u = u*inv_tmp;
		intersectionData->v = v*inv_tmp;
		intersectionData->t = tt;
		intersectionData->collidingObject = &object;
		intersectionData->triangle_nr = triangle_nr;
	}

}

iObjectRayIntersection::iObjectRayIntersection()
{
	privateObjectRayIntersectionData = new cPrivateObjectRayIntersectionData;
}

iObjectRayIntersection::~iObjectRayIntersection()
{
	delete privateObjectRayIntersectionData;
}
