text-based-BlackJack
====================

This is a simple text-based blackjack written fully in C++. Most of the key features of Blackjack are fulfilled.

- A much simpler version can be found [here](../easy_mode)
- A seperate [branch](../auto_analysis) is created in an attempt to analyze how the factors affect the winning probabilities, along with what is right thing to do for the players. I'm also considering to get the max winning probability (rate) a player can reach, given that there's only one deck of cards and the player is capabale of memorizing all the used cards.

#Key Features
##Game Mode (Dealer's Perspective)
1. Choose how many decks are used in a game.
2. Shuffle Rules. Either a shuffle is triggered every hand or it's not triggered until there're 15 or less cards left.
3. [Soft 17 Rule](http://www.smartgaming.com/html/articles/soft17.htm). Choose whether the deal stands when he has a soft 17.
4. Split limits. The maximum number that a player can split.

All the above are stored in a file named "game.config". If not found, the default values will be used, which are respectively,

1. Only 1 deck of cards is used.
2. A shuffle is triggered only when the cards are not enough to use (15 or less).
3. Dealer hits when the 17 is soft.
4. The player can split no more than 3 times.


##Game Rules
###Player Decisions
1. **Hit**. Take another card from the dealer
2. **Stand**. Take no more cards.
3. **DoubleDown**. The player is allowed to increase the initial bet by up to 100% in exchange for committing to stand after receiving exactly one more card.
4. **Split** (only available as the first decision of a hand). If the first two cards have the same value, the player can split them into two hands, by moving a second bet equal to the first into an area outside the betting box. The dealer separates the two cards and draws an additional card on each, placing one bet with each hand. The player then plays out the two separate hands in turn.
	> After a split, the player can still double on the first round, but he can no longer split. The number of split limits is by default 3, but can be changed in the `game.config` file.
	
	> Example of mutiple split,
	
	>![](http://shawnhero.github.io/img/bj_split.png)
	
5. **Surrender** (only available as first decision of a hand). After the dealer has checked for blackjack, the user can choose to surrender. By doing so, the house takes half the player's bet and returns the other half to the player.

###Dealer's Decisions
1. **Hit**. If the dealer's cards add equal to or less than 16, he must choose to hit.
2. **Stand**. If the dealer's cards add equal to or more than 17, he must choose to stand.

###Beat the Dealer
The player can beat the dealer in one of the following ways,
- Get 21 points on the player's first two cards (called a blackjack), without a dealer blackjack;
- Reach a final score higher than the dealer without exceeding 21; or
let the dealer draw additional cards until his or her hand exceeds 21.

##Load and Save the Game
The game offers a choice to load game and save games.

