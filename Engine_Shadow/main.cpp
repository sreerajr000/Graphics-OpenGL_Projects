#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "callback.h"
#include "scene.h"

#include <iostream>

int main()
{

	//Create Window
	Window window(1280, 720, "Shadow Test");
	if (!window.createWindow()) {
		return EXIT_FAILURE;
	}
    glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window.window, mouse_callback);
    //glfwSetScrollCallback(window.window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("shadow_mapping.vs", "shadow_mapping.fs");
    Shader simpleDepthShader("shadow_mapping_depth.vs", "shadow_mapping_depth.fs");
    Shader skyboxShader("skybox.vs", "skybox.fs");

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
    shader.setInt("shadowMap", 3);

    skyboxShader.use();
	skyboxShader.setInt("skybox", 0);


	loadModels();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window.window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window.window);

        // change light position over time
        lightPos.x = sin(glfwGetTime()) * 3.0f;
        lightPos.z = cos(glfwGetTime()) * 2.0f;
        lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        renderDepth(simpleDepthShader);

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

    glfwTerminate();
    return 0;
}


