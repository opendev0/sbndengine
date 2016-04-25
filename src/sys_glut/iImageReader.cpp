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

#include "sbndengine/graphics/iImageReader.hpp"

#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <jpeglib.h>
#include <png.h>

#include <iostream>
#include <fstream>
#include <string>

iImageReader::iImageReader()
{
    buffer = NULL;
    sizeX = 0;
    sizeY = 0;
}

iImageReader::~iImageReader()
{
    freeBuffer();
}

void iImageReader::readJPEGFromFile(const char *filename)
{
    // Clear old buffer
    freeBuffer();

    // Try opening jpeg file
    FILE *infile = fopen(filename, "rb");
    if (!infile)
    {
        std::cout << "Error opening jpeg file " << filename << "!\n";
        return;
    }

    // Create libjpeg structures
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // Specify input file
    jpeg_stdio_src(&cinfo, infile);

    // Read header information
    jpeg_read_header(&cinfo, true);

    // Set output parameters
    cinfo.out_color_space = JCS_RGB;

    // Start decompression
    jpeg_start_decompress(&cinfo);

    // Get size
    sizeX = cinfo.output_width;
    sizeY = cinfo.output_height;
    assert(cinfo.output_components == 3);

    // Allocate buffer
    buffer = new unsigned char[sizeX * sizeY * 3];

    // Decompress the image
    JSAMPROW row_pointer;
    while (cinfo.output_scanline < cinfo.image_height) {
    	row_pointer = &buffer[cinfo.output_scanline * sizeX * 3];

        jpeg_read_scanlines(&cinfo, &row_pointer, 1);
    }

    // Cleanup
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}

void iImageReader::readPNGFromFile(const char *filename)
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
    
    if ((fp = fopen(filename,"rb")) == NULL) {
        printf("Error opening output png file maybe texture path or filename is wrong %s\n!", filename );
        return;
    }
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (png_ptr == NULL) {
        fclose(fp);
        printf("Error could not allocate png ptr for png file");
        return;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        printf("Error could not allocate info struct for png file");
        return;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        
        /* Free all of the memory associated
         
         * with the png_ptr and info_ptr */
        
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        
        fclose(fp);
        
        /* If we get here, we had a
        
         * problem reading the file */
         printf("Error could not read the png file");
        return;
        
    }
    
    
    
    /* Set up the output control if
    
     * you are using standard C streams */
    
    png_init_io(png_ptr, fp);
    
    
    
    /* If we have already
     
     * read some of the signature */
    
    png_set_sig_bytes(png_ptr, sig_read);
    
    
    
    /*
     
     * If you have enough memory to read
     
     * in the entire image at once, and
     
     * you need to specify only
     
     * transforms that can be controlled
     
     * with one of the PNG_TRANSFORM_*
     
     * bits (this presently excludes
     
     * dithering, filling, setting
     
     * background, and doing gamma
     
     * adjustment), then you can read the
     
     * entire image (including pixels)
     
     * into the info structure with this
     
     * call
     
     *
     
     * PNG_TRANSFORM_STRIP_16 |
     
     * PNG_TRANSFORM_PACKING  forces 8 bit
     
     * PNG_TRANSFORM_EXPAND forces to
     
     *  expand a palette into RGB
     
     */;
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int colors = PNG_COLOR_TYPE_RGBA;
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
    //png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
    
    sizeX = width;
    
    sizeY = height;
    
    int channels;
    bool alpha;

    switch (color_type) {
            
        case PNG_COLOR_TYPE_RGBA:
            
            alpha = true;
            channels = 4;
            break;
            
        case PNG_COLOR_TYPE_RGB:
            
            alpha = false;
            channels = 3;
            
            break;
            
        default:
            
            std::cout << "Color type " << color_type << " not supported" << std::endl;
            
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            
            fclose(fp);
            
            return ;

    }

    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    //delete old buffer
    freeBuffer();
    
    buffer = (unsigned char*) malloc(width * height * bit_depth * channels / 8);

    png_bytep* rowPtrs = NULL;
    //Here's one of the pointers we've defined in the error handler section:
    //Array of row pointers. One for every row.
    rowPtrs = new png_bytep[height];
    
    //Alocate a buffer with enough space.
    //(Don't use the stack, these blocks get big easilly)
    //This pointer was also defined in the error handling section, so we can clean it up on error.
    //buffer = new char[width * height * bit_depth * channels / 8];
    //This is the length in bytes, of one row.
    const unsigned int stride = width * bit_depth * channels / 8;
    
    //A little for-loop here to set all the row pointers to the starting
    //Adresses for every row in the buffer
    
    for (size_t i = 0; i < height; i++) {
        //Set the pointer to the data pointer + i times the row stride.
        //Notice that the row order is reversed with q.
        //This is how at least OpenGL expects it,
        //and how many other image loaders present the data.
        png_uint_32 q = (height- i - 1) * stride;
        rowPtrs[i] = (png_bytep)buffer + q;
    }

    /*
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    

    for (int i = 0; i < height; i++) {

        // note that png is ordered top to

        // bottom, but OpenGL expect it bottom to top

        // so the order or swapped

        memcpy(buffer+(row_bytes * (width-1-i)), row_pointers[i], row_bytes);
    }
  
    /* Clean up after the read,

     * and free any memory allocated */
    png_read_image(png_ptr, rowPtrs);
    delete[] (png_bytep)rowPtrs;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    /* Close the file */
    fclose(fp);
    /* That's it */

    return;
}
