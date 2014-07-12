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
	}
}

WHO Game::GameLoop(){

	// Player's loop
	while(true){	
		// when a player busts himself, it will return directly
		// when a player gets a blackjack, it will return directly
		// otherwise the dealer's loop will be entered.
		string input;
		cout<< "To Hit, press enter. To Stand, input anything and then press enter."<<endl;
		getline (cin, input);
		if(input.empty()){
			// player choose to hit
			player_.HitCard(mycards_.SendCard());
			player_.PrintCards(false);
			if(player_.IsBlackJack()){
				// player got blackjack. return player as the winner
				cout<<"Wow, you got a BlackJack!"<<endl;
				return kPlayer;
			}
			if(player_.IsBusted()){
				// player got busted. return dealer as the winner
				cout<<"Oops, you busted yourself!"<<endl;
				return kDealer;
			}
		}
		else{
			// player choose to stand
			player_.PrintCards(false);
			break;
		}
	}
	
	cout <<endl<<"Now the dealer's turn.."<<endl;

	// Deal's loop
	while(!dealer_.IsBusted()){
		if(dealer_.WhatToDo()==kHit){
			dealer_.HitCard(mycards_.SendCard());
			dealer_.PrintCards(false);
			// check whether the dealer got a blackjack
			if(dealer_.IsBlackJack()){
				// dealer got blackjack. return player as the winner
				cout<<"The dealer got a BlackJack!"<<endl;
				return kDealer;
			}
		}
		else{
			// dealer choose to stand
			dealer_.PrintCards(false);
			break;
		}
	}
	if(dealer_.IsBusted()){
		cout<<"Oops, the deal busted himself!"<<endl;
		return kPlayer;
	}
	player_.PrintCards(false);
	int diff = dealer_.MaxValidSum() - player_.MaxValidSum();
	if(diff>0){
		// the dealer wins
		return kDealer;
	}
	else if(diff<0){
		// the player wins
		return kPlayer;
	}
	else{
		// it's a push
		return kBoth;

	}
}

void Game::CloseGame(WHO winner){
	assert(winner!=kNeither);
	switch(winner){
		case kDealer:
			cout << "The dealer wins!"<<endl;
			dealer_.CloseMoney(bet_);
			player_.CloseMoney(-bet_);
			break;
		case kPlayer:
			cout << "Congratulations! You win!"<<endl;
			dealer_.CloseMoney(-bet_);
			player_.CloseMoney(bet_);
			break;
		case kBoth:
			cout <<"It's a push!"<<endl;
		default:
			break;
	}
	PrintMoneyStatus();
}



void Game::PrintMoneyStatus(){
	cout<<"-------------------------"<<endl;
	cout<<"Your money, "<<player_.GetMoney()<<endl;
	cout<<"Dealer's money, "<<dealer_.GetMoney()<<endl;
	cout<<"-------------------------"<<endl;
}