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
	Dealer dealer_;

	// below two are used for unsplitted player
	Player player_;
	WHO winner_;
	// WHO split_winner_;

	// the split limitation
	// 1 stands for there can be only 1 times of split
	int split_limit_;
	int split_number_;

	// the current hand of all the splitted ones
	// this is to inform the playerloop which hand is active now
	int current_hand_;

	// use vector of Player to support multiple splits 
	vector < Player > splitted_hands_;
	vector < WHO > winners_;
	// two times of the winning rate, use int to avoid troubles
	// e.g., push, the rate will be 0
	// e.g., player win, the rate will be 2
	// e.g., dealer win, the rate will be -2
	vector <int> win_rate_2_;
	// indicate whether the player has chosen to surrender
	bool surrender_flag_;

private:
	// get/set the bet for the current round
	int GetBet() ;
	void SetBet(int bet);

	

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
	void Split(int index);

	// print all the splitted cards
	void PrintSplitted();

	// Print how many chips for the player and dealer
	void PrintChipStatus();

	// set all the winners,
	// set all the winning rates
	void SetWinners();

	// in: a single hand
	// set: the winner
	// return: the winning rate
	int SetWinner_GetWinningRate(const Player & hand, WHO & winner);

public:
	Game():bet_(1), winner_(kNeither), split_limit_(10), split_number_(0), current_hand_(0), surrender_flag_(false){}

	// load the game configuration file
	void LoadConfig();
	// determine whether someone is running out of money
	bool MoneyOut();
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