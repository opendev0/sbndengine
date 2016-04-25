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

#ifndef __I_EVENT_HANDLERS_HPP__
#define __I_EVENT_HANDLERS_HPP__


/**
 * event handlers which can be overridden by another class
 */
class iEventHandlers
{
public:
	virtual void mouseMove(int x, int y);
	virtual void mouseButtonDown(int button_nr);
	virtual void mouseButtonUp(int button_nr);

	virtual void keyPressed(int key);
	virtual void keyReleased(int key);

	virtual void drawFrame();

	virtual void windowClose();
	virtual void windowResized(int width, int height);
};

#endif //__I_EVENT_HPP__
