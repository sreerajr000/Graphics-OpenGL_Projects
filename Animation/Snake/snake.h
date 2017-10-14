/*
 * snake.h
 *
 *  Created on: 12-Oct-2017
 *      Author: sree
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include <vector>
#include "snake_block.h"

class Snake{
public:
	std::vector<SnakeBlock> tail;
	SnakeBlock head;
};


#endif /* SNAKE_H_ */
