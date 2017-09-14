/*
 * shader.h
 *
 *  Created on: 14-Aug-2017
 *      Author: sree
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader {
public:
	GLuint Program;

	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
	void Use();
};



#endif /* SHADER_H_ */
