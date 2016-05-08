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

#define GL_GLEXT_PROTOTYPES

extern "C"{
#include "GL/freeglut.h"
#include "GL/gl.h"
// GLSLMath has to be included after gl.h!
}
#include "libmath/CGlSlMath.hpp"
#include "sbndengine/camera/iCamera.hpp"
#include "sbndengine/engine/iObject.hpp"
#include "sbndengine/graphics/iGraphicsMaterial.hpp"
#include "sbndengine/graphics/iTexture.hpp"
#include "CTexturePrivateData.hpp"
#include "sbndengine/graphics/iDraw3D.hpp"
#include "sbndengine/graphics/iGraphicsObject.hpp"
#include <iostream>
#include "CGlError.hpp"

#define SHADER_PATH "../src/shaders/"


class cPrivateDataDraw3D
{
public:
	CMatrix4<float> projection_matrix;
	CMatrix4<float> view_matrix;
};

iDraw3D::iDraw3D()
{
	privateDataDraw3D = new cPrivateDataDraw3D;
}


iDraw3D::~iDraw3D()
{
	delete privateDataDraw3D;
}

void iDraw3D::setup()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);

	// Load shaders
	shaderManager.createProgram(SHADER_PATH "default.vs.glsl", SHADER_PATH "default.fs.glsl", defaultShader);
	shaderManager.createProgram(SHADER_PATH "normal.vs.glsl", SHADER_PATH "normal.fs.glsl", normalShader);
}

void iDraw3D::setupCamera(iCamera &p_camera)
{
	CGlErrorCheck();

	glMatrixMode(GL_PROJECTION);
	GLfloat m[16];
	p_camera.projection_matrix.storeColMajorMatrix(m);
	glLoadMatrixf(m);

	privateDataDraw3D->projection_matrix = p_camera.projection_matrix;
	privateDataDraw3D->view_matrix = p_camera.view_matrix;

	CGlErrorCheck();
}

void iDraw3D::setupLight()
{
	CGlErrorCheck();

	glMatrixMode(GL_MODELVIEW);
	GLfloat m[16];
	privateDataDraw3D->view_matrix.storeColMajorMatrix(m);
	glLoadMatrixf(m);

	GLfloat lightPosition[]= { 2.0f, 4.0f, 9.0f, 1.0f };
	GLfloat lightDiffuse[]= { 0.6f, 0.7f, 1.0f, 1.0f };
	//GLfloat lightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightAmbient[]= { 0.1f, 0.1f, 0.1f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightSpecular);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 20);	// specular exponent

	glEnable(GL_LIGHT0);

	CGlErrorCheck();
}

void iDraw3D::drawObject(iObject &object, iGraphicsMaterial &material)
{
	// TODO: setup material
	drawObject(*(iGraphicsObject*)object.graphics_engine_ptr);
}

void iDraw3D::drawObject(iGraphicsObject &graphics_object)
{
	CGlErrorCheck();

	iObject &object = *graphics_object.object;

	glMatrixMode(GL_MODELVIEW);
	GLfloat m[16];
	(privateDataDraw3D->view_matrix*object.model_matrix).storeColMajorMatrix(m);
	glLoadMatrixf(m);

	// setup "material"
	bool texture_activated = false;

	glVertexPointer(3, GL_FLOAT, 0, object.objectFactory->vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	if (object.objectFactory->normals_valid)
	{
		glNormalPointer(GL_FLOAT, 0, object.objectFactory->normals);
		glEnableClientState(GL_NORMAL_ARRAY);
	}

	if (graphics_object.material.isNotNull())
	{
		if (graphics_object.material->texture.isNotNull())
		{
			if (object.objectFactory->texcoords_valid)
			{
				glTexCoordPointer(2, GL_FLOAT, 0, object.objectFactory->texCoords);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, graphics_object.material->texture->privateData->gl_TextureId);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glMaterialf(GL_FRONT, GL_SHININESS, graphics_object.material->shininess);

#if SHADERS == 1
				glDisable(GL_LIGHTING);

				if (graphics_object.material->normalTexture.isNotNull()) {
					// Set normal texture and use shader, if we have one
					glUseProgram(normalShader);

					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, graphics_object.material->normalTexture->privateData->gl_TextureId);

					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				} else {
					// No normal map, use default shader
					glUseProgram(defaultShader);
				}
#endif // SHADERS == 1

				texture_activated = true;
			}
		}
		else
		{
			glColor4fv(graphics_object.material->color.color);
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, object.objectFactory->triangles_count*3);

	glDisableClientState(GL_VERTEX_ARRAY);

	if (object.objectFactory->normals_valid)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (texture_activated)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

#if SHADERS == 1
		// Set normal texture and use shader, if we have one
		glEnable(GL_LIGHTING);
		glUseProgram(0);
#endif // SHADERS == 1

		glDisable(GL_TEXTURE_2D);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	CGlErrorCheck();
}

void iDraw3D::clearBuffers()
{
	glClearColor(0.7f, 0.8f, 0.9f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void iDraw3D::windowResized(int width, int height)
{
	glViewport(0, 0, width, height);
}


void iDraw3D::drawLine(
		const CVector<3,float> &p1,
		const CVector<3,float> &p2,
		const iRef<iGraphicsMaterial> &material
	)
{
	// setup "material"
	if (material.isNotNull())
	{
		glColor4fv(material->color.color);
	}

	glMatrixMode(GL_MODELVIEW);
	GLfloat m[16];
	privateDataDraw3D->view_matrix.storeColMajorMatrix(m);
	glLoadMatrixf(m);

	glBegin(GL_LINES);
	glVertex3fv(p1.data);
	glVertex3fv(p2.data);
	glEnd();
}
