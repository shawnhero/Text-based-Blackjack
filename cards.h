#include <vector>
#include <assert.h>
#include <random>

#if defined(_WIN64) || defined(_WIN32)
//define something for Windows
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
#include <ctime>
#endif


using namespace std;
struct Card{
	int num;
	int color;
	Card(int i, int j): num(i), color(j){
		assert(i>=1 && i<=13);
		assert(j>=0 && j<=3);
	}
	void DisplayCard();
};

class Cards{
public:
	Cards();
	void Shuffle();
	Card SendCard();
	void PrintAllFreshCards();
private:
	vector<Card > fresh_cards_;
	vector<Card > used_cards_;
	struct Gen {
		mt19937 g;
		Gen(): g(static_cast<uint32_t>(time(0))){}
		size_t operator()(size_t n)
		{
			std::uniform_int_distribution<size_t> d(0, n ? n-1 : 0);
			return d(g);
		}
	};
};


enum ACTION { kHit=0, kStand, kDouble, kSplit};
class Player{
protected:
	// here 1 stands for Ace. It could either be 1 or 11
	vector<Card > player_cards_;
	int money_in_hand;
	struct Status{
		bool is_busted;
		bool is_sum_soft;
		int max_valid_sum;
		bool is_blackjack;
	}status_;
public:
	
	void HitCard(Card newcard);
	void UpdateStatus();
	bool IsBlackJack();
	bool IsBusted();
	int MaxValidSum();
	bool IsSumSoft();

	void GetMoney();
	void SetMoney();
	Player(int initial_money):money_in_hand(initial_money){}
	Player():money_in_hand(100){};
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