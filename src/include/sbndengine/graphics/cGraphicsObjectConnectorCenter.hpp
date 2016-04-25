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

#ifndef __C_GRAPHICS_OBJECT_CONNECTOR_CENTER_HPP__
#define __C_GRAPHICS_OBJECT_CONNECTOR_CENTER_HPP__

#include "sbndengine/engine/iObject.hpp"
#include "sbndengine/iBase.hpp"
#include "sbndengine/graphics/iGraphicsMaterial.hpp"
#include "sbndengine/graphics/iGraphicsObjectConnector.hpp"

/**
 * \brief visualize a connection between 2 objects by a line
 */
class cGraphicsObjectConnectorCenter	: public iGraphicsObjectConnector
{
public:
	iRef<iObject> object1;
	iRef<iObject> object2;


	cGraphicsObjectConnectorCenter(
			iRef<iObject> &p_object1,
			iRef<iObject> &p_object2,
			iRef<iGraphicsMaterial> &p_material,
			bool p_visibility = true
		);

	void setVisibility(bool p_visibility_flag);

	void getStartAndEndPoint(CVector<3,float> &start_point, CVector<3,float> &end_point);
};

#endif
