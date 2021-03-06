/*
 * terrain.h
 *
 *  Created on: 18-Sep-2017
 *      Author: sree
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

class Terrain {
public:
	Model* terrain[3][3];
	pair<int, int> pos[3][3];
	int x,y;
	void update(){
		glm::vec3 playerPos = camera.Position;
		if(x != (playerPos.x / 10.0f) || y != (playerPos.z / 10.0f)){
			x = (playerPos.x / 10.0f);
			y = (playerPos.z / 10.0f);
			for(int i = x - 1, k = 0; i <= x + 1; i++,k++){
				for(int j = y - 1,l = 0; j <= y + 1; j++,l++){
					stringstream ss;
					ss << "terrain_";
					ss << i << "_" << j << ".obj";
					std::cout << ss.str() << std::endl;
					pos[k][l].first = i;
					pos[k][l].second = j;
					//delete terrain[k][l];
					//terrain[k][l] = new Model("terrain/" + ss.str());
				}
			}
		}
	}
	Terrain(){
		glm::vec3 playerPos = camera.Position;
		x = (playerPos.x / 10.0f);
		y = (playerPos.z / 10.0f);
		for(int i = x - 1, k = 0; i <= x + 1; i++,k++){
			for(int j = y - 1,l = 0; j <= y + 1; j++,l++){
				stringstream ss;
				ss << "terrain_";
				ss << i << "_" << j << ".obj";
				std::cout << ss.str() << std::endl;
				pos[k][l].first = i;
				pos[k][l].second = j;
				terrain[k][l] = new Model(ss.str());
			}
		}

	}
	void draw(Shader shader){
		for(unsigned int i = 0; i < 3; i++){
			for(unsigned int j = 0; j < 3; i++){
				//Draw each cells
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(pos[i][j].first, 0.0f, pos[i][j].second));
				shader.setMat4("model", model);
				terrain[i][j]->Draw(shader);
			}
		}
	}
};




#endif /* TERRAIN_H_ */
