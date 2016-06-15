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

#ifndef __I_EVENTS_HPP__
#define __I_EVENTS_HPP__

#include "iEventHandlers.hpp"

enum
{
	SBND_EVENT_KEY_BACKSPACE = 8,
	SBND_EVENT_KEY_ENTER = 13,
	SBND_EVENT_KEY_F1 = 0x101,
	SBND_EVENT_KEY_F2,
	SBND_EVENT_KEY_F3,
	SBND_EVENT_KEY_F4,
	SBND_EVENT_KEY_F5,
	SBND_EVENT_KEY_F6,
	SBND_EVENT_KEY_F7,
	SBND_EVENT_KEY_F8,
	SBND_EVENT_KEY_F9,
	SBND_EVENT_KEY_F10,
	SBND_EVENT_KEY_F11,
	SBND_EVENT_KEY_F12,
    SBND_EVENT_KEY_UP,
    SBND_EVENT_KEY_DOWN,
	SBND_EVENT_KEY_LEFT,
	SBND_EVENT_KEY_RIGHT,

	SBND_EVENT_KEY_UNKNOWN
};


/**
 * event handling class
 *
 * a window has to be created before events are registered
 */
class iEvents
{
public:
	iEvents();
	~iEvents();

	void startEventLoop();
	void leaveEventLoop();

	void setup(iEventHandlers &p_event_handler_class);
	void shutdown();
};

#endif //__I_EVENT_HPP__
