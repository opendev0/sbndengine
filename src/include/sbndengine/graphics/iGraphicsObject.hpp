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

#ifndef __I_GRAPHICS_OBJECT_HPP__
#define __I_GRAPHICS_OBJECT_HPP__

#include "sbndengine/iRef.hpp"
#include "sbndengine/engine/iObject.hpp"
#include "sbndengine/graphics/iGraphicsMaterial.hpp"


/*
 * \brief storage renderable graphics object
 *
 * to allow rendering of objects, they need two properties:
 * - a vertex dataset itself
 * - a material
 *
 * this class combines both things into a single class
 */
class iGraphicsObject	:	public iBase
{
public:
	iRef<iObject> object;

	// material data is referenced to existing material
	iRef<iGraphicsMaterial> material;

	bool visible;

	iGraphicsObject(
			const iRef<iObject> &p_object,
			const iRef<iGraphicsMaterial> &p_material
		);

	void setVisible();
	void unsetVisible();
};

#endif
