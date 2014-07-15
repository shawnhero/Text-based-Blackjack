// Author: Shawn Wu
// Email:  wuxu@cs.ucla.edu

// Provide a class to simulate all the processes in a hand of the game
#include "player.h"

enum GAMEMODE { kPlayerVSDealer=0, kSuperGamblerVSDealer};
enum WHO{ kDealer=0, kPlayer, kBoth, kNeither};
class Game{
private:
	int bet_;	// the money the player bet on this hand
	Cards mycards_;	// the current deck of cards
	Player player_;
	Dealer dealer_;

	// the split limitation
	// 1 stands for there can be only 1 times of split
	int split_limit_;

	// the current hand of all the splitted ones
	// this is to inform the playerloop which hand is active now
	int current_hand_;
	int split_number_;

	vector < Player > splitted_hands_;
	vector < WHO > winners_;
	// two times of the winning rate
	// use int to avoid troubles
	vector <int> win_rate_2_;

	bool splitted_;
	bool splitted_loop_;
	WHO winner_;
	WHO split_winner_;

	struct gamemode_{
		bool shuffle_every_hand;

	};

private:
	// get/set the bet for the current round
	int GetBet();
	void SetBet(int bet);

	// determine whether someone is running out of money
	bool MoneyOut();

	// deal with the player's choices
	// send cards or changes bets
	// return whether to continue on Dealer's Loop. 
	// If the player busted, return false 
	// If the player got a blackjack, return false
	// Else return true
	bool PlayerLoop();

	// deal with the player's choices
	// send cards
	// set the winner. 
	void DealerLoop();

	// split the card
	// the new set of cards will be hold be a virtual player: player_split
	void Split();

	// print all the splitted cards
	void PrintSplitted();

	// Print how many chips for the player and dealer
	void PrintChipStatus();

public:
	Game():bet_(1), splitted_(false), winner_(kNeither), split_winner_(kNeither), split_limit_(1), split_number_(0), current_hand_(0){}
	// first try to read from configure file
	// if not found, then do some initialization
	// Hmm, maybe I'll add some encryption feature, 
	// otherwise it's just too easy for the players to cheat
	void LoadGame();

	// return the person who wins
	// kBoth stands for a push, while kNeither means the game should continue 
	WHO StartGame();

	// The main game loop
	// Include first the player's loop and then the dealer's loop
	void GameLoop();

	// print the result and change the money
	void CloseGame();

	// save the current money to file (optional) and exit
	// if exit, return true, else return false
	void SaveGame();
	bool PromptExit();
};