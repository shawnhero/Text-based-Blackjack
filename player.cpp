#include "player.h"
#include <iostream>
using namespace std;


void Player::HitCard(Card newcard){
	player_cards_.push_back(newcard);
	// update the card status every time after a hit
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

int Player::MaxValidSum(){
	return status_.max_valid_sum;
}

int Player::GetMoney(){
	return money_in_hand_;
}

void Player::SetMoney(int m){
	money_in_hand_ = m;
}

void Player::CloseMoney(int profit){
	money_in_hand_ += profit;
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
	cout <<"Your cards are, ";
	for(auto i:player_cards_){
		i.DisplayCard();
	}
	cout<<endl;
}

void Dealer::PrintCards(bool firstround){
	cout <<"The dealer's cards are, ";
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