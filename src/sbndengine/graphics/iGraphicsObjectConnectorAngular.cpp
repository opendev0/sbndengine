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

#include "sbndengine/graphics/cGraphicsObjectConnectorAngular.hpp"


/**
 * constructor
 */
cGraphicsObjectConnectorAngular::cGraphicsObjectConnectorAngular(
		iRef<iObject> &p_object1,
		CVector<3,float> p_object_point1,
		iRef<iObject> &p_object2,
		CVector<3,float> p_object_point2,
		iRef<iGraphicsMaterial> &p_material,
		bool p_visibility
	)	:
		object1(p_object1),
		object_point1(p_object_point1),
		object2(p_object2),
		object_point2(p_object_point2)
{
	material = p_material;
	visibility = p_visibility;
}


void cGraphicsObjectConnectorAngular::setVisibility(bool p_visibility)
{
	visibility = p_visibility;
}



void cGraphicsObjectConnectorAngular::getStartAndEndPoint(CVector<3,float> &start_point, CVector<3,float> &end_point)
{
	start_point = object1->position + object1->model_matrix.getInverseTranspose3x3()*object_point1;
	end_point = object2->position + object2->model_matrix.getInverseTranspose3x3()*object_point2;
}
