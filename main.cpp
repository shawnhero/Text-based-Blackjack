#include <iostream>
#include "cards.h"
using namespace std;


int main(){
	Cards mycards;
	Dealer dealer;
	SuperGambler player;
	
	
	mycards.Shuffle();
	dealer.HitCard(mycards.SendCard());
	dealer.HitCard(mycards.SendCard());
	player.HitCard(mycards.SendCard());
	player.HitCard(mycards.SendCard());

	dealer.PrintCards(true);
	player.PrintCards(true);

}