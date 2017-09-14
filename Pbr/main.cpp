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

#include <iostream>

void renderSphere();

// settings
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;
char *TITLE = "OpenGL PBR Test";

int main() {
	Window window(WIDTH, HEIGHT, TITLE);
	if (!window.createWindow()) {
		return EXIT_FAILURE;
	}
	glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.window, mouse_callback);
	glfwSetScrollCallback(window.window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader shader("pbr.vs", "pbr.fs");

	shader.use();
	shader.setInt("albedoMap", 0);
	shader.setInt("normalMap", 1);
	shader.setInt("metallicMap", 2);
	shader.setInt("roughnessMap", 3);
	shader.setInt("aoMap", 4);

	// load PBR material textures
	// --------------------------
	unsigned int albedo = loadTexture("albedo.png");
	unsigned int normal = loadTexture("normal.png");
	unsigned int metallic = loadTexture("metallic.png");
	unsigned int roughness = loadTexture("roughness.png");
	unsigned int ao = loadTexture("ao.jpg");

	// lights
	// ------
	glm::vec3 lightPositions[] = { glm::vec3(0.0f, 0.0f, 10.0f), };
	glm::vec3 lightColors[] = { glm::vec3(150.0f, 150.0f, 150.0f), };
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	glm::mat4 projection = glm::perspective(camera.Zoom,
			(float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
	shader.use();
	shader.setMat4("projection", projection);

	Model ourModel( "sphere.obj" );
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window.window)) {
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window.window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		shader.setVec3("camPos", camera.Position);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallic);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ao);

		// render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
		glm::mat4 model;
		for (unsigned int row = 0; row < nrRows; ++row) {
			for (unsigned int col = 0; col < nrColumns; ++col) {
				model = glm::mat4();
				model = glm::translate(model,
						glm::vec3((float) (col - (nrColumns / 2)) * spacing,
								(float) (row - (nrRows / 2)) * spacing, 0.0f));
				shader.setMat4("model", model);
				//renderSphere();
				ourModel.Draw(shader);
			}
		}

		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and
		// keeps the codeprint small.
		for (unsigned int i = 0;
				i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
			glm::vec3 newPos = lightPositions[i]
					+ glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			shader.setVec3("lightColors[" + std::to_string(i) + "]",
					lightColors[i]);

			model = glm::mat4();
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			shader.setMat4("model", model);
			renderSphere();
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere() {
	if (sphereVAO == 0) {
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
				float xSegment = (float) x / (float) X_SEGMENTS;
				float ySegment = (float) y / (float) Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI)
						* std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI)
						* std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y) {
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x) {
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			} else {
				for (int x = X_SEGMENTS; x >= 0; --x) {
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (int i = 0; i < positions.size(); ++i) {
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0) {
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0) {
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0],
				GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				indices.size() * sizeof(unsigned int), &indices[0],
				GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
				(void*) (3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
				(void*) (5 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}


