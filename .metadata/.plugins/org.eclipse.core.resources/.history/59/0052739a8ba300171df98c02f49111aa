/*
 * tetris.h
 *
 *  Created on: 13-Sep-2017
 *      Author: sree
 */

#pragma once

std::vector<std::vector<int> > frame(18, std::vector<int>(10, 0));

#include "block.h"

Block *block;

void drawFrame(Shader shader) {
	for(int i = 0; i < frame.size(); i++) {
		for(int j = 0; j < frame[i].size(); j++) {
			if(frame[i][j]) {
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(2.01f *GLfloat(j), 2.01f * GLfloat(i), 0.0f));
				shader.setMat4("model", model);
				box->Draw(shader);
			}
		}
	}
}

void clearFrame() {
	frame.clear();
	frame.resize(18, std::vector<int>(10, 0));
}
void display() {
	for (auto x : frame) {
		for (auto y : x)
			cout << y << " ";
		cout << "\n";
	}
	cout << "\n";
}

void step() {
	if(moveLeft){
		SoundEngine->play2D("sound/drop_beep.wav");
		moveLeft = false;
		block->x--;
		if(block->checkCollision())
			block->x++;
	}
	else if(moveRight){
		SoundEngine->play2D("sound/drop_beep.wav");
		moveRight = false;
		block->x++;
		if(block->checkCollision())
			block->x--;
	}
	else if(moveDown) {
		//moveDown = false;
		speed = 10;
	}
	else if(moveUp) {
		SoundEngine->play2D("sound/drop_beep.wav");
		moveUp = false;
		block->rotateBlock();
		if(block->checkCollision())
			block->urotateBlock();
	}
	if(!moveDown)
		speed = 1;
}

void checkClear(){
	vector<vector<int> > newFrame;
	int ct = 0;
	for(int i = 0; i < frame.size(); i++){
		bool flag = false;
		for(int j = 0; j < frame[i].size(); j++){
			if(frame[i][j] == 0){
				flag = true;
				break;
			}
		}
		if(flag){
			newFrame.push_back(frame[i]);
		}
		else{
			ct++;
		}
	}
	vector<int> zeroArray(10, 0);
	while(newFrame.size() <= 18)
		newFrame.push_back(zeroArray);
	frame = newFrame;
}

void renderTetrisSceneContents(const Shader &shader) {
	//frame and blocks
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(9.0f, 18.0f, -1.25f));
	shader.setMat4("model", model);
	backFrame->Draw(shader);
	drawFrame(shader);
	block->draw(shader);

}

void gameStep(Window *window, Shader shader, Shader simpleDepthShader, Shader skyboxShader) {
	block->y--;
	SoundEngine->play2D("sound/drop_beep.wav");
	if(block->checkCollision()){
		block->addtoFrame();
		float d = 0.0f;
		SoundEngine->play2D("sound/spin.wav");
		while (!glfwWindowShouldClose(window->window)) {
			glfwWaitEvents();
			d += 2 * deltaTime;
			if(d > 3.14f/2.0f)
				break;
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			stepFrame = stepFrame + deltaTime;

			processInput(window->window);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			box->modelMatrix = glm::rotate(box->modelMatrix, 2 * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));

			renderDepth(simpleDepthShader, TETRIS);
			renderScene(shader, TETRIS);
			renderSkyBox(skyboxShader);
			glfwSwapBuffers(window->window);
		}
		box->modelMatrix = glm::mat4();
		//display();
		delete block;
		block = new Block();
	}
	checkClear();
}

void runTetris(Window *window, Shader shader, Shader simpleDepthShader, Shader skyboxShader) {
	camera.Pitch = 0.0f;
	camera.Yaw = -90.0f;
	camera.updateCameraVectors();
	while (!glfwWindowShouldClose(window->window)) {
		camera.Position = glm::vec3(8.0f, 15.0f, 31.0f);
		glfwWaitEvents();
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		stepFrame = stepFrame + deltaTime;
		// input
		// -----
		processInput(window->window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		step();
		//Game step
		if(stepFrame > 1/GLfloat(speed)){
			stepFrame = 0.0f;
			gameStep(window, shader, simpleDepthShader, skyboxShader);
		}

		renderDepth(simpleDepthShader, TETRIS);
		renderScene(shader, TETRIS);
		renderSkyBox(skyboxShader);
		glfwSwapBuffers(window->window);

	}
}
