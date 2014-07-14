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

int Game::GetBet(){
	return bet_;
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
			winner_ = kDealer;
			return kDealer;
		case 2:
			cout <<"You got a BlackJack!"<<endl;
			player_.PrintCards(true);
			dealer_.PrintCards(false);
			winner_ = kPlayer;
			return kPlayer;
		case 3:
			cout <<"Both of you have a BlackJack! It's a push."<<endl;
			player_.PrintCards(true);
			dealer_.PrintCards(false);
			winner_ = kBoth;
			return kBoth;
		case 0:
		default:
			break;

	}
	// here, neither has a blackjack.
	dealer_.PrintCards(true);
	player_.PrintCards(true);
	cout<<endl;
	winner_ = kNeither;
	return kNeither;
}

// split the card
// the new set of cards will be hold be a virtual player: player_split
void Game::Split(){
	player_split_.HitCard(player_.SplitCard());
	splitted_ = true;
}


void Game::GameLoop(){

	auto & player = splitted_loop_ ? player_split_ : player_;
	auto & winner = (splitted_ && splitted_loop_) ? split_winner_ : winner_ ;
	// Player's loop
	// add feature for double, split and surrender
	bool player_first_round = true;
	bool end_of_player_loop = false;
	while(!end_of_player_loop){	
		// when a player busts himself, it will return kDealer directly
		// when a player gets a blackjack, it will return kPlayer directly
		// otherwise the dealer's loop will be entered.

		bool invalid_input = true;
		string input;
		char input_char;
		while(!invalid_input){
			// according to the specific cards the player got, prompt different choices
			if(player_first_round){
				// double down is allowed after a split
				// but surrender is not allowed after a split
				if(splitted_){
					cout<<"Do you wanna Hit(h), Stand(s), or DoubleDown(d)?"<<endl;
				}
				else if(player.CanSplit()){
					cout << "Do you wanna Hit(h), Stand(s), DoubleDown(d), Split(t), or Surrender(r)?"<<endl;
				}
				else{
					cout<< "Do you wanna Hit(h), Stand(s), DoubleDown(d), or Surrender(r)?"<< endl;
				}
			}
			else{
				cout<< "Do you wanna Hit(h), or Stand(s)?";
			}
			// Get and evaluate the player's input
			cin >> input;
			if(input.size()!=1){
				cout<<"I didn't get that."<<endl;
				continue;
			}
			input_char = input[0];
			switch(input_char){
				case 'h':
				case 's':
					invalid_input = false;
					break;
				case 't':
					if(splitted_){
						cout<<"You cannot split any more."<<endl;
						break;
					}
					if(!player.CanSplit()){
						cout<<"You can Split only when you have two cards of the same value."<<endl;
						break;
					}
				case 'd':
					if(!player_first_round){
						cout<<"You can Split/DoubleDown/Surrender only as the first decision of a hand."<<endl;
						break;
					}
					invalid_input = false;
					break;

				case 'r':
					if(splitted_){
						cout<<"You cannot surrender after a split."<<endl;
					}
					if(!player_first_round){
						cout<<"You can Surrender only as the first decision of a hand."<<endl;
						break;
					}
					invalid_input = false;
					break;
				
				default:
					cout<<"I didn't get that."<<endl;
					break;
			}

		}
		
		// now we can say the input_char is valid.
		// deal with different choices
		switch(input_char){
			case 's':
				// player choose to stand
				player.PrintCards(false);
				end_of_player_loop = true;
				break;
			case 'd':
				// player choose to double down
				SetBet(2*GetBet());
				cout << "Your bet is now,\t" << GetBet()<<endl;
			case 'h':
				// player choose to hit
				player.HitCard(mycards_.SendCard());
				player.PrintCards(false);
				if(player.IsBlackJack()){
					// player got blackjack. return player as the winner
					cout<<"Wow, you got a BlackJack!"<<endl;
					winner = kPlayer;
					return;
				}
				if(player.IsBusted()){
					// player got busted. return dealer as the winner
					cout<<"Oops, you busted yourself!"<<endl;
					winner = kDealer;
					return;
				}
				// double down must exit
				if(input_char=='d') end_of_player_loop=true;
				break;
			case 'r':
				SetBet(GetBet()/2);
				cout << "You chose to surrender. Your bet is now,\t" << GetBet()<<endl;
				winner = kDealer;
				return;
			// now comes the most exciting part.
			// Split the card
			case 't':
				Split();	// in the same time splitted is set to be true
				splitted_loop_ = false;
				GameLoop();
				splitted_loop_ = true;
				GameLoop();
				break;
			default:
				break;
		}
		player_first_round = false;
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
				winner = kDealer;
				return;
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
		winner = kPlayer;
		return;
	}

	// the dealer choose to stand
	// now compare the cards
	cout <<endl<<"Dealer choose to stand"<<endl;
	dealer_.PrintCards(false);
	player.PrintCards(false);
	int diff = dealer_.MaxSum() - player.MaxSum();
	if(diff>0){
		// the dealer wins
		winner = kDealer;
		return;
	}
	else if(diff<0){
		// the player wins
		winner = kPlayer;
		return;
	}
	else{
		// it's a push
		winner = kBoth;
		return;

	}
}

void Game::CloseGame(){
	assert(winner_!=kNeither);
	int win_num = 0;
	if(splitted_){
		cout << "Your first half,"<<endl;
	}
	switch(winner_){
		case kDealer:
			cout << "The dealer wins!"<<endl;
			win_num--;
			break;
		case kPlayer:
			cout << "Congratulations! You win!"<<endl;
			win_num++;
			break;
		case kBoth:
			cout <<"It's a push!"<<endl;
		default:
			break;
	}
	if(splitted_){
		cout << "Your second half,"<<endl;
		switch(split_winner_){
			case kDealer:
				cout << "The dealer wins!"<<endl;
				win_num--;
				break;
			case kPlayer:
				cout << "Congratulations! You win!"<<endl;
				win_num++;
				break;
			case kBoth:
				cout <<"It's a push!"<<endl;
			default:
				break;
		}

	}
	player_.CloseMoney(win_num * bet_);
	dealer_.CloseMoney(- win_num * bet_);
	
	PrintChipStatus();

	// clear the cards
	dealer_.ClearCards();
	player_.ClearCards();
	player_split_.ClearCards();
	// reset all the values
	splitted_ = false;
	splitted_loop_ = false;
	winner_ = kNeither;
	split_winner_ = kNeither;
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