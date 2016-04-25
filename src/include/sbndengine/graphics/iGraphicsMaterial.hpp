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

#ifndef __I_GRAPHICS_MATERIAL_HPP__
#define __I_GRAPHICS_MATERIAL_HPP__

#include "sbndengine/iRef.hpp"
#include "sbndengine/iBase.hpp"
#include "sbndengine/iColor.hpp"
#include "libmath/CVector.hpp"
#include "sbndengine/graphics/iTexture.hpp"

/**
 * \brief
 */
class iGraphicsMaterial	: public iBase
{
public:
	iColorRGBA color;
	iRef<iTexture> texture;
	iRef<iTexture> normalTexture;
	float shininess;

	iGraphicsMaterial();

	void setColor(const iColorRGBA &p_color);

	void setTexture(iRef<iTexture> &p_texture);

	void setNormalTexture(iRef<iTexture> &p_texture);
};

#endif //__I_MATERIAL_HPP__
