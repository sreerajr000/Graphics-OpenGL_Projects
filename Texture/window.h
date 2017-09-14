/*
 * window.h
 *
 *  Created on: 14-Aug-2017
 *      Author: sree
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	GLFWwindow *window;
	Window(GLint, GLint, GLchar*);
	bool createWindow();
};



#endif /* WINDOW_H_ */
