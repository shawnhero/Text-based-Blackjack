// Author: Shawn Wu
// Email:  wuxu@cs.ucla.edu

// Provides a class describing the player's status
// 1. Playerer is the base class. It is used to describe the user of this program;
// 2. Dealer is inherited from Player to describe the dealer, which is controlled by the computer.
// 3. SuperGambler is inherited from Player. In the analysis mode it can be used to simulate a player taking certain strategies.

#include "player.h"
#include <iostream>
using namespace std;

// After each hand, the cards need to be cleared
void Player::ClearCards(){
	player_cards_.clear();
	player_cards2_.clear();
}

// The player choose to hit card
// add a card the player_cards_
void Player::HitCard(Card newcard){
	player_cards_.push_back(newcard);
	// update the card status every time after a hit
	UpdateStatus();
}

// Get the Updated Status
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
		status_.max_sum = sum;
		status_.is_blackjack = false;
	}
	else{
		// if not busted, determine the sum and soft status
		status_.is_busted = false;
		// the point is soft if there is an ace treated as 11
		status_.is_sum_soft = (ace_num!=0) && (sum<=11);
		// try to add 10 until almost bust
		while(ace_num>0){
			sum += 10;
			if(sum>21){
				sum -= 10;
				break;
			}
		}
		status_.max_sum = sum;
		status_.is_blackjack = (sum==21) && (player_cards_.size()==2);
	}
}

// Determine whether it's a blakcjack from Status
bool Player::IsBlackJack() const {
	return status_.is_blackjack;

}

// Determine whether it's busted from Status
bool Player::IsBusted() const {
	return status_.is_busted;
}

// Get the Max sum of the cards from Status
int Player::MaxSum() const {
	return status_.max_sum;
}

int Player::GetChips(){
	return chips_in_hand_;
}

void Player::SetChips(int m){
	chips_in_hand_ = m;
}

// must be called only to evaluate whether to split
bool Player::CanSplit(){
	if(!  (player_cards_.size()==2 && player_cards2_.empty() )  ){
		return false;
	}
	if(player_cards_[0].num<10 && player_cards_[1].num<10){
		return player_cards_[0].num==player_cards_[1].num;
	}
	else if (player_cards_[0].num>=10 && player_cards_[1].num>=10){
		return true;
	}
	return false;
}

// split the player's cards to two parts
// must be called when the player has exactly two identical cards
Card Player::SplitCard(){
	assert(player_cards_.size()==2 && player_cards2_.empty());
	auto newcard = player_cards_.back();
	player_cards_.pop_back();
	UpdateStatus();
	return newcard;
}


void Player::CloseMoney(int profit){
	chips_in_hand_ += profit;
}

void Dealer::SetHitSoft(bool flag){
	hit_soft_17_ = flag;
}
ACTION Dealer::WhatToDo(){
	// to deal with soft 17: choose to hit
	bool tohit = status_.max_sum<=16 || (status_.max_sum==17 && status_.is_sum_soft && hit_soft_17_);
	if(tohit){
		return kHit;
	}
	else{
		return kStand;
	}
}

// to add: hint blackjack
void Player::PrintCards(bool firstround){
	cout <<"Player:\t";
	for(auto i:player_cards_){
		i.DisplayCard();
	}
	cout<<endl;
	cout <<"Sum:   \t";
	cout << MaxSum()<<endl;

}


void Dealer::PrintCards(bool firstround){
	cout <<"Dealer:\t";
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
		cout<<"Unknown"<<endl;
	}
	cout <<"Sum:   \t";
	if(!firstround){
		cout << MaxSum()<<endl;
	}
	else{
		cout <<"Unknown"<<endl;
	}
}

ACTION SuperGambler::WhatToDo(){
	return kHit;
}