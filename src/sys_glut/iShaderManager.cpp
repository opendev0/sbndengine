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

#define GL_GLEXT_PROTOTYPES

#include "sbndengine/iShaderManager.hpp"

#include "worksheets_precompiler.hpp"

#include <iostream>

bool iShaderManager::createProgram(
		const char *vertexShaderFilename,
		const char *fragmentShaderFilename,
		GLuint &program)
{
#if SHADERS == 1
    shaderComponent shaderComp;

    shaderComp.vertexShader = glCreateShader(GL_VERTEX_SHADER_ARB);
    shaderComp.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER_ARB);

    if (!loadCompileShader(vertexShaderFilename, shaderComp.vertexShader))
    	return false;
    if (!loadCompileShader(fragmentShaderFilename, shaderComp.fragmentShader))
    	return false;
    
    program = glCreateProgram();
    glAttachShader(program, shaderComp.vertexShader);
    glAttachShader(program, shaderComp.fragmentShader);
    
    glLinkProgram(program);
    
    GLint linkStatus = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        std::cerr << "Could not compile shader:" << std::endl;
    	printInfo(program, false);

        return false;
    }

    shaders[program] = shaderComp;

    return true;
#else // SHADERS == 1
    return false;
#endif // SHADERS == 1
}


bool iShaderManager::loadCompileShader(const char* filename, GLuint shader)
{
#if SHADERS == 1
	// Read the file into sourceBuffer
    std::ifstream file(filename, std::ifstream::in | std::ifstream::binary);
    GLint size = getFileSize(file);

    if (!file || size == 0) {
        std::cerr << "Could not load vertex shader, wrong path or empty file" << std::endl;
        return false;
    }

    GLchar *sourceBuffer = new GLchar[size];
    file.read(sourceBuffer, size);
    
    // Load the source to OpenGL and free local buffer
    glShaderSource(shader, 1, (const GLchar**) &sourceBuffer, &size);
    delete [] sourceBuffer;

    // Compile shader
    glCompileShader(shader);

    // Check compile status
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        std::cerr << "Could not compile shader:" << std::endl;
    	printInfo(shader);

        return false;
    }

    return true;
#else // SHADERS == 1
    return false;
#endif // SHADERS == 1
}

unsigned long iShaderManager::getFileSize(std::ifstream &file)
{
	// Error?
    if (!file.good())
    	return 0;

    // Get length
    file.seekg(0, std::ios::end);
    unsigned long len = file.tellg();

    // Clear error state end seek to the beginning
    file.clear();
    file.seekg(0, std::ios::beg);

    return len;
}

void iShaderManager::printInfo(GLuint object, bool isShader)
{
#if SHADERS == 1
	GLint logLength;
	if (isShader)
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
	else
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength == 0)
		return;

    char* msg = new char[logLength];
    if (isShader)
    	glGetShaderInfoLog(object, logLength, NULL, msg);
    else
    	glGetProgramInfoLog(object, logLength, NULL, msg);

    std::cerr << msg << std::endl;

    delete [] msg;
#endif // SHADERS == 1
}
