/*
 * game_main.h
 *
 *  Created on: 21-Sep-2017
 *      Author: sree
 */

#ifndef GAME_MAIN_H_
#define GAME_MAIN_H_

void game(Window *window, Shader shader, Shader simpleDepthShader, Shader skyboxShader) {
	//Check Positions
	//std::cout << glm::distance(glm::vec3(0), camera.Position) << std::endl;
	switch(stage){
	case STAGE_1:
		if(glm::distance(glm::vec3(0), camera.Position) < 20.0f){
			tetrisStart = true;
			cameraMove = false;
			//put some transition here
			runTetris(window, shader, simpleDepthShader, skyboxShader);
		}
		break;
	default:
		std::cout << "Default Stage\n";
	}
}



#endif /* GAME_MAIN_H_ */
