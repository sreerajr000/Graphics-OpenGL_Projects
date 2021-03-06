/*
 * player.h
 *
 *  Created on: 25-Sep-2017
 *      Author: sree
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#include <fstream>
struct playerData{
	int stage, score;
};
class Player{
public:
	playerData data;
	Player (){
		data.stage = STAGE_1;
		data.score = 0;
	}

	void saveGame() {
		fstream out("save.dat", ios::out | ios::binary);
		out.write((char*)&data, sizeof(playerData));
		out.close();
	}

	void loadGame() {
		try{
			fstream in("save.dat", ios::in | ios::binary);
			in.read((char*)&data, sizeof(playerData));
			in.close();
		}
		catch(exception &e){
			std::cout << e.what() << std::endl;
		}
	}

};



#endif /* PLAYER_H_ */
