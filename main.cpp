#include <iostream>
#include <string>
#include "game.h"
//#include "player.h"
//#include "cards.h"
using namespace std;

int main(){
	Game mygame;
	// try to load last saved game
	mygame.LoadGame();
	while(!mygame.MoneyOut()){
		if(mygame.PromptExit()) break;
		WHO possible_winner = mygame.StartGame();
		if(possible_winner!=kNeither){
			mygame.CloseGame();
		}
		else{
			mygame.GameLoop();
			mygame.CloseGame();	
		}
	}
	return 0;
}