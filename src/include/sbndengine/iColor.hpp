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

#ifndef __I_COLOR_HPP__
#define __I_COLOR_HPP__

/**
 * simple color container
 */
class iColorRGBA
{
public:
	float color[4];

	iColorRGBA()
	{
		color[0] = color[1] = color[2] = color[3] = 1.0;
	}

	iColorRGBA(float r, float g, float b, float a)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
	}

	iColorRGBA(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = 1.0;
	}

	void set(float r, float g, float b, float a)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
	}

	void set(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = 1.0;
	}
};


class iColorRGB
{
	float color[3];

	void set(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
};



#endif
