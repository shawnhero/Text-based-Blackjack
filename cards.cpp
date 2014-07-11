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
	for(int i=0; i<52; i++){
		Card newcard(i/4+1, i%4);
		m_fresh_cards.push_back(newcard);
	}
	m_used_cards.clear();
}

void Cards::PrintAllFreshCards(){
	for(auto i:m_fresh_cards){
		i.DisplayCard();
	}
}


void Cards::Shuffle(){
	// first merge two parts into one, and clear the used cards
	m_fresh_cards.insert(m_fresh_cards.end(), m_used_cards.begin(), m_used_cards.end());
	m_used_cards.clear();
	// then do the shuffle
	// if Gen() is not set, everytime the result will be the same
	random_shuffle(m_fresh_cards.begin(), m_fresh_cards.end(), Gen());
}

Card Cards::SendCard(){
	// if the number of cards available is less than 15, do a shuffle
	if(m_fresh_cards.size()<=15){
		cout<<"-------------------------"<<endl;
		cout<<"A shuffle is triggered..."<<endl;
		cout<<"-------------------------"<<endl;
		Shuffle();
	}
	m_used_cards.push_back(m_fresh_cards.back());
	m_fresh_cards.pop_back();
	return m_used_cards.back();
}

void Player::HitCard(Card newcard){
	m_player_cards.push_back(newcard);
}

bool Player::isBusted(){
	// to deal with Ace
	// always use Ace as 1 to determine whether it's busted
	int sum=0;
	for(auto i:m_player_cards){
		int value=i.num;
		value=(value>10)?10:value;
		sum += value;
	}
	return sum>21;
}

int Player::Sum(bool & soft){
	// to deal with Ace
	// first try to use it as 11, 
	// if it's busted, then use it as 1.
	int sum=0;
	int acenum = 0;
	for(auto i:m_player_cards){
		int value=i.num;
		// J,Q,K are treated as 10
		value=(value>10)?10:value;
		if(value==1){
			acenum++;
		}
		sum += value;
	}
	// acenum of 1,2,3,4 are all soft
	soft = (acenum!=0);
	// try to add 10 until almost bust
	while(acenum>0){
		sum += 10;
		if(sum>21){
			sum -= 10;
			break;
		}
	}
	return sum;
}

Action Dealer::WhatToDo(){
	// to deal with soft 17
	// choose to hit
	bool soft;
	int sum = Sum(soft);
	if(sum>=17  && !soft) return Hit;
	else	return Stand;

}

void Player::PrintCards(bool firstround){
	for(auto i:m_player_cards){
		i.DisplayCard();
	}
	cout<<endl;
}

void Dealer::PrintCards(bool firstround){
	if(!firstround){
		for(auto i:m_player_cards){
			i.DisplayCard();
		}
		cout<<endl;
	}
	else{
		// for the first round, the second one must be hidden
		assert(m_player_cards.size()==2);
		m_player_cards[0].DisplayCard();
		cout<<"second card unknown..."<<endl;
	}
}

Action SuperGambler::WhatToDo(){
	return Hit;
}
