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

#include "sbndengine/engine/iObjectFactory.hpp"

void iObjectFactory::init()
{
	vertices = NULL;
	normals = NULL;
	texCoords = NULL;

	triangles_count = 0;

	normals_valid = false;
	texcoords_valid = false;

	bounding_sphere_radius = CMath<float>::inf();
}

void iObjectFactory::setupBoundingSphereRadius()
{
	float *v = vertices;
	quad_bounding_sphere_radius = 0.0;

	for (int i = 0; i < triangles_count*3; i++)
	{
		float tmp_quad_dist = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
		if (tmp_quad_dist > quad_bounding_sphere_radius)
			quad_bounding_sphere_radius = tmp_quad_dist;
		v+=3;
	}

	bounding_sphere_radius = CMath<float>::sqrt(quad_bounding_sphere_radius);
}

void iObjectFactory::setNormalsValid(bool valid)
{
	normals_valid = valid;
}

void iObjectFactory::setTexcoordsValid(bool valid)
{
	texcoords_valid = valid;
}

iObjectFactory::iObjectFactory()
{
	init();
}

void iObjectFactory::clear()
{
	delete[] vertices;
	delete[] normals;
	delete[] texCoords;
}

void iObjectFactory::resizeTriangleList(size_t size)
{
	clear();

	triangles_count = size;

	vertices = new float[size*3*3];
	normals = new float[size*3*3];
	texCoords = new float[size*2*3];
}


void iObjectFactory::setTriangleDataV(
		int id,
		const CVector<3,float> &vertex0,
		const CVector<3,float> &vertex1,
		const CVector<3,float> &vertex2
	)
{
	int idx = id*3*3;
	vertices[idx+0] = vertex0.data[0];
	vertices[idx+1] = vertex0.data[1];
	vertices[idx+2] = vertex0.data[2];

	idx += 3;
	vertices[idx+0] = vertex1.data[0];
	vertices[idx+1] = vertex1.data[1];
	vertices[idx+2] = vertex1.data[2];

	idx += 3;
	vertices[idx+0] = vertex2.data[0];
	vertices[idx+1] = vertex2.data[1];
	vertices[idx+2] = vertex2.data[2];
}


void iObjectFactory::setTriangleDataVT(
		int id,
		const CVector<3,float> &vertex0,
		const CVector<3,float> &vertex1,
		const CVector<3,float> &vertex2,
		const CVector<2,float> &texCoord0,
		const CVector<2,float> &texCoord1,
		const CVector<2,float> &texCoord2
	)
{
	int idx = id*3*3;
	int tidx = id*2*3;
	vertices[idx+0] = vertex0.data[0];
	vertices[idx+1] = vertex0.data[1];
	vertices[idx+2] = vertex0.data[2];
	texCoords[tidx+0] = texCoord0.data[0];
	texCoords[tidx+1] = texCoord0.data[1];

	idx += 3;
	tidx += 2;
	vertices[idx+0] = vertex1.data[0];
	vertices[idx+1] = vertex1.data[1];
	vertices[idx+2] = vertex1.data[2];
	texCoords[tidx+0] = texCoord1.data[0];
	texCoords[tidx+1] = texCoord1.data[1];

	idx += 3;
	tidx += 2;
	vertices[idx+0] = vertex2.data[0];
	vertices[idx+1] = vertex2.data[1];
	vertices[idx+2] = vertex2.data[2];
	texCoords[tidx+0] = texCoord2.data[0];
	texCoords[tidx+1] = texCoord2.data[1];
}

void iObjectFactory::setTriangleDataN(
		int id,
		const CVector<3,float> &normal0,
		const CVector<3,float> &normal1,
		const CVector<3,float> &normal2
	)
{
	int idx = id*3*3;
	normals[idx+0] = normal0.data[0];
	normals[idx+1] = normal0.data[1];
	normals[idx+2] = normal0.data[2];

	idx += 3;
	normals[idx+0] = normal1.data[0];
	normals[idx+1] = normal1.data[1];
	normals[idx+2] = normal1.data[2];

	idx += 3;
	normals[idx+0] = normal2.data[0];
	normals[idx+1] = normal2.data[1];
	normals[idx+2] = normal2.data[2];

}

void iObjectFactory::setTriangleDataVN(
		int id,
		const CVector<3,float> &vertex0,
		const CVector<3,float> &normal0,
		const CVector<3,float> &vertex1,
		const CVector<3,float> &normal1,
		const CVector<3,float> &vertex2,
		const CVector<3,float> &normal2
	)
{
	setTriangleDataV(id, vertex0, vertex1, vertex2);
	setTriangleDataN(id, normal0, normal1, normal2);
}

void iObjectFactory::computeTriangleFlatNormals()
{
	CVector<3,float> normal;
	for (int idx = 0; idx < triangles_count*3*3; idx += 3*3)
	{
		CVector<3,float> d1(vertices[idx+3]-vertices[idx+0], vertices[idx+4]-vertices[idx+1], vertices[idx+5]-vertices[idx+2]);
		CVector<3,float> d2(vertices[idx+3]-vertices[idx+6], vertices[idx+4]-vertices[idx+7], vertices[idx+5]-vertices[idx+8]);
		normal = (d2%d1).getNormalized();

		normals[idx+0] = normal[0];
		normals[idx+1] = normal[1];
		normals[idx+2] = normal[2];

		normals[idx+3] = normal[0];
		normals[idx+4] = normal[1];
		normals[idx+5] = normal[2];

		normals[idx+6] = normal[0];
		normals[idx+7] = normal[1];
		normals[idx+8] = normal[2];
	}
}

iObjectFactory::~iObjectFactory()
{
	clear();
}
