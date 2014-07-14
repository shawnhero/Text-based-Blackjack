text-based-BlackJack
====================

Try to find how the factors below affect the result of the game.
#Key Features
##Game Mode (Dealer's Perspective)
1. Shuffle Rules. Either a shuffle is triggered every hand or it's not triggered until there're 15 or less cards left.
2. Choose how many decks are used in a game.
3. [Soft 17 Rule](http://www.smartgaming.com/html/articles/soft17.htm). Choose whether the deal stands when he has a soft 17.

All the above are stored in a file named "game.config". If not found, the default values will be used, which are respectively,

1. A shuffle is triggered only when the cards are not enough to use (15 or less).
2. Only 1 deck of cards is used.
3. Dealer hits when the 17 is soft.

##Game Strategy (Player's Perspective)
###Same as the Dealer
The player adopt a strategy same as the dealer.

###[HitOrStand](http://www.hitorstand.net/strategy.php) Strategy
1. Double when you have 10 and the dealer show a card less than 10.
2. Stand if you have 17 or above.
3. Always stand when you have 19 or more.
3. Don't risk bursting when the dealer show 6 or below. Stand and hope he bursts.
4. When you have a soft 18, double if the dealer shows a 3-6.
5. Double-down when you have 11, unless the dealer show an Ace.
6. Double-down when you have 10, and the deaeler shows a card less than 10.
7. When you have a soft Ace always hit or double if you have less than 18.
8.  

Use the strategy table to determine what is the right thing to do.

###Investigate The Best Strategy on the First Round
