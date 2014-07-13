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

	struct gamemode_{
		bool shuffle_every_hand;

	};
public:
	Game():bet_(1){}
	// first try to read from configure file
	// if not found, then do some initialization
	// Hmm, maybe I'll add some encryption feature, 
	// otherwise it's just too easy for the players to cheat
	void LoadGame();

	// set the bet for the current round
	void SetBet(int bet);

	// determine whether someone is running out of money
	bool MoneyOut();

	// return the person who wins
	// kBoth stands for a push, while kNeither means the game should continue 
	WHO StartGame();

	// The main game loop
	// Include first the player's loop and then the dealer's loop
	WHO GameLoop();

	// print the result and change the money
	void CloseGame(WHO winner);

	// Print how many chips for the player and dealer
	void PrintChipStatus();

	// save the current money to file (optional) and exit
	// if exit, return true, else return false
	void SaveGame();
	bool PromptExit();
};