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

#include "sbndengine/engine/iObject.hpp"
#include "libmath/CGlSlMath.hpp"
#include <string.h>

void iObject::init()
{
	model_matrix.loadIdentity();
	physics_engine_ptr = NULL;
	graphics_engine_ptr = NULL;
	intersections_computable = true;
	touchable = true;
}


iObject::iObject()
{
	init();
}

iObject::iObject(const std::string &p_identifier_string)
{
	init();
	identifier_string = p_identifier_string;
}

iObject::~iObject()
{
//	std::cout << "OBJECT DELETE" << std::endl;
}

void iObject::createFromFactory(
			iObjectFactory &p_objectFactory
		)
{
	objectFactory = p_objectFactory;
}


void hardTransform(CMatrix4<float> &m)
{
	std::cerr << "TODO" << std::endl;
}

void iObject::translate(float x, float y, float z)
{
	position[0] += x;
	position[1] += y;
	position[2] += z;
}


void iObject::translate(const CVector<3,float> &p_translation)
{
	position += p_translation;
}


void iObject::setPosition(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}


void iObject::setPosition(const CVector<3,float> &p_position)
{
	position = p_position;
}


void iObject::rotate(const CVector<3,float> &i_axis, float i_angle)
{
	rotation.rotatePost(i_axis, i_angle);
}
void iObject::rotate(const CQuaternion<float> &i_quaternion_rotation)
{
	rotation *= i_quaternion_rotation;
}

void iObject::rotate(const CVector<3,float> &i_angular_rotation)
{
	rotation.applyAngularRotation(i_angular_rotation);
}

void iObject::setRotation(const CVector<3,float> &axis, float angle)
{
	rotation = CQuaternion<float>(axis, angle);
}

void iObject::setRotation(const CQuaternion<float> &p_rotation)
{
	rotation = p_rotation;
}

void iObject::updateModelMatrix()
{
	// the rotation is applied at first
	model_matrix = GLSL::translate(position)*rotation.getRotationMatrix();
	inverse_model_matrix = model_matrix.getInverse();
}

void iObject::setIntersectionsComputable(bool p_computable)
{
	intersections_computable = p_computable;
}

void iObject::setTouchable(bool p_touchable)
{
	touchable = p_touchable;
}