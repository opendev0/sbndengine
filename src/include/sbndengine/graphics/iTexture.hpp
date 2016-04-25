/*
 * iTexture.hpp
 *
 *  Created on: Feb 4, 2011
 *      Author: schreibm
 *  Modified on: Feb 14, 2013
 *      Author: rettenbs
 */

#ifndef ITEXTURE_HPP_
#define ITEXTURE_HPP_

#include "sbndengine/iBase.hpp"
#include "libmath/CMath.hpp"
#include <stdlib.h>

class iTexture : public iBase
{
	friend class iDraw3D;
private:
	class CTexturePrivateData *privateData;

public:
	iTexture();

	void textureFromRGBArray(int size_x, int size_y, unsigned char *texture_array);

	void createRandomizedTexture(
			float r, float r_max_variance,
			float g, float g_max_variance,
			float b, float b_max_variance,
			int size_x = 512, int size_y = 512
		);

	void createRandomizedTexture(
			float r, float g, float b,
			float variance,
			int size_x = 512, int size_y = 512
		);

	void createTextureFromFile(const char *filename);

	virtual ~iTexture();
};

#endif /* ITEXTURE_HPP_ */
