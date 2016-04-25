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

#ifndef __I_WINDOW_HPP__
#define __I_WINDOW_HPP__

/**
 * Window handler to create a single window which is capable be used
 * as opengl render surface
 *
 * currently, there may be only one such window!
 */
class iWindow
{
public:
	int width, height;
	int center_x, center_y;
	float aspect_ratio;

	iWindow();
	virtual ~iWindow();

	void init();

	void create(
			int width = 640,
			int height = 480,
			const char *title = ""
		);

	void destroy();

	void swapBuffers();

	void shutdown();

	void windowResized(int width, int height);

	void setTitle(const char *title);

	void setMousePosition(int x, int y);

	void hideMouse();
	void showMouse();

	class iWindowPrivate *windowPrivate;
};

#endif //__I_WINDOW_HPP__
