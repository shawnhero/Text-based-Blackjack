// Author: Shawn Wu
// Email:  wuxu@cs.ucla.edu

// Provides a class describing the player's status
// 1. Playerer is the base class. It is used to describe the user of this program;
// 2. Dealer is inherited from Player to describe the dealer, which is controlled by the computer.
// 3. SuperGambler is inherited from Player. In the analysis mode it can be used to simulate a player taking certain strategies.
#include "cards.h"
#include <vector>

// The initial money of the player and the dealer
const int kPlayerChips = 100;
const int kDealerChips = 10000;

// The decision for a player/dealer after each deal
enum ACTION { kHit=0, kStand, kDouble, kSplit};

class Player{
protected:

	// Use a vector to describe the cards in hand
	// here 1 stands for Ace. However, it could either be 1 or 11
	// 11,12,13 stand for J,Q,K, respectively
	vector<Card > player_cards_;
	vector<Card > player_cards2_;

	// Remaining chips in hand for the player
	int chips_in_hand_;

	// Some values describing the status of the player after each deal
	struct Status{
		bool is_busted;
		bool is_sum_soft;
		int max_sum;
		bool is_blackjack;
	}status_, status2_;
public:
	// When a Player is initialized, set the chips to the default number
	Player():chips_in_hand_(kPlayerChips){}

	// After each hand, the cards need to be cleared
	void ClearCards();

	// The player choose to hit card(or double card)
	// add a card the player_cards_
	void HitCard(Card newcard);

	// Get the Updated Status
	void UpdateStatus();

	// Determine whether it's a blakcjack from Status
	bool IsBlackJack() const;

	// Determine whether it's busted from Status
	bool IsBusted() const;

	// Get the Max sum of the cards from Status
	int MaxSum() const;


	// Return the current chips left for the player
	int GetChips();
	// Set the initial chips. This will be called when a saved game is loaded
	void SetChips(int m);

	// After each hand, the player/dealer either win some chips or lose some.
	// The profit will be added to chips_in_hand_
	void CloseMoney(int profit);

	// must be called only to evaluate whether to split
	bool CanSplit();


	// split the player's cards to two 
	// pop out one card and return it.
	// must be called when the player has exactly two identical cards
	Card SplitCard();

	// // below are some overloaded methods provided for the split

	// // Get called if previously the user has chosen to split the card.
	// // the second parameter determines which set of card to hit.
	// void HitCard(Card newcard, bool isfirst);
	// void UpdateStatus(bool isfirst);
	// bool IsBlackJack(bool isfirst);
	// bool IsBusted(bool isfirst);
	// int MaxSum(bool isfirst);

	virtual void PrintCards(bool firstround);
	
	void PrintCards(){PrintCards(false);}
};

class Dealer: public Player{
public:
	Dealer(){chips_in_hand_ = kDealerChips;}

	// According the current status, what's the right decision to make?
	// This describes the dealer's strategy. 
	// Here the soft 17 rule (See http://www.smartgaming.com/html/articles/soft17.htm) is applied.
	ACTION WhatToDo();
	void PrintCards(bool firstround);

	void PrintCards(){PrintCards(false);}
};

class SuperGambler: public Player{
public:
	// This simulates the player's strategy
	ACTION WhatToDo();
};