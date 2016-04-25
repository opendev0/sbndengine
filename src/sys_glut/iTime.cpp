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

#include "sbndengine/iTime.hpp"
#include "GL/freeglut.h"
#include <stdio.h>
#include <time.h>
#include <iostream>

class cPrivateTime
{
public:
	// timestamp for last fps value update
	double next_fps_time_update;
	double last_fps_time_update;
	double fps_frames;
};

iTime::iTime()
{
	privateTime = new cPrivateTime;
	setup();
}

iTime::~iTime()
{
	delete privateTime;
}

void iTime::setup()
{
	elapsed_seconds = (double)glutGet(GLUT_ELAPSED_TIME)*0.001;
	frame_elapsed_seconds = 0;
	fps = 0;
	privateTime->fps_frames = 0;
	privateTime->next_fps_time_update = 0;
	privateTime->last_fps_time_update = 0;
}

void iTime::update()
{
	double new_seconds = (double)glutGet(GLUT_ELAPSED_TIME)*0.001;

	frame_elapsed_seconds = new_seconds - elapsed_seconds;

	elapsed_seconds = new_seconds;

	privateTime->fps_frames++;

	if (privateTime->next_fps_time_update < elapsed_seconds)
	{
		// compute fps
		fps = privateTime->fps_frames/(elapsed_seconds-privateTime->last_fps_time_update);

		// set next fps time update for every second
		privateTime->next_fps_time_update += 1.0;

		// if the additional second of the last step is already behind (in this case the program runs really slow), use current time value+1
		if (privateTime->next_fps_time_update < elapsed_seconds)
			privateTime->next_fps_time_update = elapsed_seconds+1.0;

		privateTime->last_fps_time_update = elapsed_seconds;

		privateTime->fps_frames = 0;
	}
}



void iTime::nanoSleep(double i_seconds)
{
	timespec ts;
	ts.tv_sec = (time_t)i_seconds;
	ts.tv_nsec = (long)(i_seconds*1000000000.0);

	nanosleep(&ts, 0);
}
