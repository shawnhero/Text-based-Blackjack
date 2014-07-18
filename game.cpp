// Author: Shawn Wu
// Email:  wuxu@cs.ucla.edu

// Provide a class to simulate all the processes in a hand of the game

#include "game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

// determine whether a string is a valid number
inline bool is_number(const std::string& s)
{
    auto it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

// load the game configuration file
// if file not found, use the default setting
void Game::LoadConfig(){
	ifstream myfile("bjconfig.dat");
	if(!myfile.good())	return;
	
	// file is good, read config to maps
	unordered_map<string, int> mymap;
	string line;
	while ( getline (myfile,line) )
    {
      unsigned int pos=0;
      for(auto i:line){
      	if(i==' ' || i=='\t') break;
      	pos++;
      }
      string line1 = line.substr(0, pos);
      string line2 = line.substr(pos+1, line.size()-pos-1);
      if(is_number(line2)){
      	mymap[line1] = stoi(line2);
      }
      else{
      	// bad file format, no need to set up
      	return;
      }
    }
    myfile.close();

    cout <<endl<< "Settings Loaded,"<<endl;
    cout<<"-------------------------"<<endl;
    // set the deck numer
	if(mymap["DeckNum"]>=1){
		mycards_.SetDeckNum(mymap["DeckNum"]);
		cout << "Number of Decks set to, "<< mymap["DeckNum"]<<endl;
	}

    // set the split limit
    if(mymap["SplitLimit"] >=0){
    	split_limit_ = mymap["SplitLimit"];
    	cout <<"Split Limit set to, " << split_limit_<<endl;
    }

    // set shuffle
	shuffle_every_round_ = (bool) mymap["ShuffleEveryRound"];
	cout << "Shuffle Every Round set to, " << shuffle_every_round_ <<endl;
	
	// set hit rule
	if(mymap["HitSoft17"]==0 || mymap["HitSoft17"] ==1){
		dealer_.SetHitSoft((bool)mymap["HitSoft17"]);
		cout << "Hit Soft 17 set to, "<< (bool)mymap["HitSoft17"] <<endl;
	}
	cout <<endl;

}


// first try to read from configure file
// if not found, then do some initialization
// Hmm, maybe I'll add some encryption feature, 
// otherwise it's just too easy for the players to cheat
void Game::LoadGame(){
	// first try to read from save.data
	// Hmm, maybe I'll add some encryption feature, 
	// otherwise it's just too easy for the players to cheat
	while(true){
		cout<<"Load Last Saved Game?(y/n)";
		string input;
		cin>>input;
		// prevent the ctrl+D hell
		if(cin.eof()){
			cout << "Hate ctrl-D hell\n";
			std::exit(EXIT_FAILURE);
		}

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
				else if(dchips<=0){
					cout<<"Last time you won all the money from the dealer. A new game is started.."<<endl;
				}
				else if(pchips + dchips == (kPlayerChips+kDealerChips)){
					player_.SetChips(pchips);
					dealer_.SetChips(dchips);
				}
				else{
					cout <<"Incorrect file format. Creating new game profile.."<<endl;
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


// determine whether someone is running out of money
bool Game::MoneyOut(){
	if(player_.GetChips()<=0){
		cout<<"You have no money left. Game Over."<<endl;
		// save the game status
		// next time chips will be reset upon the user load the saved game
		SaveGame();
		return true;
	}
	else if(dealer_.GetChips()<=0){
		cout<<"You have won all the money from the dealer! Good Job!"<<endl;
		// save the game status
		// next time chips will be reset upon the user load the saved game
		SaveGame();
		return true;
	}
	
	return false;
}


// return the person who wins
// if someone get a blackjack, there is no need to coninue the game
// kBoth stands for a push, while kNeither means the game should continue 
WHO Game::StartGame(){

	cout<<endl<<"Starting a new round.."<<endl;
	cout<<"-------------------------"<<endl;
	// shuffle for every game
	if(shuffle_every_round_){
		cout<<"Shuffling.."<<endl;
		mycards_.Shuffle();
	}
	dealer_.HitCard(mycards_.SendCard());
	dealer_.HitCard(mycards_.SendCard());
	player_.HitCard(mycards_.SendCard());
	player_.HitCard(mycards_.SendCard());

	int bj = dealer_.IsBlackJack() + 2*player_.IsBlackJack();
	switch(bj){
		case 1:
			cout <<"Dealer got a BlackJack!"<<endl;
			// dealer_.PrintCards(false);
			// player_.PrintCards(true);
			winner_ = kDealer;
			return kDealer;
		case 2:
			cout <<"You got a BlackJack!"<<endl;
			// player_.PrintCards(true);
			// dealer_.PrintCards(false);
			winner_ = kPlayer;
			return kPlayer;
		case 3:
			cout <<"Both of you have a BlackJack! It's a push."<<endl;
			// player_.PrintCards(true);
			// dealer_.PrintCards(false);
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
	Hand_Status oldHand, newHand;
	if(split_number_==0){
		// first time to split
		// after first split, there will be two players in the vector
		assert(index==0);
		newHand.hand.HitCard(player_.SplitCard());
		newHand.hand.HitCard(mycards_.SendCard());
		player_.HitCard(mycards_.SendCard());
		oldHand.hand = player_;
		hands_status_.push_back(oldHand);
		hands_status_.push_back(newHand);
	}
	else{
		// not the first time to split
		assert(index <= split_number_ && index>=0);
		newHand.hand.HitCard(hands_status_[index].hand.SplitCard());
		newHand.hand.HitCard(mycards_.SendCard());
		hands_status_[index].hand.HitCard(mycards_.SendCard());

		auto it = hands_status_.begin() + index + 1;
		hands_status_.insert(it, newHand);
	}
	split_number_ ++;
	// every split will add a winner to the vector to record who is the winner
	// should be true
	//assert(split_number_ ==(int) splitted_hands_.size()-1);
}

// deal with the player's choices
// send cards or changes bets
// return whether to continue on Dealer's Loop. 
// If the player busted, return false 
// If the player got a blackjack, return false
// Else return true
bool Game::PlayerLoop(){
	auto & player = (split_number_ == 0) ? player_ : hands_status_[current_hand_].hand;
	auto & winner = (split_number_ == 0) ? winner_ : hands_status_[current_hand_].winner ;

	

	if(split_number_){
		// when splitted, it is possible that this loop is entered 
		// even when the player got a blackjack
		// need to check that
		if(player.IsBlackJack()){
			cout <<"You got a BlackJack!"<<endl;
			// go on the dealer's loop
			return true;
		}
		else{
			cout<<endl<<"-------------------------"<<endl;
			cout<<"Now decide on your hand "<<current_hand_  <<","<< endl;
			player.PrintCards(false);
		}
		
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
					if(split_number_==0)
						cout << "Do you wanna Hit(h), Stand(s), DoubleDown(d), Split(t), or Surrender(r)?";
					else
						cout << "Do you wanna Hit(h), Stand(s), DoubleDown(d), or Split(t)?";
				}
				else{
					if(split_number_==0){
						cout<< "Do you wanna Hit(h), Stand(s), DoubleDown(d), or Surrender(r)?";
					}
					else{
						cout<< "Do you wanna Hit(h), Stand(s), or DoubleDown(d)?";
					}
				}
				 
			}
			else{
				cout<< "Do you wanna Hit(h), or Stand(s)?";
			}
			// Get and evaluate the player's input
			cin >> input;
					// prevent the ctrl+D hell
			if(cin.eof()){
				cout << "Hate ctrl-D hell\n";
				std::exit(EXIT_FAILURE);
			}
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
						cout<<"This casino has a limit of "<<split_limit_<<" split(s)."<<endl;
						cout<<"You have splitted for "<<split_number_<<" time(s)"<<endl;
						cout<<"You cannot split any more."<<endl;
						break;
					}
					if(!player.CanSplit()){
						cout<<"You can Split only when you have two cards of the same value."<<endl;
						break;
					}
					//current_hand_ --;
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
						break;
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
				if(split_number_==0){
					double_flag_ = true;
				}
				else{
					hands_status_[current_hand_].isdouble = true;
				}
				// SetBet(2*GetBet());
				cout <<endl<< "You chose to Double."<<endl;
				cout<<"Your bet is now, " << 2*GetBet()<<endl<<endl;
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
				// if the sum is already 21, no need for the user for further actions
				if(input_char=='d' || player.MaxSum()==21) end_of_player_loop=true;
				break;
			case 'r':
				//player chose to surrender. Reduce half of his bet
				SetBet(GetBet()/2);
				cout <<endl<< "You chose to Surrender."<<endl;
				cout<<"-------------------------"<<endl;
				cout<<"Your bet is now, " << GetBet()<<endl<<endl;
				winner = kDealer;
				surrender_flag_ = true;
				return false;
			
			// now comes the most exciting part.
			// Split the card
			case 't':
				Split(current_hand_);	// in the same time split_number_ increment by 1
				cout<<endl<<"A split is triggered.."<<endl;
				PrintSplitted();
				PlayerLoop();
				end_of_player_loop=true;
				// cout<<endl<<"Now decide on your second half,"<<endl;
				// player_split_.PrintCards(false);
				// splitted_loop_ = true;
				// bool todealer2 = PlayerLoop();
				// end_of_player_loop=true;
				// // if the player busted himself for twice
				// // then no need to enter the dealer's loop
				// if(!todealer1 && !todealer2)	return false;
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
void Game::DealerLoop(){
	cout <<endl<<"Now the dealer's turn.."<<endl;
	cout<<"-------------------------"<<endl;

	// Deal's loop
	while(!dealer_.IsBusted()){
		dealer_.PrintCards(false);
		if(dealer_.WhatToDo()==kHit)
			dealer_.HitCard(mycards_.SendCard());
		else
			break;// dealer choose to stand
	}
	// causes of exiting dealer's loop
	// 1. the dealer busted himself
	// 2. the dealer choose to stand
	if(dealer_.IsBusted()){
		dealer_.PrintCards(false);
		cout<<"Oops, the dealer busted himself!"<<endl;
		if(split_number_==0){
			// if the player is not splitted, the player is definitely the winner
			winner_ = kPlayer;
			
		}
		return;
	}
	cout <<endl<<"Dealer choose to stand."<<endl;
}

// the main game loop
// PlayerLoop and DealerLoop are called here
void Game::GameLoop(){
	// the split_number_ is dynamic
	// every split will increment it by 1
	bool need_dealer=true;
	while(current_hand_ <= split_number_){
		need_dealer = PlayerLoop();
		current_hand_ ++;
	}
	if(need_dealer || split_number_!=0)	DealerLoop();
}


// set all the winners,
// set all the winning rates
void Game::SetWinners(){
	if(split_number_==0){
		Hand_Status newHand;
		newHand.hand = player_;
		newHand.isdouble = double_flag_;
		hands_status_.push_back(newHand);
		if(surrender_flag_){
			hands_status_[0].winner = kDealer;
			hands_status_[0].win_rate = -2;
		}
		else{
			SetWinner_WinningRate(hands_status_[0]);
		}
		
	}
	else{
		for(int i=0; i<=split_number_; i++){
			SetWinner_WinningRate(hands_status_[i]);
		}
	}
}

// in: a single hand
// set: the winner
// return: the winning rate
void Game::SetWinner_WinningRate(Hand_Status & h_status){
	// blackjack is not dertermined in this step
	auto & hand = h_status.hand;
	auto & winner = h_status.winner;
	auto & rate = h_status.win_rate;

	int diff = 0;
	if(dealer_.IsBlackJack() && !hand.IsBlackJack()){
		// this is for the boundary condition
		// that dealer got blackjack, the player got 21
		winner = kDealer;
		rate = -3;
	}
	else if(hand.IsBlackJack() && !dealer_.IsBlackJack()){
		winner = kPlayer;
		rate = 3;
	}
	else if(hand.MaxSum()>21){
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

	rate = h_status.isdouble?(2*rate):rate;
	return;
}


// call the SetWinners, 
// then close the money
void Game::CloseGame(){
	// Now we need to compare all the hands with the dealer
	SetWinners();
	// take into account both the Winners and the Blackjacks
	for(int i=0; i<=split_number_; i++){
		if(split_number_!=0){
			cout<<endl<<"Hand "<<i<<" Summary, "<<endl;
			cout<<"-------------------------"<<endl;
			hands_status_[i].hand.PrintCards();
			dealer_.PrintCards();
		}
		else{
			cout << endl<< "Summary,"<<endl;
			cout<<"-------------------------"<<endl;
			player_.PrintCards(false);
			dealer_.PrintCards(false);
			cout <<endl;
		}
		switch(hands_status_[i].winner){
			assert(hands_status_[i].winner!=kNeither);
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
		int profit = hands_status_[i].win_rate * bet_ /2;
		player_.CloseMoney(profit);
		dealer_.CloseMoney(- profit);
		if(profit>0){
			cout  << "You win "<<profit<<" chips";
			if(hands_status_[i].isdouble){
				cout <<" (doubled)";
			}
			cout << "." << endl;
		}
		else if (profit<0){
			cout  << "You lose "<<-profit<<" chips";
			if(hands_status_[i].isdouble){
				cout <<" (doubled)";
			}
			cout << "." << endl;

		}
	}
	PrintChipStatus();

	// clear the cards
	// reset all the values
	dealer_.ClearCards();
	player_.ClearCards();
	hands_status_.clear();
	current_hand_ = 0;
	split_number_ = 0;
	double_flag_ = false;
	surrender_flag_ = false;
}




// print all the splitted cards
void Game::PrintSplitted(){
	cout<<endl<<"Splitted Hands Summary,"<<endl;
	cout<<"-------------------------"<<endl;
	int j = 0;
	for(auto & i:hands_status_){
		cout << "Hand "<<j<<endl;
		i.hand.PrintCards();
		j++;
	}

	
}

// Print how many chips for the player and dealer
void Game::PrintChipStatus(){
	cout<<endl<<"-------------------------"<<endl;
	cout<<"Your chips, "<<player_.GetChips()<<endl;
	cout<<"Dealer's chips, "<<dealer_.GetChips()<<endl;
	cout<<"-------------------------"<<endl;
}

// prompt whether to exit. if player chooses to exit, return true
bool Game::PromptExit(){
	// prompt exit
	// also let the player set a new bet if he chooses to contunue
	string input;
	int bet;
	while(true){
		cout << endl<<"Enter your bet(enter x to exit game),";
		cin>>input;
		// prevent the ctrl+D hell
		if(cin.eof()){
			cout << "Hate ctrl-D hell\n";
			std::exit(EXIT_FAILURE);
		}
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