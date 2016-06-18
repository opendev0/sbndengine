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

#ifndef __I_GRAPHICS_HPP__
#define __I_GRAPHICS_HPP__

#include "sbndengine/graphics/iGraphicsObject.hpp"
#include "sbndengine/graphics/iGraphicsObjectConnector.hpp"
#include "sbndengine/graphics/iDraw3D.hpp"
#include "sbndengine/iRef.hpp"
#include <list>

/**
 * \brief graphic objects abstraction layer of the 3d engine
 *
 * this class stores all objects which should be rendered and cares about
 * the rendering itself.
 */
class iGraphics	: public iDraw3D
{
	// list with objects to draw
	std::list<iRef<iGraphicsObject> > objectList;

	// list with objects to draw
	std::list<iRef<iGraphicsObjectConnector> > objectConnectorList;

public:
	void clear();

	void addObject(const iRef<iGraphicsObject> &p_graphics_object);
    void removeObject(const iRef<iObject> &p_graphics_object);

	void addObjectConnector(const iRef<iGraphicsObjectConnector> &p_graphics_object_connector);

	void drawFrame(iCamera &p_camera);
};

#endif
