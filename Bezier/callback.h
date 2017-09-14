/*
 * callback.h
 *
 *  Created on: 21-Aug-2017
 *      Author: sree
 */

#pragma once
const GLint WIDTH = 800, HEIGHT = 600;
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;

double x, y;
GLfloat x_pos, y_pos;
bool newPoint = false;
bool iterationInc = false;
bool iterationDec = false;
bool loop = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

void processKeys() {
	if (keys[GLFW_KEY_KP_ADD]) {
		iterationInc = true;
	}
	if (keys[GLFW_KEY_KP_SUBTRACT]) {
		iterationDec = true;
	}
	if (keys[GLFW_KEY_L]) {
		loop ? loop = false : loop = true;
	}
}

void DoMovement() {
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (GLFW_PRESS == action) {
			keys[key] = true;
			processKeys();
		} else if (GLFW_RELEASE == action) {
			keys[key] = false;
		}
	}
}

/*
 void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
 if (firstMouse) {
 lastX = xPos;
 lastY = yPos;
 firstMouse = false;
 }
 GLfloat xOffset = xPos - lastX;
 GLfloat yOffset = lastY - yPos;

 lastX = xPos;
 lastY = yPos;

 camera.ProcessMouseMovement(xOffset, yOffset);

 }
 */

void MouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

		x_pos = (x / WIDTH) * 2 - 1;
		y_pos = (y / HEIGHT) * -2 + 1;
		newPoint = true;

	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
	x = xPos;
	y = yPos;
}

