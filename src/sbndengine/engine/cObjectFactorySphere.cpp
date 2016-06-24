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

#include "sbndengine/engine/cObjectFactorySphere.hpp"


CMatrix3<float> cObjectFactorySphere::getRotationalInertia()
{
	float v = (2.0f/5.0f)*mass*radius*radius;
	return CMatrix3<float>(
			v, 0, 0,
			0, v, 0,
			0, 0, v	);
}

float cObjectFactorySphere::getInverseMass()
{
	return inv_mass;
}


cObjectFactorySphere::cObjectFactorySphere(
		float p_radius,
		int segments_horizontal,
		int segments_vertical
	)
{
	resizeSphere(p_radius, segments_horizontal, segments_vertical);

	type = TYPE_SPHERE;
	original_factory_ptr = this;
}

void cObjectFactorySphere::resizeSphere(
		float p_radius,
		int segments_horizontal,	/* left-right segments */
		int segments_vertical		/* number of up-down segments */
	)
{
	radius = p_radius;

	// setup mass and inverse mass
	mass = (4.0f/3.0f)*CMath<float>::PI()*radius*radius*radius;
	inv_mass = 1.0f/mass;

	int triangle_count = (segments_vertical-2)*segments_horizontal*2 + 2*segments_horizontal;

	// resize vertex list, 3 triangles
	resizeTriangleList(triangle_count);

	// create vertex data
	float *v = vertices;
	float *n = normals;
	float *t = texCoords;

	float scale;
	CVector<3,float> normal;

#define STORE(r, y, tx, ty)											\
			scale = CMath<float>::sin(y);					\
			v[0] = CMath<float>::sin(r)*scale*radius;		\
			v[1] = -CMath<float>::cos(y)*radius;			\
			v[2] = -CMath<float>::cos(r)*scale*radius;		\
			normal = CVector<3,float>(v[0], v[1], v[2]);	\
			normal.normalize();		\
			n[0] = normal[0];		\
			n[1] = normal[1];		\
			n[2] = normal[2];		\
			t[0] = tx;				\
			t[1] = ty;				\
			v += 3;					\
			n += 3;					\
			t += 2;

	for (int dbeta = 1; dbeta < segments_vertical-1; dbeta++)
	{
		float ty0 = (float)dbeta/(float)segments_vertical;
		float ty1 = (float)(dbeta+1)/(float)segments_vertical;
		float y0 = ty0 * CMath<float>::PI();
		float y1 = ty1 * CMath<float>::PI();

		for (int dalpha = 0; dalpha < segments_horizontal; dalpha++)
		{
			float tx0 = (float)dalpha/(float)segments_horizontal;
			float tx1 = (float)(dalpha+1)/(float)segments_horizontal;
			float r0 = tx0 * 2.0f * CMath<float>::PI();
			float r1 = tx1 * 2.0f * CMath<float>::PI();

			STORE(r1, y1, tx1, ty1);
			STORE(r1, y0, tx1, ty0);
			STORE(r0, y0, tx0, ty0);

			STORE(r0, y0, tx0, ty0);
			STORE(r0, y1, tx0, ty1);
			STORE(r1, y1, tx1, ty1);
		}
	}

	{
		float ty0 = 0;
		float ty1 = ((float)1/(float)segments_vertical);
		float y0 = 0;
		float y1 = ty1 * CMath<float>::PI();


		for (int dalpha = 0; dalpha < segments_horizontal; dalpha++)
		{
			float tx0 = (float)dalpha/(float)segments_horizontal;
			float tx1 = (float)(dalpha+1)/(float)segments_horizontal;
			float r0 = tx0 * 2.0f * CMath<float>::PI();
			float r1 = tx1 * 2.0f * CMath<float>::PI();

			STORE(r0, y1, tx0, ty1);
			STORE(r1, y1, tx1, ty1);
			STORE(r0, y0, tx0, ty0);
		}
	}

	{
		float ty0 = ((float)(segments_vertical-1)/(float)segments_vertical);
		float ty1 = 1;
		float y0 = ty0*CMath<float>::PI();
		float y1 = CMath<float>::PI();

		for (int dalpha = 0; dalpha < segments_horizontal; dalpha++)
		{
			float tx0 = (float)dalpha/(float)segments_horizontal;
			float tx1 = (float)(dalpha+1)/(float)segments_horizontal;
			float r0 = tx0 * 2.0f * CMath<float>::PI();
			float r1 = tx1 * 2.0f * CMath<float>::PI();

			STORE(r0, y1, tx0, ty1);
			STORE(r1, y0, tx1, ty0);
			STORE(r0, y0, tx0, ty0);
		}
	}

#undef STORE
	setNormalsValid(true);
	setTexcoordsValid(true);

	setupBoundingSphereRadius();
}

float cObjectFactorySphere::getRadius() 
{
	return radius;
}
