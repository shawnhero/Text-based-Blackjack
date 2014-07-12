#include "player.h"

enum GAMEMODE { kPlayerDealer=0, kSuperGambler};
enum WHO{ kDealer=0, kPlayer, kBoth, kNeither};
class Game{
private:
	int total_money_;
	int bet_;
	Cards mycards_;
	Player player_;
	Dealer dealer_;
public:
	// first try to read from configure file
	// if not found, then do some initialization
	// Hmm, maybe I'll add some encryption feature, 
	// otherwise it's just too easy for the players to cheat
	void Configure();

	// return the person who wins
	// kBoth stands for a push, while kNeither means the game should continue 
	WHO StartGame();

	// given who to detect, return who has a blackjack
	// who_to_detect is passed into the method purely for efficiency. because in many situations we know who can or cannot have a blackjack.
	WHO DetectBlackJack(WHO who_to_detect);
	WHO GameLoop();

	// print the result and change the money
	void CloseGame(WHO winner);
	void PrintMoneyStatus();

	// save the current money to file (optional) and exit
	void SaveGame(bool save);
};