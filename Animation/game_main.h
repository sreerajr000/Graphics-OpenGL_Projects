/*
 * game_main.h
 *
 *  Created on: 21-Sep-2017
 *      Author: sree
 */

#ifndef GAME_MAIN_H_
#define GAME_MAIN_H_

#include <time.h>
#include <unistd.h>


void displayMap(Window *window, Shader shader, Shader simpleDepthShader, Shader skyboxShader){
	cameraMove = false;
	cameraMouseMove = false;
	try{
		delete img;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	switch(player.data.stage){
	case STAGE_1:
		img = new Model("cyborg/stage1_map.obj");
		break;
	default:
		std::cout << "Somehow...bloody merlin" << std::endl;
		break;
	}
	shader.setFloat("transition", 1.0f);
	selection = 0;
	while (!glfwWindowShouldClose(window->window)) {
		glfwWaitEvents();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		stepFrame = stepFrame + deltaTime;

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(glfwGetKey(window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			enterSound.play();
			break;
		}

		renderDepth(simpleDepthShader, MAP);
		renderScene(shader, MAP);
		glfwSwapBuffers(window->window);

	}
	shader.setFloat("transition", 1.0f);
	cameraMouseMove = true;
}


void displayMenu(Window *window, Shader shader, Shader simpleDepthShader, Shader skyboxShader){
	sf::Music music;
	camera.Position = glm::vec3(0.0f, 5.0f, 0.0f);
	camera.Pitch = -90.0f;
	camera.updateCameraVectors();
	cameraMove = false;
	cameraMouseMove = false;
	try{
		delete img;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	img = new Model("cyborg/menu.obj");
	shader.setFloat("transition", 1.0f);
	selection = 0;
	while (!glfwWindowShouldClose(window->window)) {
		glfwWaitEvents();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		stepFrame = stepFrame + deltaTime;

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(glfwGetKey(window->window, GLFW_KEY_ENTER) == GLFW_PRESS){
			enterSound.play();
			break;
		}
		if(stepFrame > 0.1f){
			if(glfwGetKey(window->window, GLFW_KEY_DOWN) == GLFW_PRESS){
				selection = (selection + 1) % 3;
				dropSound.play();
			}
			if(glfwGetKey(window->window, GLFW_KEY_UP) == GLFW_PRESS){
				selection--;
				dropSound.play();
				if(selection == -1)
					selection = 2;
			}
			stepFrame = 0.0f;
		}

		renderDepth(simpleDepthShader, MENU);
		renderScene(shader, MENU);
		glfwSwapBuffers(window->window);

	}
	shader.setFloat("transition", 1.0f);
	cameraMouseMove = true;
}

void displayEscapeMenu(Window *window, Shader shader, Shader simpleDepthShader, Shader skyboxShader){
	sf::Music music;
	cameraMove = false;
	//cameraMouseMove = false;
	try{
		delete img;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	img = new Model("cyborg/escape_menu.obj");
	shader.setFloat("transition", 1.0f);
	selection = 0;

	while (!glfwWindowShouldClose(window->window)) {
		glfwWaitEvents();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		stepFrame = stepFrame + deltaTime;

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(glfwGetKey(window->window, GLFW_KEY_ENTER) == GLFW_PRESS){
			enterSound.play();
			break;
		}
		if(stepFrame > 0.1f){
			if(glfwGetKey(window->window, GLFW_KEY_DOWN) == GLFW_PRESS){
				selection = (selection + 1) % 3;
				dropSound.play();
			}
			if(glfwGetKey(window->window, GLFW_KEY_UP) == GLFW_PRESS){
				selection--;
				dropSound.play();
				if(selection == -1)
					selection = 2;
			}
			stepFrame = 0.0f;
		}

		renderDepth(simpleDepthShader, ESCAPE_MENU);
		renderScene(shader, ESCAPE_MENU);
		glfwSwapBuffers(window->window);

	}

	shader.setFloat("transition", 1.0f);

	switch(selection) {
	case 0:
		std::cout << "Resumed" << std::endl;
		break;
	case 1:
		//Display map and position
		std::cout << "Displaying Map" << std::endl;
		displayMap(window, shader, simpleDepthShader, skyboxShader);
		break;
	case 2:
		glfwSetWindowShouldClose(window->window, true);
		break;
	default:
		std::cout << "This is crazy. Should not happen...WTH?" << std::endl;
		break;
	}

	cameraMouseMove = true;
	cameraMove = true;
}

void game(Window *window, Shader shader, Shader simpleDepthShader, Shader skyboxShader) {
	//Check Positions
	//std::cout << glm::distance(glm::vec3(0), camera.Position) << std::endl;
	sf::Music music;
	switch(player.data.stage){
	case STAGE_1:
		if(glm::distance(glm::vec3(0), camera.Position) < 16.0f){
			tetrisStart = true;
			cameraMove = false;
			//put some transition here
			player.data.stage = TETRIS;
			enterSound.play();
			setUpSkyBox(tetrisSkybox);
			runTetris(window, shader, simpleDepthShader, skyboxShader);
		}
		break;
	default:
		std::cout << "Default Stage\n";
	}
}



void fadeOutMusic(sf::Sound *sound, float duration){
	time_t start = time(0);
	while(1){
		double now = difftime( time(0), start);
		double volume = 100 * (1.0f - now / duration);
		if(volume < 1)
			break;
		sound->setVolume(volume);
	}
	sound->stop();
}

#endif /* GAME_MAIN_H_ */
