#include <iostream>
#include <string>
#include "game.h"
//#include "player.h"
//#include "cards.h"
using namespace std;

int main(){
	Game mygame;
	mygame.Configure();
	WHO possible_winner = mygame.StartGame();
	if(possible_winner!=kNeither){
		mygame.CloseGame(possible_winner);
		return 0;
	}
	mygame.CloseGame(mygame.GameLoop());
	return 0;
}