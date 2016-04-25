/*
 * Copyright 2012 Maximilian Jakasovic
 * Copyright 2013 Sebastian Rettenberger
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

/**
 * \brief Simple image reader for png or jpeg files into unsigned byte buffer
 */
class iImageReader
{
private:
	/** The image buffer */
    unsigned char* buffer;

    /** Number of pixels in x dimension */
    unsigned int sizeX;

    /** Number of pixels in y dimension */
    unsigned int sizeY;

public:
	iImageReader();
	virtual ~iImageReader();

    /**
     * \brief Reads an image from a file with libjpeg into the buffer
     */
	void readJPEGFromFile(const char* filename);

	/**
	 * \brief Reads an image from a file with libpng into the buffer
	 *
	 * \Warning: Not tested!! Needs refectoring!
	 */
	void readPNGFromFile(const char* filename);

    //void readImageFromFile(const char* filename);

    /**
     * \return A pointer the image buffer
     */
    unsigned char* getBuffer()
    {
    	return buffer;
    }

    /**
     * \return The width of the image
     */
    unsigned int getWidth()
    {
    	return sizeX;
    }

    /**
     * \return The height of the image
     */
    unsigned int getHeight()
    {
    	return sizeY;
    }

private:
    /**
     * Frees (deletes) the buffer
     */
    void freeBuffer()
    {
    	delete [] buffer;
    	buffer = 0L;
    }
};
