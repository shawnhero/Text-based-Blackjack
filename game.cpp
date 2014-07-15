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
// the new set of cards will be hold be a virtual player
// the virtual player will be added to a vector
void Game::Split(int index){
	//assert(split_number_ > 0);
	Player newPlayer;
	if(split_number_==0){
		// first time to split
		// after first split, there will be two players in the vector
		assert(index==0);
		newPlayer.HitCard(player_.SplitCard());
		splitted_hands_.push_back(player_);
		splitted_hands_.push_back(newPlayer);
	}
	else{
		// not the first time to split
		assert(index <= split_number_ && index>=0);
		newPlayer.HitCard(splitted_hands_[index].SplitCard());
		auto it = splitted_hands_.begin() + index + 1;
		splitted_hands_.insert(it, newPlayer);
	}
	split_number_ ++;
	WHO new_winner;
	// every split will add a winner to the vector to record who is the winner
	winners_.push_back(new_winner);
	// should be true
	assert(split_number_ == splitted_hands_.size()-1);
	//splitted_ = true;
}

// deal with the player's choices
// send cards or changes bets
// return whether to continue on Dealer's Loop. 
// If the player busted, return false 
// If the player got a blackjack, return false
// Else return true
bool Game::PlayerLoop(){
	auto & player = (split_number_ == 0) ? player_ : splitted_hands_[current_hand_];
	auto & winner = (split_number_ == 0) ? winner_ : winners_[current_hand_] ;

	if(split_number_){
		cout<<endl<<"-------------------------"<<endl;
		cout<<"Now decide on your hand "<<current_hand_ << <<","<< endl;
		player.PrintCards(false);
	}
	
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
		while(invalid_input){
			// according to the specific cards the player got, prompt different choices
			if(player_first_round){
				// double down is allowed after a split
				// but surrender is not allowed after a split
				if(player.CanSplit() && split_number_ < split_limit_){
					cout << "Do you wanna Hit(h), Stand(s), DoubleDown(d), Split(t), or Surrender(r)?";
				}
				else{
					cout<< "Do you wanna Hit(h), Stand(s), DoubleDown(d), or Surrender(r)?";
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
					if(split_number_ >= split_limit_){
						cout<<"This casino has a limit of "<<split_limit_<<"splits."<<endl;
						cout<<"You have splitted for "<<split_number_<<" times"<<endl;
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
					if(split_number_){
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
				if(player.IsBusted()){
					// player got busted. return dealer as the winner
					cout<<"Oops, you busted yourself!"<<endl;
					winner = kDealer;
					return false;
				}
				// double down must exit
				if(input_char=='d') end_of_player_loop=true;
				break;
			case 'r':
				//player chose to surrender. Reduce half of his bet
				SetBet(GetBet()/2);
				cout << "You chose to surrender. Your bet is now,\t" << GetBet()<<endl;
				winner = kDealer;
				return false;
			
			// now comes the most exciting part.
			// Split the card
			case 't':
				Split(current_hand_);	// in the same time split_number_ increment by 1
				PrintSplitted();
				cout<<endl<<"A split is triggered.."<<endl;
				PlayerLoop();


				cout<<endl<<"Now decide on your second half,"<<endl;
				player_split_.PrintCards(false);
				splitted_loop_ = true;
				bool todealer2 = PlayerLoop();
				end_of_player_loop=true;
				// if the player busted himself for twice
				// then no need to enter the dealer's loop
				if(!todealer1 && !todealer2)	return false;
				break;
			default:
				break;
		}
		player_first_round = false;
	}
	return true;
}

// deal with the player's choices
// send cards
// set the winner. 
void Game::DealerLoop(){
	cout<<"-------------------------"<<endl;
	cout <<endl<<"Now the dealer's turn.."<<endl;

	// Deal's loop
	while(!dealer_.IsBusted()){
		dealer_.PrintCards(false);
		if(dealer_.WhatToDo()==kHit)
			dealer_.HitCard(mycards_.SendCard());
		else
			break;// dealer choose to stand
	}
	// reasons for exiting dealer's loop
	// 1. the dealer busted himself
	// 2. the dealer choose to stand
	if(dealer_.IsBusted()){
		dealer_.PrintCards(false);
		cout<<"Oops, the dealer busted himself!"<<endl;
		if(!splitted){
			// if the player is not splitted, the player is definitely the winner
			winner_ = kPlayer;
			return;
		}
		// if the player is splitted, need to further investigate on both hands
	}

	// Here, either the dealer choose to stand
	// or dealer busted himself but the user splitted
	// note, if the player busted himself for one of the splitted cards, that hand is considered a lose regardless whether the dealer busted or not
	// now compare the cards

	/// changes are needed to support busted conditions
	cout <<endl<<"Dealer choose to stand"<<endl;
	dealer_.PrintCards(false);
	player_.PrintCards(false);
	
}

// the main game loop
// PlayerLoop and DealerLoop are called here
// set the winner to either winner_ or splitted_winner
void Game::GameLoop(){
	// the split_number_ is dynamic
	// every split will increment it by 1
	while(current_hand_ <= split_number_){
		PlayerLoop();
		current_hand_ ++;
	}
	DealerLoop();
	// Now we need to compare all the hands with the dealer
	SetWinners();
}


// set all the winners,
// set all the winning rates
void Game::SetWinners(){
	int rate;
	if(split_number_==0){
		rate = SetWinner_GetWinningRate(player_, winner_);
		win_rate_2_.push_back(rate);
	}
	else{
		for(int i=0; i<=split_number_; i++){
			rate = SetWinner(splitted_hands_[i], winners_[i]);
			win_rate_2_.push_back(rate);
		}
	}
}

// in: a single hand
// set: the winner
// return: the winning rate
int Game::SetWinner_GetWinningRate(const & Player hand, WHO & winner){
	// blackjack is not dertermined in this step
	int rate = 0;
	int diff = 0;
	if(hand.MaxSum()>21){
		// player is busted. 
		// even the dealer is also busted, cause player busted first, 
		// the winner is still the dealer
		winner = kDealer;
		rate = -2;
	}
	else if(dealer_.MaxSum()>21){
		// dealer is busted.
		winner = kPlayer;
		rate = 2;
	}
	else if(dealer_.IsBlackJack() && !hand.IsBlackJack()){
		// this is for the boundary condition
		// that dealer got blackjack, the player got 21
		winner = kDealer;
		rate = -3;
	}
	else if(hand.IsBlackJack() && !dealer_.IsBlackJack()){
		winner = kPlayer;
		rate = 3;
	}
	else{
		// for now, no bust, no blackjack
		diff = dealer_.MaxSum() - hand.MaxSum();
		if(diff>0){
			// the dealer wins
			winner = kDealer;
			rate = -2;
		}
		else if(diff<0){
			// the player wins
			winner = kPlayer;
			rate = 2;
		}
		else{
			// it's a push
			winner = kBoth;
		}
	}
}


void Game::CloseGame(){
	// take into account both the Winners and the Blackjacks
	for(int i=1; i<=split_number_; i++){
		if(split_number_!=0){
			cout<<endl<<"Hand "<<i<<" Summary, "<<endl;
			cout<<"-------------------------"<<endl;
			cout<<"Your cards,"<<endl;
			splitted_hands_[i].PrintCards();
			cout<<"Dealer's cards,"<<endl;
			dealer_.PrintCards();
		}
		switch(winners_[i]):{
			assert(winners_[i]!=kNeither);
			case kDealer:
				cout << "The dealer wins!"<<endl;
				break;
			case kPlayer:
				cout << "You win!"<<endl;
				break;
			case kBoth:
				cout <<"It's a push!"<<endl;
			default:
				break;
		}
		// close the chips
		int profit = win_rate_2_[i] * bet_ /2
		player_.CloseMoney(profit);
		dealer_.CloseMoney(- profit);
		if(profit>0){
			cout << "You win "<<profit<<" chips."<<endl;
		}
		else if (profit<0){
			cout <<"You lose "<<profit<<" chips."<<endl;
		}
	}
	PrintChipStatus();

	// clear the cards
	dealer_.ClearCards();
	player_.ClearCards();
	splitted_hands_.clear();
	win_rate_2_.clear();
	winners_.clear();
	// reset all the values
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
		cout << endl<<"Enter your bet(enter x to exit game),";
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