#include "game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//int Game::total_money = 100;


void Game::Configure(){
	// first try to read from configure file
	// if not found, then do some initialization
	// Hmm, maybe I'll add some encryption feature, 
	// otherwise it's just too easy for the players to cheat
	ifstream file("save.dat");

	if(!file.good()){
		// write to file
		int initial_money;
		cout << "Enter inital money: ";
		cin >> initial_money;

	}
	else{
		cout << "Loading saved game..."<<endl;
		int pmoney, dmoney;
		file >> pmoney >> dmoney;
		player_.SetMoney(pmoney);
		dealer_.SetMoney(dmoney);
	}
	file.close();
	return;

}

WHO Game::StartGame(){
	mycards_.Shuffle();
	dealer_.HitCard(mycards_.SendCard());
	dealer_.HitCard(mycards_.SendCard());
	player_.HitCard(mycards_.SendCard());
	player_.HitCard(mycards_.SendCard());

	switch(DetectBlackJack(kBoth)){
		case kDealer:
			cout <<"Dealer got a BlackJack!"<<endl;
			dealer_.PrintCards(false);
			player_.PrintCards(true);
			return kDealer;
		case kPlayer:
			cout <<"You got a BlackJack!"<<endl;
			player_.PrintCards(true);
			dealer_.PrintCards(false);
			return kPlayer;
		case kBoth:
			cout <<"Both of you have a BlackJack! It's a push."<<endl;
			player_.PrintCards(true);
			dealer_.PrintCards(false);
			return kBoth;
		case kNeither:
		default:
			break;

	}
	// here, neither has a blackjack.
	dealer_.PrintCards(true);
	player_.PrintCards(true);
	cout<<endl;
	return kNeither;
}

WHO Game::DetectBlackJack(WHO who_to_detect){
	switch(who_to_detect){
		case kNeither:
			return kNeither;
		case kDealer:
			return (dealer_.IsBlackJack())?kDealer:kNeither;
		case kPlayer:
			return (player_.IsBlackJack())?kPlayer:kNeither;
		case kBoth:
			int bj = dealer_.IsBlackJack() + 2*player_.IsBlackJack();
			switch(bj){
				case 3:
					return kBoth;
				case 2:
					return kPlayer;
				case 1:
					return kDealer;
				case 0:
				default:
					return kNeither;
			}
		default:
			break;
	}
}

void Game::PlayerLoop(){
	while(true){
		// steps here:
		// 1. ask for actions
		// 2. determine status
		// 3. continue to ask for actions
		string input;
		cout<< "To Hit, press enter. To Stand, input anything and then press enter."<<endl;
		getline (cin, input);
		if(input.empty()){
			// player choose to hit
			player_.HitCard(mycards_.SendCard());
			player.PrintCards(false);
		}
		else{
			player.PrintCards(false);
			break;
		}
	}
	if(player.IsBusted()){
		cout<<"You got busted!"<<endl;
	}
	else{
		// the deal begin to hit card
		while(!dealer.IsBusted()){
			switch(dealer.WhatToDo()){
			case kHit:
				dealer.HitCard(mycards.SendCard());
				cout<<"-------------------------"<<endl;
				cout<<"Dealer's Cards are now, "<<endl;
				dealer.PrintCards(false);
				cout<<"-------------------------"<<endl;
				break;
			case kStand:
				cout<<"-------------------------"<<endl;
				cout<<"Dealer's Cards are now, "<<endl;
				dealer.PrintCards(false);
				cout<<"-------------------------"<<endl;
				break;
			default:
				break;
			}

		}
		if(dealer.IsBusted()){
			cout<<"Oops, the deal busted himself! You win!"<<endl;
		}
		else{
			// to this point, neither is busted
			int diff = dealer.MaxValidSum() - player.MaxValidSum();
			cout<<"The dealer's cards:\t";
			dealer.PrintCards(false);
			cout<<"Your cards:\t";
			player.PrintCards(false);
			if(diff>0){
				// the dealer wins
				cout << "The dealer wins!"<<endl;

			}
			else if(diff<0){
				// the player wins
				cout << "You win!"<<endl;

			}
			else{
				// it's a push
				cout <<"It's a push!"<< endl;

			}
			
		}
		
	}

}
}

void Game::CloseGame(){

}