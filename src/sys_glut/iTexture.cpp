

#include "sbndengine/graphics/iTexture.hpp"
#include "sbndengine/graphics/iImageReader.hpp"
#include "CTexturePrivateData.hpp"
#include "CGlError.hpp"
#include <stdlib.h>

bool CTexturePrivateData::glGenerateMipmapInitialized = false;
PFNGLGENERATEMIPMAPPROC CTexturePrivateData::glGenerateMipmap = NULL;
//PFNGLGENTEXTURESEXTPROC CTexturePrivateData::glGenTextures = NULL;
//PFNGLTEXIMAGE2D CTexturePrivateData::glGenTextures = NULL;

iTexture::iTexture()
{
	privateData = new CTexturePrivateData;

	glGenTextures(1, &(privateData->gl_TextureId));
	CGlErrorCheck();
}


void iTexture::textureFromRGBArray(int size_x, int size_y, unsigned char *texture_array)
{
	glBindTexture(GL_TEXTURE_2D, privateData->gl_TextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size_x, size_y, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_array);

	if (privateData->glGenerateMipmap)
		privateData->glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	CGlErrorCheck();
}

void iTexture::createRandomizedTexture(
			float r, float r_max_variance,
			float g, float g_max_variance,
			float b, float b_max_variance,
			int size_x, int size_y
)
{
	unsigned char *texture_data = new unsigned char[size_x*size_y*3];
	unsigned char *tex_ptr = texture_data;

	for (int y = 0; y < size_y; y++)
		for (int x = 0; x < size_x; x++)
		{
			tex_ptr[0] = CMath<int>::clamp((int)((r + ((2.0f*(float)rand())/(float)RAND_MAX - 1.0f)*r_max_variance)*255.f), 0, 255);
			tex_ptr[1] = CMath<int>::clamp((int)((g + ((2.0f*(float)rand())/(float)RAND_MAX - 1.0f)*g_max_variance)*255.f), 0, 255);
			tex_ptr[2] = CMath<int>::clamp((int)((b + ((2.0f*(float)rand())/(float)RAND_MAX - 1.0f)*b_max_variance)*255.f), 0, 255);
			tex_ptr += 3;
		}

	textureFromRGBArray(size_x, size_y, texture_data);

	delete []texture_data;
}


void iTexture::createRandomizedTexture(
			float r, float g, float b,
			float max_variance,
			int size_x, int size_y
)
{
	unsigned char *texture_data = new unsigned char[size_x*size_y*3];
	unsigned char *tex_ptr = texture_data;

	for (int y = 0; y < size_y; y++)
		for (int x = 0; x < size_x; x++)
		{
			float d = (2.0f*(float)rand()/(float)RAND_MAX-1.0f)*max_variance;
			tex_ptr[0] = CMath<int>::clamp((int)((r + d)*255.f), 0, 255);
			tex_ptr[1] = CMath<int>::clamp((int)((g + d)*255.f), 0, 255);
			tex_ptr[2] = CMath<int>::clamp((int)((b + d)*255.f), 0, 255);
			tex_ptr += 3;
		}

	textureFromRGBArray(size_x, size_y, texture_data);

	delete []texture_data;
}

void iTexture::createTextureFromFile(const char *filename)
{
    //currently only jpeg is supported
    iImageReader imgR;
    imgR.readJPEGFromFile(filename);

    if (imgR.getBuffer() == NULL) {
        //Something went wrong
        std::cerr << "Error texture not created from" << filename << "\n";
        return;
    }

    textureFromRGBArray(imgR.getWidth(), imgR.getHeight(), imgR.getBuffer());
}

iTexture::~iTexture()
{
	glDeleteTextures(1, &(privateData->gl_TextureId));
	CGlErrorCheck();
	delete privateData;
}

