#include "game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//int Game::total_money = 100;


void Game::LoadGame(){
	// first try to read from configure file
	// if not found, then do some initialization
	// Hmm, maybe I'll add some encryption feature, 
	// otherwise it's just too easy for the players to cheat
	ifstream file("save.dat");

	if(!file.good()){
		// file not existed
		// int initial_money;
		// string input;
		// while(true){
		// 	cout << "Enter inital money: ";
		// 	getline(cin, input);
		// 	stringstream ss(input);
		// 	if (!(ss>>initial_money)){
		// 		cout<<"I didn't get that. Please enter an integer"<<endl;
		// 	}
		// 	else break;
		// }
		// player_.SetMoney(initial_money);
		// dealer_.SetMoney(initial_money);
	}
	else{
		cout << "Loading saved game..."<<endl;
		int pmoney, dmoney;
		file >> pmoney >> dmoney;
		if(pmoney<=0){
			cout<<"Last time you lost all your money. A new game is started.."<<endl;
		}
		else if(pmoney + dmoney == (PLAYER_MONEY+DEALER_MONEY)){
			player_.SetMoney(pmoney);
			dealer_.SetMoney(dmoney);
		}
	}
	PrintMoneyStatus();
	file.close();
	return;

}

void Game::SetBet(int bet){
	bet_ = bet;
}

bool Game::MoneyOut(){
	if(player_.GetMoney()<=0){
		cout<<"You have no money left. Game Over."<<endl;
		return true;
	}
	else if(dealer_.GetMoney()<=0){
		cout<<"You have won all the money from the dealer! Good Job!"<<endl;
		return true;
	}
	return false;
}

WHO Game::StartGame(){
	cout<<endl<<"-------------------------"<<endl;
	cout<<"Starting a new round.."<<endl;
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
		char input;
		cout<< "Do you wanna Hit(h), or Stand(s)?";
		// getline (cin, input);
		cin >> input;
		if(input=='h'){
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
		else if(input=='s'){
			// player choose to stand
			player_.PrintCards(false);
			break;
		}
		else{
			cout<<"I didn't get that."<<endl;
		}
	}

	cout<<"-------------------------"<<endl;
	cout <<endl<<"Now the dealer's turn.."<<endl;

	// Deal's loop
	while(!dealer_.IsBusted()){
		dealer_.PrintCards(false);
		if(dealer_.WhatToDo()==kHit){
			dealer_.HitCard(mycards_.SendCard());
			//dealer_.PrintCards(false);
			// check whether the dealer got a blackjack
			if(dealer_.IsBlackJack()){
				// dealer got blackjack. return player as the winner
				cout<<"The dealer got a BlackJack!"<<endl;
				dealer_.PrintCards(false);
				return kDealer;
			}
		}
		else{
			// dealer choose to stand
			break;
		}
	}
	// reasons for exiting dealer's loop
	// 1. the dealer busted himself
	// 2. the dealer choose to stand
	if(dealer_.IsBusted()){
		dealer_.PrintCards(false);
		cout<<"Oops, the deal busted himself!"<<endl;
		return kPlayer;
	}

	// the dealer choose to stand
	// now compare the cards
	cout <<endl<<"Dealer choose to stand"<<endl;
	dealer_.PrintCards(false);
	player_.PrintCards(false);
	int diff = dealer_.MaxSum() - player_.MaxSum();
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
	dealer_.ClearCards();
	player_.ClearCards();
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

bool Game::PromptExit(){
	// prompt exit
	// also let the player set a new bet if he chooses to contunue
	string input;
	int bet;
	while(true){
		cout << "Enter your bet(enter x to exit game),";
		cin>>input;
		stringstream s(input);
		if(!(s>>bet)){
			// the player entered a non-integer
			if(s.str().compare("x")==0){
				// player entered x to exit game
				SaveGame();
				return true;
			}
			cout<<"I didn't get that. Please enter an integer"<<endl;
		}
		else{
			// the player entered a valid integer
			// still need to check whether it's a valid bet
			if(bet<=0){
				cout<<"You must must bet at least 1 chip each hand!"<<endl;
			}
			else if(bet>player_.GetMoney()){
				cout<<"You must bet within your budget!"<<endl;
			}
			else break;
		}
	}
	SetBet(bet);
	cout <<endl;
	return false;
}

void Game::SaveGame(){
	cout <<"Saving game to save.dat"<<endl;
	ofstream myfile;
 	myfile.open ("save.dat");
 	myfile << player_.GetMoney()<<" "<<dealer_.GetMoney();
 	myfile.close();
}