// Author: Shawn Wu
// Email:  wuxu@cs.ucla.edu

// Describe one or more decks of cards

#include "cards.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>        // std::time

using namespace std;

void Card::DisplayCard(){
	const char spade[] = "\xe2\x99\xa0";
	const char heart[] = "\xe2\x99\xa1";
	const char diamond[] = "\xe2\x99\xa2";
	const char club[] = "\xe2\x99\xa3";
	switch(color){
		case 0:
			cout<< spade;
			break;
		case 1:
			cout<< heart;
			break;
		case 2:
			cout<< diamond;
			break;
		case 3:
			cout<< club;
			break;
		default:
			break;
	}
	if(num<=10){
		cout<<" "<<num<<"\t";	
	}
	else{
		switch(num){
			case 11:
				cout<<" "<<"J\t";
				break;
			case 12:
				cout<<" "<<"Q\t";
				break;
			case 13:
				cout<<" "<<"K\t";
				break;	
		}
	}
}

Cards::Cards(){
	//type_test_cards = kNormal;
	for(int i=0; i<52; i++){
		Card newcard(i/4+1, i%4);
		fresh_cards_.push_back(newcard);
	}
	used_cards_.clear();
	Shuffle();
}

void Cards::SetDeckNum(int num){
	fresh_cards_.clear();
	used_cards_.clear();
	if(num<=0) num = 1;
	for(int j=0; j<=num; j++){
		for(int i=0; i<52; i++){
			Card newcard(i/4+1, i%4);
			fresh_cards_.push_back(newcard);
		}

	}
	Shuffle();
}


// for debug and test use
void Cards::PrintAllFreshCards(){
	for(auto i:fresh_cards_){
		i.DisplayCard();
	}
}


void Cards::Shuffle(){
	// switch(type_test_cards){
	// 	case kSplitHell:
	// 		break;
	// 	default:

	// first merge two parts into one, and clear the used cards
	fresh_cards_.insert(fresh_cards_.end(), used_cards_.begin(), used_cards_.end());
	used_cards_.clear();
	// then do the shuffle
	// if Gen() is not set, everytime the result will be the same
	random_shuffle(fresh_cards_.begin(), fresh_cards_.end(), Gen());
	//break;
	//}

}

Card Cards::SendCard(){
	// if the number of cards available is less than 15, do a shuffle
	if(fresh_cards_.size()<=15){
		cout<<"-------------------------"<<endl;
		cout<<"A shuffle is triggered..."<<endl;
		cout<<"-------------------------"<<endl;
		Shuffle();
	}
	used_cards_.push_back(fresh_cards_.back());
	fresh_cards_.pop_back();
	return used_cards_.back();
}

