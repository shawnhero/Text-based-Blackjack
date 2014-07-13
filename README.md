text-based-BlackJack
====================

#Key Features
##Game Mode
1. Shuffle Rules. Either a shuffle is triggered every hand or it's not triggered until there're 15 or less cards left.
2. Choose how many decks are used in a game.
3. [Soft 17 Rule](http://www.smartgaming.com/html/articles/soft17.htm). Choose whether the deal stands when he has a soft 17.

All the above are stored in a file named "game.config". If not found, the default values will be used, which are respectively,

1. A shuffle is triggered only when the cards are not enough to use (15 or less).
2. Only 1 deck of cards is used.
3. Dealer hits when the 17 is soft.

##Game Rules
1. Initially, Player has 100 chips, while the dealer has 10000.
2. The player can bet any number of chips, but he cannot bet non-positive or bet beyond the number of chips he possesses.
3. During the game, the player can either `Hit` or `Stand`.
4. `Split`, `Double` and `Surrender` will be added in another branch and will be merged to the master shortly.

##Save and Load Game
1. At the beginning of the game the player will be prompted whether to load last saved game status.
2. Before exiting the game the game status will be stored in a file names "save.dat".

##How to Deal with Ace?
Aces can be treated either as 1 or as 11. Count as many Aces as 11 so long as it's not busted to determine the sum.

#Simulation Mode
A seperate branch will add support to do some simulations and do some exploratory analysis. 
