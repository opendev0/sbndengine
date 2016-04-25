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

#include <stdarg.h>
#include <stdio.h>

#include <GL/glut.h>
#include <GL/gl.h>

#include <iostream>
#include "sbndengine/iText.hpp"
#include "CGlError.hpp"


iText::iText()
{

}

void iText::printfxy(
		float x,
		float y,
		const char *format,
		...)
{
	va_list list;
	static char text[2048];

	CGlErrorCheck();

	va_start(list, format);
	
#if defined(WIN32) || defined(_WIN32)
	vsnprintf_s(text, 2048, sizeof(text), format, list);
#else
	vsnprintf(text, sizeof(text), format, list);
#endif
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1,-1,0);
	glTranslatef(0.0f, -(float)viewport[3], 0.0f);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, (float)viewport[2], 0.0f, (float)viewport[3], -1.0f, 1.0f);

	CGlErrorCheck();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(x, y);
	for (char *c = text; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	CGlErrorCheck();
}


void iText::printfScreenxy(
		float x,
		float y,
		const char *format,
		...)
{
	va_list list;
	static char text[2048];

	CGlErrorCheck();

	va_start(list, format);
#if defined(WIN32) || defined(_WIN32)
	vsnprintf_s(text, sizeof(text), format, list);
#else
	vsnprintf(text, sizeof(text), format, list);
#endif

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	CGlErrorCheck();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);
	glRasterPos2f(x, y);
	for (char *c = text; *c != '\0'; c++)
	{
		if (*c == '\n')
		{
			y -= (14.0f*2.0f)/viewport[3];
			glRasterPos2f(x, y);
			continue;
		}
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	CGlErrorCheck();
}
