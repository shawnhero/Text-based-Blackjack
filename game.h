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
	Dealer dealer_;	// the dealer

	// below two are used for unsplitted player
	Player player_;
	WHO winner_;

	// indicate whether to shuffle every round
	bool shuffle_every_round_;


	// the split limitation
	// 1 stands for there can be only 1 times of split
	int split_limit_;
	// the number of splits that the player has carried out
	int split_number_;

	// the current hand of all the splitted ones
	// this is to inform the playerloop which hand is active now
	int current_hand_;

	 
	 // this struct is to describe the status of each hand after split(s)
	struct Hand_Status{
		Player hand;
		WHO winner;
		// two times of the winning rate, use int to avoid troubles
		// this way we can handle the 3:2 profit of blackjack using elegant integers
		// e.g., push, the rate will be 0
		// e.g., player win, the rate will be 2
		// e.g., dealer win, the rate will be -2
		// the profit = bet * rate / 2
		int win_rate;
		bool isdouble;
		Hand_Status():winner(kNeither),win_rate(0),isdouble(false){};
	};
	// use vector of Hand_Status to support multiple splits
	vector< Hand_Status> hands_status_;
	
	// indicate a double action
	bool double_flag_;
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

	// deal with the dealer's choices
	// send cards
	void DealerLoop();

	// split the card
	// the new set of cards will be hold by a virtual player
	void Split(int index);

	// print all the splitted cards
	void PrintSplitted();

	// Print how many chips for the player and dealer
	void PrintChipStatus();

	// set all the winners,
	// set all the winning rates
	void SetWinners();

	// in: a single hand_status
	// set: the winner
	// return: the winning rate
	void SetWinner_WinningRate(Hand_Status & h_status);

public:
	Game():bet_(1), winner_(kNeither), shuffle_every_round_(false), split_limit_(3), split_number_(0), current_hand_(0), double_flag_(false),surrender_flag_(false){}

	// load the game configuration file
	// if file not found, use the default setting
	void LoadConfig();

	// determine whether someone is running out of money
	bool MoneyOut();

	// first try to read from configure file
	// if not found, then do some initialization
	// Hmm, maybe I'll add some encryption feature, 
	// otherwise it's just too easy for the players to cheat
	void LoadGame();

	// return the person who wins
	// if someone get a blackjack, there is no need to coninue the game
	// kBoth stands for a push, while kNeither means the game should continue 
	WHO StartGame();

	// The main game loop
	// Include first the player's loop and the dealer's loop
	void GameLoop();

	// call the SetWinners, 
	// then close the money
	void CloseGame();

	// save the current money to file (optional) and exit
	// if exit, return true, else return false
	void SaveGame();

	// prompt whether to exit. if player chooses to exit, return true
	bool PromptExit();
};