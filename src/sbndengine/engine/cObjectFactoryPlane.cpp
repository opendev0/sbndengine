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

#include "sbndengine/engine/cObjectFactoryPlane.hpp"


CMatrix3<float> cObjectFactoryPlane::getRotationalInertia()
{
	float v = (1.0f/12.0f)*mass;
	float size_y = 0.1f;

	return CMatrix3<float>(
			v*(size_y*size_y + size_z*size_z),0, 0,
			0, v*(size_x*size_x + size_z*size_z), 0,
			0, 0, v*(size_x*size_x + size_y*size_y)
			);
}

float cObjectFactoryPlane::getInverseMass()
{
	return inv_mass;
}

void cObjectFactoryPlane::setInverseMass(float p_inv_mass)
{
	inv_mass = p_inv_mass;
	mass = 1.0f/inv_mass;
}

cObjectFactoryPlane::cObjectFactoryPlane(
		float p_size_x,
		float p_size_z
	)
{
	setInverseMass(1.0);
	resizePlane(p_size_x, p_size_z);

	type = TYPE_PLANE;
	original_factory_ptr = this;
}

void cObjectFactoryPlane::resizePlane(
		float p_size_x,
		float p_size_z
	)
{
	size_x = p_size_x;
	size_z = p_size_z;

	mass = 1.0f;
	inv_mass = 1.0f;

	int segments_x = 10;
	int segments_z = 10;

	// resize vertex list, 3 triangles
	resizeTriangleList(2*segments_x*segments_z);

	// compute size of one small plate
	float psize_x = size_x / (float)segments_x;
	float psize_z = size_z / (float)segments_z;

	float phsize_x = psize_x*0.5f;
	float phsize_z = psize_z*0.5f;

	CVector<3,float> v000(-phsize_x, 0, -phsize_z);
	CVector<3,float> v100(phsize_x, 0, -phsize_z);
	CVector<3,float> v001(-phsize_x, 0, phsize_z);
	CVector<3,float> v101(phsize_x, 0, phsize_z);

	int triangle_iter = 0;

	for (int seg_z = 0; seg_z < segments_z; seg_z++)
	{
		float z = -phsize_z*(float)(segments_z-1) + psize_z*(float)seg_z;

		for (int seg_x = 0; seg_x < segments_x; seg_x++)
		{
			float x = -phsize_x*(float)(segments_x-1) + psize_x*(float)seg_x;

			setTriangleDataVT(triangle_iter,
					CVector<3,float>(-phsize_x+x, 0, phsize_z+z),
					CVector<3,float>(phsize_x+x, 0, phsize_z+z),
					CVector<3,float>(phsize_x+x, 0, -phsize_z+z),
					CVector<2,float>(((float)(seg_x)/(float)segments_x), ((float)(seg_z+1)/(float)segments_z)),
					CVector<2,float>(((float)(seg_x+1)/(float)segments_x), ((float)(seg_z+1)/(float)segments_z)),
					CVector<2,float>(((float)(seg_x+1)/(float)segments_x), ((float)(seg_z)/(float)segments_z))
					);	triangle_iter++;

			setTriangleDataVT(triangle_iter,
					CVector<3,float>(phsize_x+x, 0, -phsize_z+z),
					CVector<3,float>(-phsize_x+x, 0, -phsize_z+z),
					CVector<3,float>(-phsize_x+x, 0, phsize_z+z),
					CVector<2,float>(((float)(seg_x+1)/(float)segments_x), ((float)(seg_z)/(float)segments_z)),
					CVector<2,float>(((float)(seg_x)/(float)segments_x), ((float)(seg_z)/(float)segments_z)),
					CVector<2,float>(((float)(seg_x)/(float)segments_x), ((float)(seg_z+1)/(float)segments_z))
					);	triangle_iter++;
		}
	}

	setTexcoordsValid(true);

	computeTriangleFlatNormals();
	setNormalsValid(true);

	setupBoundingSphereRadius();
}
