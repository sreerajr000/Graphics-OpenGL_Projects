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
		moveLeft = false;
		block->x--;
		if(block->checkCollision())
			block->x++;
	}
	else if(moveRight){
		moveRight = false;
		block->x++;
		if(block->checkCollision())
			block->x--;
	}
	else if(moveDown) {
		moveDown = false;
		speed = 10;
	}
	else if(moveUp) {
		moveUp = false;
		block->rotateBlock();
		if(block->checkCollision())
			block->urotateBlock();
	}
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


void gameStep() {
	block->y--;
	if(block->checkCollision()){
		block->addtoFrame();
		//display();
		delete block;
		block = new Block();
	}
	checkClear();
}


