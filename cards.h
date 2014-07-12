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