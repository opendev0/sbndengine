/*
 * Copyright 2010 Martin Schreiber
 * Copyright 2013 Sebastian Rettenberger
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

#include "sbndengine/iRef.hpp"
#include "sbndengine/graphics/iGraphicsMaterial.hpp"
#include "sbndengine/iColor.hpp"
#include "libmath/CVector.hpp"

iGraphicsMaterial::iGraphicsMaterial()
{
	shininess = 100.0f;
}

void iGraphicsMaterial::setColor(const iColorRGBA &p_color)
{
	color = p_color;
}


void iGraphicsMaterial::setTexture(iRef<iTexture> &p_texture)
{
	texture = p_texture;
}

void iGraphicsMaterial::setNormalTexture(iRef<iTexture> &p_texture)
{
    normalTexture = p_texture;
}
