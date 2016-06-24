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

#include "sbndengine/engine/cObjectFactoryBox.hpp"


CMatrix3<float> cObjectFactoryBox::getRotationalInertia()
{
	float v = (1.0f/12.0f)*mass;

	return CMatrix3<float>(
			v*(size[1]*size[1] + size[2]*size[2]),0, 0,
			0, v*(size[0]*size[0] + size[2]*size[2]), 0,
			0, 0, v*(size[0]*size[0] + size[1]*size[1])
			);
}

float cObjectFactoryBox::getInverseMass()
{
	return inv_mass;
}

cObjectFactoryBox::cObjectFactoryBox(
		float p_size_x,
		float p_size_y,
		float p_size_z
	)
{
	// resize vertex list, 3 triangles
	resizeTriangleList(12);

	resizeBox(p_size_x, p_size_y, p_size_z);

	type = TYPE_BOX;
	original_factory_ptr = this;
}

void cObjectFactoryBox::resizeBox(
		float p_size_x,
		float p_size_y,
		float p_size_z
	)
{
	size[0] = p_size_x;
	size[1] = p_size_y;
	size[2] = p_size_z;

	mass = size[0]*size[1]*size[2];
	inv_mass = 1.0f/mass;

	half_size = size*0.5f;

	CVector<3,float> n001(0,0,1);
	CVector<3,float> n00x(0,0,-1);
	CVector<3,float> n010(0,1,0);
	CVector<3,float> n0x0(0,-1,0);
	CVector<3,float> n100(1,0,0);
	CVector<3,float> nx00(-1,0,0);

	CVector<3,float> v000(	-half_size[0],	-half_size[1],	-half_size[2]);
	CVector<3,float> v100(	half_size[0],	-half_size[1],	-half_size[2]);
	CVector<3,float> v010(	-half_size[0],	half_size[1],	-half_size[2]);
	CVector<3,float> v110(	half_size[0],	half_size[1],	-half_size[2]);
	CVector<3,float> v001(	-half_size[0],	-half_size[1],	half_size[2]);
	CVector<3,float> v101(	half_size[0],	-half_size[1],	half_size[2]);
	CVector<3,float> v011(	-half_size[0],	half_size[1],	half_size[2]);
	CVector<3,float> v111(	half_size[0],	half_size[1],	half_size[2]);

	CVector<2,float> t00(0, 0);
	CVector<2,float> t10(1, 0);
	CVector<2,float> t01(0, 1);
	CVector<2,float> t11(1, 1);

	// front
	setTriangleDataVT(0, v001, v101, v111, t00, t10, t01);
	setTriangleDataVT(1, v111, v011, v001, t11, t01, t10);

	// right
	setTriangleDataVT(2, v101, v100, v110, t00, t10, t01);
	setTriangleDataVT(3, v110, v111, v101, t11, t01, t10);

	// back
	setTriangleDataVT(4, v100, v000, v010, t00, t10, t01);
	setTriangleDataVT(5, v010, v110, v100, t11, t01, t10);

	// left
	setTriangleDataVT(6, v000, v001, v011, t00, t10, t01);
	setTriangleDataVT(7, v011, v010, v000, t11, t01, t10);

	// bottom
	setTriangleDataVT(8, v000, v100, v101, t00, t10, t01);
	setTriangleDataVT(9, v101, v001, v000, t11, t01, t10);

	// top
	setTriangleDataVT(10, v110, v010, v111, t00, t10, t01);
	setTriangleDataVT(11, v011, v111, v010, t11, t01, t10);

	setTexcoordsValid(true);

	computeTriangleFlatNormals();
	setNormalsValid(true);

	setupBoundingSphereRadius();
}


/**
 * set the mass
 */
void cObjectFactoryBox::setMass(
		float p_mass
	)
{
	mass = p_mass;
	inv_mass = 1.0f/p_mass;
}


CVector<3, float> cObjectFactoryBox::getBoxHalfSize()
{
	return half_size;
}