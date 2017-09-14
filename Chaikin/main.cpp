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

void runIteration(vector<GLfloat> controlPoints, vector<GLfloat> &vertices, GLuint iteration);

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

	//Load Shader
	Shader curveShader("line.vs", "line.frag");
	Shader controlShader("line.vs", "control.frag");
	//create VAO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	vector<GLfloat> controlPoints;

	vector<GLfloat> vertices = controlPoints;
	//create VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
			&vertices[0], GL_DYNAMIC_DRAW);



	GLuint iteration = 0;

	//main event loop
	while (!glfwWindowShouldClose(window.window)) {
		//DoMovement();
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//calculate vertices
		if(newPoint) {
			newPoint = false;
			controlPoints.push_back(x_pos);
			controlPoints.push_back(y_pos);
			controlPoints.push_back(0.0f);
		}

		if(iterationInc) {
			iterationInc = false;
			iteration++;
		}
		if(iterationDec) {
			iterationDec = false;
			if(iteration != 0)
				iteration--;
		}
		runIteration(controlPoints, vertices, iteration);
		//display curve
		curveShader.Use();
		glEnableVertexAttribArray(0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
					&vertices[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		if(loop)
			glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 3);
		else
			glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 3);

		//display control lines
		controlShader.Use();
		glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(GLfloat),
							&controlPoints[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		if(loop)
			glDrawArrays(GL_LINE_LOOP, 0, controlPoints.size() / 3);
		else
			glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size() / 3);
		glDisableVertexAttribArray(0);


		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();

	return EXIT_SUCCESS;
}

void runIteration(vector<GLfloat> controlPoints, vector<GLfloat> &vertices, GLuint iteration) {
	vector<GLfloat> g;
	g = controlPoints;
	vertices.clear();
	if(controlPoints.size() < 1 ) return;

	while(iteration--) {
		vector<GLfloat> h;
		if(!loop){
			h.push_back(g[0]);
			h.push_back(g[1]);
			h.push_back(g[2]);
		}
		for(int i = 0; i < g.size() - 3; i+=3){
			h.push_back((3*g[i]/4 + g[(i+3)]/4));
			h.push_back((3*g[i+1]/4 + g[(i+4)]/4));
			h.push_back((3*g[i+2]/4 + g[(i+5)]/4));
			h.push_back((g[i]/4 + 3*g[(i+3)]/4));
			h.push_back((g[i+1]/4 + 3*g[(i+4)]/4));
			h.push_back((g[i+2]/4 + 3*g[(i+5)]/4));
		}
		if(!loop){
			h.push_back(g[g.size() - 3]);
			h.push_back(g[g.size() - 2]);
			h.push_back(g[g.size() - 1]);
		}
		else {
			int j = g.size() - 3;
			h.push_back((3*g[j]/4 + g[0]/4));
			h.push_back((3*g[j+1]/4 + g[1]/4));
			h.push_back((3*g[j+2]/4 + g[2]/4));
			h.push_back((g[j]/4 + 3*g[0]/4));
			h.push_back((g[j+1]/4 + 3*g[1]/4));
			h.push_back((g[j+2]/4 + 3*g[2]/4));
		}
		g.clear();
		g = h;

	}
	vertices.insert(vertices.end(), g.begin(), g.end());

}

