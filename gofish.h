#ifndef GOFISH_H
#define GOFISH_H
#include "deck.h"
#include "player.h"
/*
   Define any prototype functions
   for gofish.h here.
*/

/*main function to run through the game, exits the game if a player wins.
  then asks the player if he wants to play again, if the player says "Y",
  we run the game again else if player says "N", we exit the main function.
*/
int main(int args, char* argv[]);

/*simple function to check the target's current hand, we usually only check
user's hand, as computer's hand is hidden.
*/
void display_hand(struct player* target);

/*simple function to check the target's books, we display both user's books
and the computer's book.
*/
void display_book(struct player* target);

//int game_end();
const char* pR(char r);
//void print_hand(struct player* target);
//void print_book(struct player* target);
#endif
