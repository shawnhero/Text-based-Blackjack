#include <vector>
#include <assert.h>
#include <random>
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
	vector<Card > m_fresh_cards;
	vector<Card > m_used_cards;
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


enum Action { Hit=0, Stand, Double, Split};
class Player{
protected:
	// here 1 stands for Ace. It could either be 1 or 11
	vector<Card > m_player_cards;
public:
	void HitCard(Card newcard);
	bool isBusted();
	// Sum may only be a valid call when it's not busted
	int Sum(bool & soft);
	virtual void PrintCards(bool firstround);
	
	virtual Action WhatToDo()=0;
};

class Dealer: public Player{
public:
	Action WhatToDo();
	void PrintCards(bool firstround);


};

class SuperGambler: public Player{
public:
	Action WhatToDo();
};