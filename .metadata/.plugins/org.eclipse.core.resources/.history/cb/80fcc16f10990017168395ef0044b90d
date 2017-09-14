#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>

#include "camera.h"
glm::mat4 projection, view, model;
int speed = 1;
// camera
Camera camera(glm::vec3(0.0f, 2.0f, 15.0f));
btRigidBody *cameraBody;
#include "model.h"
Model *cyborg, *planeScene, *sphere, *plane, *landscape, *box;
float stepFrame = 0.0f;
#include "window.h"
#include "shader.h"
#include "model.h"
#include "callback.h"
//#include "physics_main.h"
#include "scene.h"

#include <iostream>


int main() {
	srand(time(NULL));
	//Create Window
	Window window(1280, 720, "Shadow Test");
	if (!window.createWindow()) {
		return EXIT_FAILURE;
	}
	glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.window, mouse_callback);
	glfwSetKeyCallback(window.window, key_callback);
	//glfwSetScrollCallback(window.window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader shader("shadow_mapping.vs", "shadow_mapping.fs");
	Shader simpleDepthShader("shadow_mapping_depth.vs",
			"shadow_mapping_depth.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	//Shader pointDepthShader("point_shadows_depth.vs", "point_shadows_depth.fs", "point_shadows_depth.gs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	setUpSkyBox();

	setUpDepthMap();

	// shader configuration
	// --------------------
	shader.use();
	shader.setInt("material.diffuse", 0);
	shader.setInt("material.specular", 1);
	shader.setInt("material.normal", 2);
	//Dir light
	shader.setInt("shadowMap", 3);
	//Point light
	//shader.setInt("depthMap", 3);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	loadModels();
	//physicsSetUp();
	// render loop
	// -----------
	block = new Block();
	while (!glfwWindowShouldClose(window.window)) {
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		stepFrame = stepFrame + deltaTime;
		// input
		// -----
		processInput(window.window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Game simulation
		step();
		//Game step
		if(stepFrame > 1/GLfloat(speed)){
			stepFrame = 0.0f;
			gameStep();
		}
		//step simulation
		//world->stepSimulation(deltaTime);

		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		renderDepth(simpleDepthShader);
		//renderPointDepth(pointDepthShader);

		// 2. render scene as normal using the generated depth/shadow map
		// --------------------------------------------------------------
		renderScene(shader);
		renderSkyBox(skyboxShader);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//cleanPhysics();

	glfwTerminate();
	return 0;
}

