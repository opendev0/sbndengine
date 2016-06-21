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

#include "sbndengine/graphics/iGraphics.hpp"
#include "sbndengine/graphics/iGraphicsObject.hpp"

void iGraphics::clear()
{
	objectList.clear();
	objectConnectorList.clear();
}



void iGraphics::addObject(const iRef<iGraphicsObject> &p_graphics_object)
{
	objectList.push_back(p_graphics_object);
}

void iGraphics::removeObject(const iRef<iObject> &p_object)
{
	for (std::list<iRef<iGraphicsObject> >::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if ((*it)->object == p_object) {
			objectList.erase(it);
			return;
		}
	}
}

void iGraphics::addObjectConnector(const iRef<iGraphicsObjectConnector> &p_graphics_object_connector)
{
	objectConnectorList.push_back(p_graphics_object_connector);
}


void iGraphics::drawFrame(iCamera &p_camera)
{
	setupCamera(p_camera);
	setupLight();

	for (std::list<iRef<iGraphicsObject> >::iterator i = objectList.begin(); i != objectList.end(); i++)
	{
		iGraphicsObject &go = **i;
		if (go.visible)
			drawObject(*go.object, *go.material);
	}

	for (std::list<iRef<iGraphicsObjectConnector> >::iterator i = objectConnectorList.begin(); i != objectConnectorList.end(); i++)
	{
		iGraphicsObjectConnector &goc = **i;

		if (goc.visibility)
		{
			CVector<3,float> start_point, end_point;
			goc.getStartAndEndPoint(start_point, end_point);
			drawLine(	start_point,
						end_point,
						*goc.material
					);
		}
	}
}
