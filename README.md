BlackJack Game Analysis
====================

> For a interactive game version, see the [master](https://github.com/shawnhero/Text-based-Blackjack/tree/master) branch.



The purpose of this branch is to discover and analyze something interesting of the game BlackJack.

In order to do this,

1. Find a way to simulate the user's input. Or rewrite the whole logic of the game loop (might be better).
2. Log all the game information in a single `.csv` file.
3. Use R or Python to analyze the result of the file.

Factors to change can be classified into to aspectes,

1. The game settings, which can already be modified by the `bjconfig.dat` file.
2. Player's strategy. This need to be hard coded in the `Action SuperGambler::WhatToDo()` method. The input parameters should be 
	1.	The dealer's cards that the player can see. (This part can be easy, especially for the first round. There is a [table](http://www.blackjackinfo.com/bjbse.php) which can be referred to.)
	2. The used cards.(This part may be very hard. To get the wisest decision lots of advanced mathematical derivations might be applied.)


The final objective is to find the maximum winning rate a most skilled player can reach. Also we may find lots of other interesting stuff which may help us to win money in Las Vegas.

I'll continue on this project hopefully in August, 2014.