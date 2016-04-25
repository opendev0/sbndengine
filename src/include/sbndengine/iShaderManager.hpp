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

#ifndef __ISHADERMANAGER_HPP
#define __ISHADERMANAGER_HPP

#include <map>
#include <fstream>

#include <GL/gl.h>

struct shaderComponent {
    GLuint vertexShader;
    GLuint fragmentShader;
};

class iShaderManager
{
private:
    std::map<GLuint, shaderComponent> shaders;
    
public:
    iShaderManager() {};
    /**
     * \todo Delete shaders
     */
	virtual ~iShaderManager() {};
    
    /**
     * \brief Compiles and creates a shader program
     */
    bool createProgram(const char* vertexShaderFilename,
    		const char* fragmentShaderFilename,
    		GLuint &program);
    
private:
    /**
     * \brief Loads and compiles a shader from a file
     */
    static bool loadCompileShader(const char* filename, GLuint shader);

    /**
     * \brief Return the size of a file
     */
    static unsigned long getFileSize(std::ifstream &file);

    /**
     * \brief Prints to log information of a shader to stderr
     */
    static void printInfo(GLuint object, bool isShader = true);
};

#endif // __ISHADERMANAGER_HPP
