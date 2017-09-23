/*
 * window.h
 *
 *  Created on: 14-Aug-2017
 *      Author: sree
 */

#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	GLint width, height;
	GLFWwindow* window;
	Window(GLchar*);
	Window(GLint, GLint, GLchar*);
	bool createWindow();
};

