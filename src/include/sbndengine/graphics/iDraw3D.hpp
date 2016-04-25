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

#ifndef __I_DRAW3D_HPP__
#define __I_DRAW3D_HPP__

#include "libmath/CGlSlMath.hpp"
#include "sbndengine/iShaderManager.hpp"
#include "sbndengine/camera/iCamera.hpp"
#include "sbndengine/engine/iObject.hpp"
#include "sbndengine/graphics/iGraphicsMaterial.hpp"
#include "sbndengine/graphics/iGraphicsObject.hpp"

/**
 * \brief interface description for 3D rendering engine.
 *
 * this class is inherited to the iGraphics class to offer 3D drawing
 * abilities.
 */
class iDraw3D
{
	class cPrivateDataDraw3D *privateDataDraw3D;

	/** Loads and compiles the different shaders */
	iShaderManager shaderManager;

	/** Default shader for objects */
	GLuint defaultShader;

	/** Shader for objects with normal map */
	GLuint normalShader;
public:
	iDraw3D();
	virtual ~iDraw3D();

	/**
	 * main setup
	 *
	 * this should be called once for each engine initialization
	 */
	void setup();

	/**
	 * setup camera position
	 *
	 * called once per frame
	 */
	void setupCamera(iCamera &camera);

	/**
	 * setup light position
	 */
	void setupLight();

	/**
	 * call this function to start rendering an object
	 */
	void drawObject(iObject &object, iGraphicsMaterial &material);
	void drawObject(iGraphicsObject &object);

	void drawLine(
			const CVector<3,float> &p1,
			const CVector<3,float> &p2,
			const iRef<iGraphicsMaterial> &material
		);

	void clearBuffers();

	/**
	 * call this function if the window was resized
	 * this updates the viewport
	 */
	void windowResized(int width, int height);
};

#endif //__I_DRAW3D_HPP__
