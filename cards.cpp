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
		fresh_cards_.push_back(newcard);
	}
	used_cards_.clear();
}

void Cards::PrintAllFreshCards(){
	for(auto i:fresh_cards_){
		i.DisplayCard();
	}
}


void Cards::Shuffle(){
	// first merge two parts into one, and clear the used cards
	fresh_cards_.insert(fresh_cards_.end(), used_cards_.begin(), used_cards_.end());
	used_cards_.clear();
	// then do the shuffle
	// if Gen() is not set, everytime the result will be the same
	random_shuffle(fresh_cards_.begin(), fresh_cards_.end(), Gen());
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

void Player::HitCard(Card newcard){
	player_cards_.push_back(newcard);
	UpdateStatus();
}

void Player::UpdateStatus(){
	// first determin whether it's busted
	int sum=0;
	int ace_num = 0;
	for(auto i:player_cards_){
		int card_point=i.num;
		// J,Q,K are treated as 10
		card_point=(card_point>10)?10:card_point;
		// get the number of aces
		if(card_point==1){
			ace_num++;
		}
		// always use Ace as 1 to determine whether it's busted
		sum += card_point;
	}
	if(sum>21){
		status_.is_busted = true;
		status_.is_sum_soft = false;
		status_.max_valid_sum = 0;
		status_.is_blackjack = false;
	}
	else{
		// if not busted, determine the sum and soft status
		status_.is_busted = false;
		// acenum of 1,2,3,4 are all soft
		status_.is_sum_soft = (acenum!=0);
		// try to add 10 until almost bust
		while(ace_num>0){
			sum += 10;
			if(sum>21){
				sum -= 10;
				break;
			}
		}
		status_.max_valid_sum = sum;
		status_.is_blackjack = (sum==21);
	}
}

bool Player::IsBlackJack(){
	return status_.is_blackjack;

}

bool Player::IsBusted(){
	return status_.is_busted;
}

void Player::MaxValidSum(){
	return status_.max_valid_sum;
}

ACTION Dealer::WhatToDo(){
	// to deal with soft 17: choose to hit
	if(status_.max_valid_sum>17 && !status_.is_sum_soft){
		return kStand;
	}
	else{
		return kHit;
	}
}

void Player::PrintCards(bool firstround){
	for(auto i:player_cards_){
		i.DisplayCard();
	}
	cout<<endl;
}

void Dealer::PrintCards(bool firstround){
	if(!firstround){
		for(auto i:player_cards_){
			i.DisplayCard();
		}
		cout<<endl;
	}
	else{
		// for the first round, the second one must be hidden
		assert(player_cards_.size()==2);
		player_cards_[0].DisplayCard();
		cout<<"second card unknown..."<<endl;
	}
}

ACTION SuperGambler::WhatToDo(){
	return kHit;
}
