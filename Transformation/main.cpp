/*
 * main.cpp
 *
 *  Created on: 21-Aug-2017
 *      Author: sree
 */

// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "window.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "callback.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

// Properties
GLchar *TITLE = "OpenGL Test";
glm::vec3 lightPos(0.0f, 1.0f, 2.0f);

int main(int argc, char *argv[]) {

	//Create Window
	Window window(WIDTH, HEIGHT, TITLE);
	if (!window.createWindow()) {
		return EXIT_FAILURE;
	}

	//Set Callbacks
	glfwSetKeyCallback(window.window, KeyCallback);
	glfwSetCursorPosCallback(window.window, MouseCallback);
	glfwSetMouseButtonCallback(window.window, MouseClickCallback);
	//glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Enable Z and Alpha
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Model suzanne("suzanne.obj");

	Shader shader("suzanne.vs", "suzanne.frag");

	glm::mat4 projection = glm::perspective(camera.GetZoom(),
			(GLfloat) WIDTH / (GLfloat) HEIGHT, 0.1f, 100.0f);

	glUniform1i( glGetUniformLocation(shader.Program, "material.diffuse"), 0);
	glUniform1i( glGetUniformLocation(shader.Program, "material.specular"), 1);
	//main event loop
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));



	while (!glfwWindowShouldClose(window.window)) {
		//DoMovement();
		processKeys();
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		GLint lightDirLoc = glGetUniformLocation(shader.Program,
				"light.direction");
		GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
		glUniform3f(lightDirLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y,
				camera.GetPosition().z);

		glUniform3f( glGetUniformLocation(shader.Program, "light.ambient"),
				0.5f, 0.5f, 0.5f);
		glUniform3f( glGetUniformLocation(shader.Program, "light.diffuse"),
				1.0f, 0.5f, 0.5f);
		glUniform3f( glGetUniformLocation(shader.Program, "light.specular"),
				0.5f, 0.5f, 0.5f);

		// Set material properties
		glUniform1f( glGetUniformLocation(shader.Program, "material.shininess"),
				32.0f);

		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv( glGetUniformLocation(shader.Program, "projection"),
				1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv( glGetUniformLocation(shader.Program, "view"), 1,
				GL_FALSE, glm::value_ptr(view));


		//Transform Matrices
		GLfloat r = (0.001f);
		if(rev){
			r = -0.001f;
		}
		GLfloat rX[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, glm::cos(r), glm::sin(
				r), 0.0f, 0.0f, -glm::sin(r), glm::cos(r), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		glm::mat4 rotateXMatrix = glm::make_mat4(rX);

		GLfloat rY[] = { glm::cos(r), 0.0f, -glm::sin(r), 0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, glm::sin(r), 0.0f, glm::cos(r), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		glm::mat4 rotateYMatrix = glm::make_mat4(rY);

		GLfloat rZ[] = { glm::cos(r), glm::sin(r), 0.0f, 0.0f, -glm::sin(r), glm::cos(r), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		glm::mat4 rotateZMatrix = glm::make_mat4(rZ);


		GLfloat tX[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, r, 0.0f, 0.0f, 1.0f};
		glm::mat4 translateXMatrix = glm::make_mat4(tX);

		GLfloat tY[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, r, 0.0f, 1.0f};
		glm::mat4 translateYMatrix = glm::make_mat4(tY);

		GLfloat tZ[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f, r, 1.0f};
		glm::mat4 translateZMatrix = glm::make_mat4(tZ);

		GLfloat sX[] = {1.0f + r, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		glm::mat4 scaleXMatrix = glm::make_mat4(sX);

		GLfloat sY[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f + r, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		glm::mat4 scaleYMatrix = glm::make_mat4(sY);

		GLfloat sZ[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f + r,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		glm::mat4 scaleZMatrix = glm::make_mat4(sZ);

		//Perform transformation
		if (rotateX) {
			rotateX = false;
			//model = glm::rotate(model, 0.001f, glm::vec3(1.0f, 0.0f, 0.0f));
			model = model * rotateXMatrix;
		}
		if (rotateY) {
			rotateY = false;
			//model = glm::rotate(model, 0.001f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = model * rotateYMatrix;
		}
		if (rotateZ) {
			rotateZ = false;
			//model = glm::rotate(model, 0.001f, glm::vec3(0.0f, 0.0f, 1.0f));
			model = model * rotateZMatrix;
		}
		if (translateX) {
			translateX = false;
			//model = glm::translate(model, glm::vec3(0.001f, 0.0f, 0.0f));
			model = model * translateXMatrix;
		}
		if (translateY) {
			translateY = false;
			//model = glm::translate(model, glm::vec3(0.0f, 0.001f, 0.0f));
			model = model * translateYMatrix;
		}
		if (translateZ) {
			translateZ = false;
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.001f));
			model = model * translateZMatrix;
		}
		if (scaleX) {
			scaleX = false;
			//model = glm::scale(model, glm::vec3(1.001f, 1.0f, 1.0f));
			model = model * scaleXMatrix;
		}
		if (scaleY) {
			scaleY = false;
			//model = glm::scale(model, glm::vec3(1.0f, 1.001f, 1.0f));
			model = model * scaleYMatrix;
		}
		if (scaleZ) {
			scaleZ = false;
			//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.001f));
			model = model * scaleZMatrix;
		}

		// Draw the loaded model

		glUniformMatrix4fv( glGetUniformLocation(shader.Program, "model"), 1,
				GL_FALSE, glm::value_ptr(model));
		suzanne.Draw(shader);

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
