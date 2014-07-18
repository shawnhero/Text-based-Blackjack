// Author: Shawn Wu
// Email:  wuxu@cs.ucla.edu

// Describe one or more decks of cards

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

//enum TESTCARDS{kSplitHell, kNormal};
// Describe one single card 
struct Card{
	int num;
	int color;
	Card(int i, int j): num(i), color(j){
		assert(i>=1 && i<=13);
		assert(j>=0 && j<=3);
	}
	void DisplayCard();
};

// Describe one or more decks of cards used in the game
class Cards{
public:
	// Initialize one deck of cards
	Cards();
	
	// Shuffle a card
	void Shuffle();

	// Send a card from the current deck(s) of cards
	// Return the card sent
	Card SendCard();

	// Specify how many decks of cards should be used
	void SetDeckNum(int num);
	// debug use
	void PrintAllFreshCards();
private:
	vector<Card > fresh_cards_;	// unused cards
	vector<Card > used_cards_;	// used cards
	
	// This method is to be called in the Shuffle method. To ensure the shuffle is random.
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