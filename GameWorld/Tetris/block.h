/*
 * block.h
 *
 *  Created on: 13-Sep-2017
 *      Author: sree
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <vector>
#include <time.h>
#include <stdlib.h>

std::vector<std::vector<std::vector<vector<int> > > > blocks = { { {
		{ 1, 1, 1 }, { 0, 1, 0 }, { 0, 0, 0 } }, { { 0, 0, 1 }, { 0, 1, 1 }, {
		0, 0, 1 } }, { { 0, 0, 0 }, { 0, 1, 0 }, { 1, 1, 1 } }, { { 1, 0, 0 }, {
		1, 1, 0 }, { 1, 0, 0 } } }, { { { 0, 0, 2, 0 }, { 0, 0, 2, 0 }, { 0, 0,
		2, 0 }, { 0, 0, 2, 0 } }, { { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
		{ 2, 2, 2, 2 }, { 0, 0, 0, 0 } } }, { { { 3, 3 }, { 3, 3 } } }, { { { 0,
		4, 0 }, { 0, 4, 0 }, { 0, 4, 4 } }, { { 0, 0, 0 }, { 0, 0, 4 }, { 4, 4,
		4 } }, { { 0, 4, 4 }, { 0, 0, 4 }, { 0, 0, 4 } }, { { 4, 4, 4 }, { 4, 0,
		0 }, { 0, 0, 0 } } }, { { { 0, 5, 0 }, { 0, 5, 0 }, { 5, 5, 0 } }, { {
		0, 0, 0 }, { 5, 0, 0 }, { 5, 5, 5 } }, { { 5, 5, 0 }, { 5, 0, 0 }, { 5,
		0, 0 } }, { { 5, 5, 5 }, { 0, 0, 5 }, { 0, 0, 0 } } }, { { { 6, 6, 0 },
		{ 0, 6, 6 }, { 0, 0, 0 } }, { { 0, 6, 0 }, { 6, 6, 0 }, { 6, 0, 0 } } },
		{ { { 0, 7, 7 }, { 7, 7, 0 }, { 0, 0, 0 } }, { { 7, 0, 0 }, { 7, 7, 0 },
				{ 0, 7, 0 } } } };

class Block {
public:
	int x = 4, y = 15;
	int blockid = glm::floor(rand() % 7);
	int orient = 0;

	void draw(Shader shader) {
		//Set the color or something
		for(int i = 0; i < blocks[blockid][orient].size(); i++) {
			for(int j = 0; j < blocks[blockid][orient][i].size(); j++) {
				if(blocks[blockid][orient][i][j]) {
					//Draw the block at some offset to this.x and this.y
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(2.01f *GLfloat(j + x), 2.01f * GLfloat(i + y), 0.0f));
					shader.setMat4("model", model);
					box->Draw(shader);
				}
			}
		}
	}

	void rotateBlock() {
		//y--;
		orient = (++orient) % (blocks[blockid].size());
	}

	void urotateBlock() {
		if (orient == 0)
		orient = blocks[blockid].size() - 1;
		else
		orient = (--orient) % (blocks[blockid].size());
	}

	bool checkCollision() {
		vector<vector<int> > blk = blocks[blockid][orient];
		try {
			//check sides
			for (int i = 0; i < blk.size(); i++) {
				for (int j = 0; j < blk[i].size(); j++) {
					if (blk[i][j]) {
						if (x + j < 0 || x + j > 9)
						return true;
					}
				}
			}

			for (int i = 0; i < blk.size(); i++) {
				for (int j = 0; j < blk[i].size(); j++) {
					if (blk[i][j]) {
						if (y + i < 0)
						return true;
					}
				}
			}

			for (int i = 0; i < blk.size(); i++) {
				for (int j = 0; j < blk[i].size(); j++) {
					if(y+i < 0 || y+i > 17 || x+j < 0 || x+j > 9) continue;
					if (blk[i][j])
					if (frame[y + i][x + j] > 0)
					return true;
				}
			}

		}
		catch(int e) {
			std::cout << e << std::endl;
		}
		return false;
	}

	void addtoFrame() {
		y++;
		vector<vector<int> > blk = blocks[blockid][orient];
		for (int i = 0; i < blk.size(); i++) {
			for (int j = 0; j < blk[i].size(); j++) {
				if (blk[i][j]) {
					frame[y + i][x + j] = blk[i][j];
				}
			}
		}
	}
};

#endif /* BLOCK_H_ */
