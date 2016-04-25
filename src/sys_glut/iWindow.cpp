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

#include "sbndengine/iWindow.hpp"
#include "sbndengine/iBase.hpp"
#include "GL/freeglut.h"

extern int *global_pargc;
extern char **global_argv;

class iWindowPrivate	:
		public iBase
{
public:
	int windowID;

	static bool glut_initialized;
};

bool iWindowPrivate::glut_initialized = false;

iWindow::iWindow()
{
	windowPrivate = new iWindowPrivate;
}

void iWindow::init()
{
	if (!windowPrivate->glut_initialized)
	{
		glutInit(global_pargc, global_argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);

		windowPrivate->glut_initialized = true;
	}
}

void iWindow::create(int p_width, int p_height, const char *title)
{
	glutInitWindowSize(p_width, p_height);
	windowResized(p_width, p_height);
	windowPrivate->windowID = glutCreateWindow(title);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}


void iWindow::swapBuffers()
{
	glutSwapBuffers();
}

void iWindow::destroy()
{
	// will be automatically destroyed on glut exit
//	glutDestroyWindow(windowPrivate->windowID);
}

void iWindow::shutdown()
{
//	glutExit();
}


iWindow::~iWindow()
{
	delete windowPrivate;
}


void iWindow::windowResized(int p_width, int p_height)
{
	width = p_width;
	center_x = width/2;
	height = p_height;
	center_y = height/2;

	aspect_ratio = (float)height/(float)width;
}


void iWindow::setTitle(const char *title)
{
	glutSetWindowTitle(title);
}


void iWindow::setMousePosition(int x, int y)
{
	glutWarpPointer(x, y);
}

void iWindow::showMouse()
{
	glutSetCursor(GLUT_CURSOR_INHERIT);
}

void iWindow::hideMouse()
{
	glutSetCursor(GLUT_CURSOR_NONE);
}
