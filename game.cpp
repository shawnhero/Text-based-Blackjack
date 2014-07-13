// Author: Shawn Wu
// Email:  wuxu@cs.ucla.edu

// Provide a class to simulate all the processes in a hand of the game

#include "game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void Game::LoadGame(){
	// first try to read from save.data
	// Hmm, maybe I'll add some encryption feature, 
	// otherwise it's just too easy for the players to cheat
	while(true){
		cout<<"Load Last Saved Game?(y/n)";
		string input;
		cin>>input;
		if(input.compare("y")==0){
			// User chooses to load saved game
			ifstream file("save.dat");
			if(file.good()){
				cout << "Loading saved game..."<<endl;
				int pchips, dchips;
				file >> pchips >> dchips;
				if(pchips<=0){
					cout<<"Last time you lost all your money. A new game is started.."<<endl;
				}
				else if(pchips + dchips == (kPlayerChips+kDealerChips)){
					player_.SetChips(pchips);
					dealer_.SetChips(dchips);
				}
			}
			else{
				// file not found. 
				cout <<"Saved File not found. Using default setting.."<<endl;
			}
			file.close();
			break;
		}
		else if(input.compare("n")==0){
			cout <<"Creating new game profile.."<<endl;
			break;
		}
		else{
			cout<<"I didn't get that."<<endl;
			
		}
	}
	PrintChipStatus();
	return;

}

void Game::SetBet(int bet){
	bet_ = bet;
}

bool Game::MoneyOut(){
	if(player_.GetChips()<=0){
		cout<<"You have no money left. Game Over."<<endl;
		return true;
	}
	else if(dealer_.GetChips()<=0){
		cout<<"You have won all the money from the dealer! Good Job!"<<endl;
		return true;
	}
	return false;
}

WHO Game::StartGame(){
	cout<<endl<<"-------------------------"<<endl;
	cout<<"Starting a new round.."<<endl;
	// // shuffle for every hand
	//mycards_.Shuffle();
	dealer_.HitCard(mycards_.SendCard());
	dealer_.HitCard(mycards_.SendCard());
	player_.HitCard(mycards_.SendCard());
	player_.HitCard(mycards_.SendCard());

	int bj = dealer_.IsBlackJack() + 2*player_.IsBlackJack();
	switch(bj){
		case 1:
			cout <<"Dealer got a BlackJack!"<<endl;
			dealer_.PrintCards(false);
			player_.PrintCards(true);
			return kDealer;
		case 2:
			cout <<"You got a BlackJack!"<<endl;
			player_.PrintCards(true);
			dealer_.PrintCards(false);
			return kPlayer;
		case 3:
			cout <<"Both of you have a BlackJack! It's a push."<<endl;
			player_.PrintCards(true);
			dealer_.PrintCards(false);
			return kBoth;
		case 0:
		default:
			break;

	}
	// here, neither has a blackjack.
	dealer_.PrintCards(true);
	player_.PrintCards(true);
	cout<<endl;
	return kNeither;
}

WHO Game::GameLoop(){

	// Player's loop
	while(true){	
		// when a player busts himself, it will return kDealer directly
		// when a player gets a blackjack, it will return kPlayer directly
		// otherwise the dealer's loop will be entered.
		string input;
		cout<< "Do you wanna Hit(h), or Stand(s)?";
		// getline (cin, input);
		cin >> input;
		if(input.compare("h")==0){
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
		else if(input.compare("s")==0){
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
	PrintChipStatus();
}



void Game::PrintChipStatus(){
	cout<<"-------------------------"<<endl;
	cout<<"Your money, "<<player_.GetChips()<<endl;
	cout<<"Dealer's money, "<<dealer_.GetChips()<<endl;
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
			else if(bet>player_.GetChips()){
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
 	myfile << player_.GetChips()<<" "<<dealer_.GetChips();
 	myfile.close();
}