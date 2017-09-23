#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <thread>
#include <irrKlang.h>

using namespace irrklang;
ISoundEngine *SoundEngine = createIrrKlangDevice();

enum STAGE{
	STAGE_1, STAGE_2
}stage;


float cameraYHeight = 15.0f;
#include "camera.h"
glm::mat4 projection, view, model;
int speed = 1;
float stepFrame = 0.0f;
float lastFrame = 0.0f;
float deltaTime = 0.0f;
// camera
Camera camera(glm::vec3(0.0f, cameraYHeight, 90.0f));
btRigidBody *cameraBody;
#include "model.h"
Model *cyborg, *planeScene, *sphere, *plane, *landscape, *box, *backFrame, *animation, *raven, *landscape_wall, *img;
bool tetrisStart = false;
bool cameraMove = true;
bool cameraMouseMove = true;
// settings
unsigned int SCR_WIDTH;
unsigned int SCR_HEIGHT;
float lastX;
float lastY;

#include "window.h"
#include "shader.h"
#include "model.h"
#include "callback.h"
//#include "physics_main.h"
#include "scene.h"
#include "game_main.h"

#include <iostream>

using namespace irrklang;

int main() {
	srand(time(NULL));
	//Create Window

	Window window("Game");
	if (!window.createWindow()) {
		return EXIT_FAILURE;
	}
	SCR_WIDTH = window.width;
	SCR_HEIGHT = window.height;
	lastX = (float) SCR_WIDTH / 2.0;
	lastY = (float) SCR_HEIGHT / 2.0;

	stage = STAGE_1;

	SoundEngine->play2D("sound/wind.wav", GL_TRUE);
	SoundEngine->play2D("sound/theme.wav", GL_TRUE);
	glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.window, mouse_callback);
	glfwSetKeyCallback(window.window, key_callback);
	//glfwSetScrollCallback(window.window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

	renderImageTransition(&window, shader, simpleDepthShader, skyboxShader, "cyborg/img.obj", 30.0f);
	camera.Position = glm::vec3(0.0f, cameraYHeight, 90.0f);
	camera.Pitch = 0.0f;
	camera.updateCameraVectors();
	cameraMove = true;
	while (!glfwWindowShouldClose(window.window)) {
		glfwWaitEvents();
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
		//move();
		//step simulation
		//world->stepSimulation(deltaTime, deltaTime * 60 + 2);

		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		renderDepth(simpleDepthShader, 1);
		//renderPointDepth(pointDepthShader);

		// 2. render scene as normal using the generated depth/shadow map
		// --------------------------------------------------------------
		renderScene(shader,1);
		renderSkyBox(skyboxShader);

		game(&window, shader, simpleDepthShader, skyboxShader);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window.window);

	}
	cleanModels();
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//cleanPhysics();

	glfwTerminate();
	return 0;
}
