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

#ifndef __I_OBJECT_HPP__
#define __I_OBJECT_HPP__

#include "iObjectFactory.hpp"
#include "sbndengine/graphics/iGraphicsMaterial.hpp"
#include "sbndengine/iBase.hpp"
#include "sbndengine/iRef.hpp"
#include "libmath/CMatrix.hpp"
#include "libmath/CVector.hpp"
#include "libmath/CQuaternion.hpp"
#include <string>


/**
 * \brief storage class for generic objects
 *
 * before an object can be used by the graphics or physics engine, it has to
 * be created and loaded.
 *
 * first, the object is created from an object factory (iObjectFactory).
 * then this object can be used by the graphics and physics engine while the
 * shared information of both systems is stored in this object (the position
 * as well as the rotation of the object).
 */
class iObject	: public iBase
{
private:
	void init();

public:
	/**
	 * true, if intersections with e. g. the mouse are allowed to be computed
	 */
	bool intersections_computable;

	/**
	 * object state
	 */
	CVector<3,float> position;
	CQuaternion<float> rotation;

	void updateModelMatrix();

	// model matrix (based on "translation", "rotation")
	CMatrix4<float> model_matrix;
	CMatrix4<float> inverse_model_matrix;

	// identifier string for convenience (e. g. to print the object's name if clicked with the mouse)
	std::string identifier_string;
	
	// true, if the player is allowed to touch this object
	bool touchable;
	
	// a pointer which can be used by the physics engine
	void *physics_engine_ptr;

	// a pointer which can be used by the graphics engine
	void *graphics_engine_ptr;

	iRef<iObjectFactory> objectFactory;

	iObject();
	iObject(const std::string &p_identifier_string);
	virtual ~iObject();

	void createFromFactory(
			iObjectFactory &p_objectFactory,
			iGraphicsMaterial &p_material
		);

	void createFromFactory(
			iObjectFactory &p_objectFactory
		);

	void hardTransform(CMatrix4<float> &m);

	void translate(float x, float y, float z);
	void translate(const CVector<3,float> &translation);
	void setPosition(float x, float y, float z);
	void setPosition(const CVector<3,float> &position);

	void rotate(const CVector<3,float> &axis, float angle);
	void rotate(const CVector<3,float> &p_angular_rotation);
	void rotate(const CQuaternion<float> &rotation);

	void setRotation(const CVector<3,float> &axis, float angle);
	void setRotation(float x, float y, float z, float angle);
	void setRotation(const CQuaternion<float> &p_rotation);

	void setIntersectionsComputable(bool p_computable);
	void setTouchable(bool p_touchable);

	void setOnlyVisualization();
	void unsetOnlyVisualization();
};

#endif //__I_OBJECT_HPP__
