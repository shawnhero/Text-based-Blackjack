#include "cards.h"
#include <vector>

enum ACTION { kHit=0, kStand, kDouble, kSplit};
class Player{
protected:
	// here 1 stands for Ace. It could either be 1 or 11
	vector<Card > player_cards_;
	int money_in_hand_;
	struct Status{
		bool is_busted;
		bool is_sum_soft;
		int max_sum;
		bool is_blackjack;
	}status_;
public:
	void ClearCards();
	void HitCard(Card newcard);
	void UpdateStatus();
	bool IsBlackJack();
	bool IsBusted();
	int MaxSum();
	bool IsSumSoft();


	int GetMoney();
	void SetMoney(int m);
	void CloseMoney(int profit);
	Player(int initial_money):money_in_hand_(initial_money){}
	Player():money_in_hand_(100){};
	virtual void PrintCards(bool firstround);
	
	virtual ACTION WhatToDo(){return kHit;};
};

class Dealer: public Player{
public:
	ACTION WhatToDo();
	void PrintCards(bool firstround);
};

class SuperGambler: public Player{
public:
	ACTION WhatToDo();
};