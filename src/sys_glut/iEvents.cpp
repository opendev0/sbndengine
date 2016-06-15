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

#include "sbndengine/iEvents.hpp"
#include "sbndengine/iEventHandlers.hpp"
#include <GL/freeglut.h>
#include <iostream>


/**
 * use static handler since glut can only setup callback functions for non-class members
 */
static iEventHandlers *global_event_handler_class = NULL;

void event_draw_frame(void)
{
	global_event_handler_class->drawFrame();
}
void event_window_resized(int width, int height)			{	global_event_handler_class->windowResized(width, height);	}
void event_window_close(void)								{	global_event_handler_class->windowClose();	}
void event_key_press(unsigned char key, int x, int y)		{	global_event_handler_class->keyPressed(key);	}
void event_key_released(unsigned char key, int x, int y)	{	global_event_handler_class->keyReleased(key);	}

int get_sbnd_keycode(int glut_special_key)
{
	switch(glut_special_key)
	{
	case GLUT_KEY_F1:	return SBND_EVENT_KEY_F1;
	case GLUT_KEY_F2:	return SBND_EVENT_KEY_F2;
	case GLUT_KEY_F3:	return SBND_EVENT_KEY_F3;
	case GLUT_KEY_F4:	return SBND_EVENT_KEY_F4;
	case GLUT_KEY_F5:	return SBND_EVENT_KEY_F5;
	case GLUT_KEY_F6:	return SBND_EVENT_KEY_F6;
	case GLUT_KEY_F7:	return SBND_EVENT_KEY_F7;
	case GLUT_KEY_F8:	return SBND_EVENT_KEY_F8;
	case GLUT_KEY_F9:	return SBND_EVENT_KEY_F9;
	case GLUT_KEY_F10:	return SBND_EVENT_KEY_F10;
	case GLUT_KEY_F11:	return SBND_EVENT_KEY_F11;
	case GLUT_KEY_F12:	return SBND_EVENT_KEY_F12;
    case GLUT_KEY_UP:   return SBND_EVENT_KEY_UP;
    case GLUT_KEY_DOWN: return SBND_EVENT_KEY_DOWN;
	case GLUT_KEY_LEFT:	return SBND_EVENT_KEY_LEFT;
	case GLUT_KEY_RIGHT: return SBND_EVENT_KEY_RIGHT;
	}

	return SBND_EVENT_KEY_UNKNOWN;
}

void event_special_press(int key, int x, int y)		{	global_event_handler_class->keyPressed(get_sbnd_keycode(key));	}
void event_special_released(int key, int x, int y)	{	global_event_handler_class->keyReleased(get_sbnd_keycode(key));	}


void event_idle()	{	glutPostRedisplay();	}

void event_mouse(int button, int state, int x, int y)
{
	if (state ==  GLUT_UP)
		global_event_handler_class->mouseButtonUp(button);
	else if (state ==  GLUT_DOWN)
		global_event_handler_class->mouseButtonDown(button);
}

void event_mouse_motion(int x, int y)
{
	global_event_handler_class->mouseMove(x, y);
}

iEvents::iEvents()
{
}

void iEvents::setup(iEventHandlers &p_event_handler_class)
{
	global_event_handler_class = &p_event_handler_class;

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	glutCloseFunc(event_window_close);
	glutDisplayFunc(event_draw_frame);
	glutReshapeFunc(event_window_resized);
	glutKeyboardFunc(event_key_press);
	glutKeyboardUpFunc(event_key_released);
	glutSpecialFunc(event_special_press);
	glutSpecialUpFunc(event_special_released);
	glutIdleFunc(event_idle);

	glutMouseFunc(event_mouse);
	glutMotionFunc(event_mouse_motion);
	glutPassiveMotionFunc(event_mouse_motion);
}

void iEvents::shutdown()
{
}

void iEvents::startEventLoop()
{
	glutMainLoop();
}

void iEvents::leaveEventLoop()
{
	glutLeaveMainLoop();
}


iEvents::~iEvents()
{
}
