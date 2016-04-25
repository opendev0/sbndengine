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

#ifndef __I_TIME_HPP__
#define __I_TIME_HPP__

/**
 * simple method to get the time in seconds, fps and elapsed frame seconds
 * from the system
 */
class iTime
{
private:
	class cPrivateTime *privateTime;

public:
	// absolute seconds
	double elapsed_seconds;

	// seconds from last to current frame
	double frame_elapsed_seconds;

	// Simply the FPS
	double fps;

	iTime();
	virtual ~iTime();

	void setup();

	void update();

	void nanoSleep(double i_seconds);
};

#endif //__I_TIME_HPP__
