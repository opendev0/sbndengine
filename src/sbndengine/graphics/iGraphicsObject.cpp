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

#include "sbndengine/graphics/iGraphicsObject.hpp"

iGraphicsObject::iGraphicsObject(
		const iRef<iObject> &p_object,
		const iRef<iGraphicsMaterial> &p_material
	)	:
	object(p_object),
	material(p_material)
{
	object->graphics_engine_ptr = this;
	visible = true;
}


void iGraphicsObject::setVisible()
{
	visible = true;
}


void iGraphicsObject::unsetVisible()
{
	visible = false;
}
