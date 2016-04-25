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

#ifndef __I_OBJECT_FACTORY_HPP__
#define __I_OBJECT_FACTORY_HPP__

#include "libmath/CVector.hpp"
#include "libmath/CMatrix.hpp"
#include "sbndengine/iBase.hpp"

/**
 * \brief interface description for other primitives (box, plane, sphere)
 *
 * iObjectFactory is the interface description for the different kinds
 * of possible objects, describing how the object data has to be stored
 * for visualization and physics purposes.
 *
 * it handles the object with it's raw data only (vertex, normal and
 * texture information).
 *
 * to use this object, an iObject has to be created.
 */
class iObjectFactory	:
			public iBase
{
public:
	// we use enumeration since dynamic binding takes more time
	enum
	{
		TYPE_SPHERE,
		TYPE_PLANE,
		TYPE_BOX
//		TYPE_MESH	// not implemented yet
	};
	int type;

	// pointer to the original factory (e. g. iObjectFactoryBox)
	void *original_factory_ptr;

	float mass, inv_mass;

	iObjectFactory();
	virtual ~iObjectFactory();

	float *vertices;
	float *normals;
	float *texCoords;

	float bounding_sphere_radius;
	float quad_bounding_sphere_radius;

	int triangles_count;

	bool normals_valid;
	bool texcoords_valid;

	virtual CMatrix3<float> getRotationalInertia() = 0;
	virtual float getInverseMass() = 0;

	void setTriangleDataV(
			int id,
			const CVector<3,float> &vertex0,
			const CVector<3,float> &vertex1,
			const CVector<3,float> &vertex2
		);

	void setTriangleDataVT(
			int id,
			const CVector<3,float> &vertex0,
			const CVector<3,float> &vertex1,
			const CVector<3,float> &vertex2,
			const CVector<2,float> &texCoord0,
			const CVector<2,float> &texCoord1,
			const CVector<2,float> &texCoord2
		);

	void setTriangleDataN(
			int id,
			const CVector<3,float> &normal0,
			const CVector<3,float> &normal1,
			const CVector<3,float> &normal2
		);

	void setTriangleDataVN(
			int id,
			const CVector<3,float> &vertex0,
			const CVector<3,float> &normal0,
			const CVector<3,float> &vertex1,
			const CVector<3,float> &normal1,
			const CVector<3,float> &vertex2,
			const CVector<3,float> &normal2
		);

	void resizeTriangleList(size_t size);

	void setNormalsValid(bool valid);
	void setTexcoordsValid(bool valid);

	void computeTriangleFlatNormals();

	void setupBoundingSphereRadius();

	void init();
	void clear();
};

#endif //__I_OBJECT_FACTORY_HPP__
