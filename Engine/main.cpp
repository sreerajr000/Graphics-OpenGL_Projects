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
Model *landscape, *suzanne;

void renderScene(Shader);
// positions of the point lights
glm::vec3 pointLightPositions[] = { glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f,
		-3.3f, -4.0f), glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f,
		-3.0f) };
glm::mat4 modelCyborg, modelPlane, projection;


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
	glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window.window, FrameCallback);

	//Enable Z and Alpha
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Shader lightShader("lighting.vs", "lighting.fs");
	Shader simpleDepthShader("point_shadows_depth.vs", "point_shadows_depth.fs", "point_shadows_depth.gs");

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	projection = glm::perspective(camera.Zoom,
			(GLfloat) WIDTH / (GLfloat) HEIGHT, 0.1f, 100.0f);


	//main event loop
	modelCyborg = glm::scale(modelCyborg, glm::vec3(0.5f, 0.5f, 0.5f));

	lightShader.use();
	lightShader.setInt("material.diffuse", 0);
	lightShader.setInt("material.specular", 1);
	lightShader.setInt("material.normal", 2);
	lightShader.setInt("depthMap", 3);
	lightShader.setInt("reverse_normals", 0);

	// be sure to activate shader when setting uniforms/drawing objects
	lightShader.use();
	lightShader.setVec3("viewPos", camera.Position);
	lightShader.setFloat("material.shininess", 16.0f);

	// directional light
	lightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	lightShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	lightShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	lightShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	lightShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	lightShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	lightShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	lightShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	lightShader.setFloat("pointLights[0].constant", 1.0f);
	lightShader.setFloat("pointLights[0].linear", 0.09);
	lightShader.setFloat("pointLights[0].quadratic", 0.032);
	// point light 2
	lightShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	lightShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	lightShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	lightShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	lightShader.setFloat("pointLights[1].constant", 1.0f);
	lightShader.setFloat("pointLights[1].linear", 0.09);
	lightShader.setFloat("pointLights[1].quadratic", 0.032);
	// point light 3
	lightShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	lightShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	lightShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	lightShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	lightShader.setFloat("pointLights[2].constant", 1.0f);
	lightShader.setFloat("pointLights[2].linear", 0.09);
	lightShader.setFloat("pointLights[2].quadratic", 0.032);
	// point light 4
	lightShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	lightShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	lightShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	lightShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	lightShader.setFloat("pointLights[3].constant", 1.0f);
	lightShader.setFloat("pointLights[3].linear", 0.09);
	lightShader.setFloat("pointLights[3].quadratic", 0.032);

	// spotLight
	lightShader.setVec3("spotLight.position", camera.Position);
	lightShader.setVec3("spotLight.direction", camera.Front);
	lightShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	lightShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	lightShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	lightShader.setFloat("spotLight.constant", 1.0f);
	lightShader.setFloat("spotLight.linear", 0.09);
	lightShader.setFloat("spotLight.quadratic", 0.032);
	lightShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightShader.setFloat("spotLight.outerCutOff",
			glm::cos(glm::radians(15.0f)));

	landscape = new Model("cyborg/cyborg.obj");
	suzanne = new Model("res/plane.obj");

	while (!glfwWindowShouldClose(window.window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		DoMovement();
		glfwPollEvents();
		//processKeys();
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		 // 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		float near_plane = 1.0f;
		float far_plane  = 25.0f;
		glm::vec3 lightPos = pointLightPositions[0];
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

		// 1. render scene to depth cubemap
		// --------------------------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			simpleDepthShader.use();
			for (unsigned int i = 0; i < 6; ++i)
				simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			simpleDepthShader.setFloat("far_plane", far_plane);
			simpleDepthShader.setVec3("lightPos", lightPos);
			renderScene(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// 2. render scene as normal
		// -------------------------
		glViewport(0, 0, WIDTH, HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightShader.use();
		//glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 view = camera.GetViewMatrix();
		//lightShader.setMat4("projection", projection);
		//lightShader.setMat4("view", view);
		// set lighting uniforms
		//lightShader.setVec3("lightPos", lightPos);
		//lightShader.setVec3("viewPos", camera.Position);
		//lightShader.setInt("shadows", shadows); // enable/disable shadows by pressing 'SPACE'
		lightShader.setFloat("far_plane", far_plane);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		renderScene(lightShader);

		glfwSwapBuffers(window.window);

	}

	glfwTerminate();

	return EXIT_SUCCESS;
}


void renderScene(Shader shader) {
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", modelCyborg);

	landscape->Draw(shader);

	//modelPlane = glm::rotate(modelPlane, 0.001f, glm::vec3(0.0f, -1.0f, 0.0f));
	shader.setMat4("model", modelPlane);
	suzanne->Draw(shader);
}
