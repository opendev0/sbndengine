#ifndef CTEXTURE_PRIVATE_DATA_HPP__
#define CTEXTURE_PRIVATE_DATA_HPP__


#if !defined(WIN32) && !defined(_WIN32)
	#include "GL/glx.h"
#else
	#include <windows.h>
#endif

#include <GL/gl.h>
#include "GL/glext.h"

class CTexturePrivateData
{
	friend class iDraw3D;
	static bool glGenerateMipmapInitialized;

public:
	static PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	GLuint gl_TextureId;

	CTexturePrivateData()
	{
		if (!glGenerateMipmapInitialized)
		{

#if defined(WIN32) || defined(_WIN32)
				glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
#else
				glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) glXGetProcAddress((GLubyte*)"glGenerateMipmap");
#endif
			if (glGenerateMipmap == NULL)
			{
				std::cerr << "WARNING: FAILED TO GET MIPMAP HANDLER" << std::endl;
			}

			glGenerateMipmapInitialized = true;
		}
	}
};

#endif	// CTEXTURE_PRIVATE_DATA_HPP__
