#include <iostream>
#include <string>
#include "game.h"
using namespace std;

int main(){
	Game mygame;
	mygame.Configure();
	return 0;
}


// int main(){
// 	Cards mycards;
// 	Dealer dealer;
// 	Player player;
	
	
// 	mycards.Shuffle();
// 	dealer.HitCard(mycards.SendCard());
// 	dealer.HitCard(mycards.SendCard());
// 	player.HitCard(mycards.SendCard());
// 	player.HitCard(mycards.SendCard());

// 	dealer.PrintCards(true);
// 	player.PrintCards(true);

// 	if(dealer.IsBlackJack()){
// 		// the dealer has a black jack, check the player
// 		if(player.IsBlackJack()){
// 			// Push
// 		}
// 		else{
// 			// Dealer Wins
// 		}
// 	}
// 	return 0;

// 	while(true){
// 		// steps here:
// 		// 1. ask for actions
// 		// 2. determine status
// 		// 3. continue to ask for actions
// 		string input;
// 		cout<< "To Hit, press enter. To Stand, input anything and then press enter."<<endl;
// 		getline (cin, input);
// 		if(input.empty()){
// 			// player choose to 
// 			player.HitCard(mycards.SendCard());
// 			cout<<"-------------------------"<<endl;
// 			cout<<"Your Cards are now, "<<endl;
// 			player.PrintCards(false);
// 			cout<<"-------------------------"<<endl;
// 		}
// 		else{
// 			cout<<"-------------------------"<<endl;
// 			cout<<"Your Cards are now, "<<endl;
// 			player.PrintCards(false);
// 			cout<<"-------------------------"<<endl;
// 			break;
// 		}
// 	}
// 	if(player.IsBusted()){
// 		cout<<"You got busted!"<<endl;
// 	}
// 	else{
// 		// the deal begin to hit card
// 		while(!dealer.IsBusted()){
// 			switch(dealer.WhatToDo()){
// 			case kHit:
// 				dealer.HitCard(mycards.SendCard());
// 				cout<<"-------------------------"<<endl;
// 				cout<<"Dealer's Cards are now, "<<endl;
// 				dealer.PrintCards(false);
// 				cout<<"-------------------------"<<endl;
// 				break;
// 			case kStand:
// 				cout<<"-------------------------"<<endl;
// 				cout<<"Dealer's Cards are now, "<<endl;
// 				dealer.PrintCards(false);
// 				cout<<"-------------------------"<<endl;
// 				break;
// 			default:
// 				break;
// 			}

// 		}
// 		if(dealer.IsBusted()){
// 			cout<<"Oops, the deal busted himself! You win!"<<endl;
// 		}
// 		else{
// 			// to this point, neither is busted
// 			int diff = dealer.MaxValidSum() - player.MaxValidSum();
// 			cout<<"The dealer's cards:\t";
// 			dealer.PrintCards(false);
// 			cout<<"Your cards:\t";
// 			player.PrintCards(false);
// 			if(diff>0){
// 				// the dealer wins
// 				cout << "The dealer wins!"<<endl;

// 			}
// 			else if(diff<0){
// 				// the player wins
// 				cout << "You win!"<<endl;

// 			}
// 			else{
// 				// it's a push
// 				cout <<"It's a push!"<< endl;

// 			}
			
// 		}
		
// 	}

// }


// // for the first round, we need to check the blackjack status
// 		int bjmod = player.IsBlackJack() + 2*dealer.IsBlackJack();
// 		switch(bjmod){
// 			case 0:
// 				break;
// 			case 1:
// 			// the player has a blackjack but the dealer does not.
// 				cout<<"-------------------------"<<endl;
// 				cout<<"Dealer's Cards are, "<<endl;
// 				dealer.PrintCards(false);
// 				cout<<"-------------------------"<<endl;
// 				cout<<"Wow, you got a BlackJack! You Win!"<<endl;
// 				break;
// 			case 2:
// 			// the dealer has a blackjack but the player does not
// 				cout<<"-------------------------"<<endl;
// 				cout<<"Dealer's Cards are, "<<endl;
// 				dealer.PrintCards(false);
// 				cout<<"-------------------------"<<endl;
// 				cout<<"Oops, the dealer got a BlackJack! You Lose!"<<endl;
// 				break;
// 			case 3:
// 			// both have a blackjack.
// 				cout<<"-------------------------"<<endl;
// 				cout<<"Dealer's Cards are, "<<endl;
// 				dealer.PrintCards(false);
// 				cout<<"-------------------------"<<endl;
// 				cout<<"Wow! You both got Blackjack! It's a push."<<endl;
// 				break;
// 		}
// 		if(bjmod!=0) break;