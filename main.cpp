#include <iostream>
#include <string>
#include "game.h"
//#include "player.h"
//#include "cards.h"
using namespace std;

int main(){
	Game mygame;
	mygame.Configure();
	while(!mygame.MoneyOut()){
		WHO possible_winner = mygame.StartGame();
		if(possible_winner!=kNeither){
			mygame.CloseGame(possible_winner);
		}
		else{
			mygame.CloseGame(mygame.GameLoop());	
		}
		
	}
	return 0;
}