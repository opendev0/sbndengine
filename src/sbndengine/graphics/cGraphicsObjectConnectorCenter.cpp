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

#include "sbndengine/graphics/cGraphicsObjectConnectorCenter.hpp"


/**
 * constructor
 */
cGraphicsObjectConnectorCenter::cGraphicsObjectConnectorCenter(
			iRef<iObject> &p_object1,
			iRef<iObject> &p_object2,
			iRef<iGraphicsMaterial> &p_material,
			bool p_visibility
		)	:
		object1(p_object1),
		object2(p_object2)
{
	material = p_material;
	visibility = p_visibility;
}


void cGraphicsObjectConnectorCenter::setVisibility(bool p_visibility)
{
	visibility = p_visibility;
}


void cGraphicsObjectConnectorCenter::getStartAndEndPoint(CVector<3,float> &start_point, CVector<3,float> &end_point)
{
	start_point = object1->position;
	end_point = object2->position;
}
