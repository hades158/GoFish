/*#include <stdio.h>
#include <stdlib.h>
#include "gofish.h"

int main(int args, char* argv[]) {
   int restart = 0;
   //char rank = "";
   do {
      char rank = "";
      reset_player(&user);
      reset_player(&computer);
      fprintRANKintf(stdout,"resetting players and shuffling cards");
      shuffle();
      deal_player_cards(&user);
      deal_player_cards(&computer);
      while(1){
         fprintRANKintf(stdout, "Player 1's Hand - ");
         display_user_hand(&user);
         fprintRANKintf(stdout, "Player 1's Book - ");
         display_book(&user);
         fprintRANKintf(stdout, "Player 2's Book - ");
         display_book(&computer);
         rank = user_play(&user);
         if(search(&computer, rank) == 0){
            transfer_cards(&computer, &user, rank);
         }
         else{
            struct card* newCard = next_card();
            add_card(&user,newCard);
         }
         rank = user_play(&computer);
         if(search(&user, rank) == 0){
            transfer_cards(&user, &computer, rank);
         }
         else{
            struct card* newCard = next_card();
            add_card(&computer,newCard);
         }
         if(game_over(&user) == 1){
            fprintRANKintf(stderr, "Congratulations! You beat the computer player.\n");
            break;
         }
         else if(game_over(&computer) == 1) {
            fprintRANKintf(stderr, "Sorry! You loose the game to the computer player. \n");
            break;
         }
      }
      char playAgain = "";
      fprintRANKintf(stdout, "\nDo you want to play again [Y/N]: ");
      scanf("%c", playAgain);
      if(playAgain == "Y") {
         restart == 1;
      }
      else if(playAgain == "N") {
         restart = 0;
      }
   } while(restart == 1);
}

void display_user_hand(struct player* target) {
   int k = target->hand_size;
   int i;
   struct hand* currentHand = target->card_list;
   if(k == 0) {
      fprintRANKintf(stdout, "\n");
      return;
   }
   while(i<k){
      currentHand = currentHand->next;
      fprintRANKintf(stdout, currentHand->top.rank, currentHand->top.suit);
      i++;
   }
   fprintRANKintf(stdout, "/n");
}

void display_book(struct player* target) {
   int k=0;
   if(target->book == NULL || target->book == 0) {
      fprintRANKintf(stdout, "/n");
   }
   while(k<7 && target->book[k] != 0) {
      fprintRANKintf(stdout, target->book[k++]);
   }
   fprintRANKintf(stdout,"\n");
}
*/

#include <stdio.h>
#include <stdlib.h>
#include "gofish.h"

int main(int args, char* argv[]) {
   srand(time(NULL));
   int playAgain =0;
   struct player* currentPlayer;
   struct player* nextPlayer;
   do {
      reset_player(&user);
      reset_player(&computer);
      fprintf(stdout, "Shuffling deck...\n");
      shuffle();
      deal_player_cards(&user);
      deal_player_cards(&computer);
      currentPlayer = &user;
      nextPlayer = &computer;
      do {
         fprintf(stdout, "Player 1's Hand - ");
         display_user_hand(&user);
         fprintf(stdout, "Player 1's Book - ");
         display_book(&user);
         fprintf(stdout, "Player 2's Hand - "); // remove from code
         display_user_hand(&computer);    // remove
         fprintf(stdout, "Player 2's Book - ");
         display_book(&computer);
         //struct player* diffPlayer = (currentPlayer == &user) ? &computer : &user;
         struct player* diffPlayer;
         if (currentPlayer == &user){
            diffPlayer = &computer;
         }
         else {
            diffPlayer = &user;
         }
         if(game_over(currentPlayer)) {
            break;
          }
         if(game_over(diffPlayer)) { 
            currentPlayer = diffPlayer; 
            break;
          }
            char rank;
         if(currentPlayer->hand_size > 0) { 
         if(currentPlayer == &user) { 
            rank = user_play(currentPlayer);
         } 
         else { 
            rank = computer_play(currentPlayer);
         }
         } 
         else
            rank = 'X'; 

         if(search(diffPlayer, rank) == 0) { 
         if(rank != 'X') 
            if(currentPlayer == &user) {
               fprintf(stdout, "    - Player 2 has no %s's\n", pR(rank));
            }
            else{
               fprintf(stdout, "    - Player 1 has no %s's\n", pR(rank));
            }
           // fprintf(stdout, "    - Player %d has no %s's\n", ((currentPlayer == &user) ? 2 : 1), pR(rank));
         struct card* deckCard = next_card();
         int next_book_i = 0;
         if (deckCard != NULL) {
            if(currentPlayer == &user)
                fprintf(stdout, "    - Go Fish, Player 1 draws %s%c\n", pR(deckCard->rank), deckCard->suit);
            else {
                if (deckCard->rank == rank) {
                    fprintf(stdout, "    - Go Fish, Player 2 draws %s%c\n", pR(deckCard->rank), deckCard->suit);
                }
                else {
                    fprintf(stdout, "    - Go Fish, Player 2 draws a card\n");
                }
            }

           while(currentPlayer->book[next_book_i] != 0) {
                next_book_i++;
            }
            add_card(currentPlayer, deckCard);
            if(currentPlayer->book[next_book_i] != 0)
                fprintf(stdout, "    - Player %d books %s\n", ((currentPlayer == &user) ? 1 : 2), pR(deckCard->rank));
         }
         if(deckCard != NULL && (currentPlayer->book[next_book_i] != 0 || deckCard->rank == rank)) {
            nextPlayer = currentPlayer;
            fprintf(stdout, "    - Player %d gets another turn\n", ((currentPlayer == &user) ? 1 : 2));
         } 
         else { 
            nextPlayer = diffPlayer;
            fprintf(stdout, "    - Player %d's turn\n", ((nextPlayer == &user) ? 1 : 2));
         }
         } 
         else { 
        
            struct player* print_player = currentPlayer;
            int i;
            for(i = 0; i < 2; i++) {
               if (currentPlayer == &computer) {
                  print_player = (currentPlayer == &user) ? &computer : &user; 
                  continue;
               }
               fprintf(stdout, "    - Player %d has ", ((print_player == &user) ? 1 : 2));
               int j;
               struct hand* h = print_player->card_list;
               int rank_count = 0;
               for(j = 0; j < print_player->hand_size && h != NULL; j++) {
                  if(h->top.rank == rank) {
                     if(rank_count++ > 0)
                           fprintf(stdout, ", ");
                     fprintf(stdout, "%s%c", pR(rank), h->top.suit);
                     //fprintf(stdout, "%s%c", rank, h->top.suit);
                  }

                  h = h->next;
               }
               fprintf(stdout, "\n");
               print_player = (currentPlayer == &user) ? &computer : &user; 
         }

            int next_book_i = 0;
            while(currentPlayer->book[next_book_i] != 0) {
               next_book_i++;
            }
            transfer_cards(diffPlayer, currentPlayer, rank);
            if(currentPlayer->book[next_book_i] != 0)
            fprintf(stdout, "    - Player %d books %s\n", ((currentPlayer == &user) ? 1 : 2), pR(rank));
              // fprintf(stdout, "    - Player %d books %s\n", ((currentPlayer == &user) ? 1 : 2), rank);
         
            if(currentPlayer->book[next_book_i] != 0) {
               nextPlayer = currentPlayer;
               fprintf(stdout, "    - Player %d gets another turn\n", ((currentPlayer == &user) ? 1 : 2));
            } else {
               nextPlayer = diffPlayer;
               fprintf(stdout, "    - Player %d's turn\n", ((nextPlayer == &user) ? 1 : 2));
            }
         }   
         currentPlayer = nextPlayer;  
      }while(1);

        struct player* diffPlayer = (currentPlayer == &user) ? &computer : &user;
    
    int count = 0;
    while(diffPlayer->book[count] != 0 && count < 6) { 
        count++;
    }
    if(currentPlayer == &user) {
        fprintf(stdout, "Player 1 Wins! 7-%d\n", count);
    } else {
        fprintf(stdout, "Player 2 Wins! 7-%d\n", count);
    }

    char yn[3] = "";
    int tryAgain = 0;
    do {
        if(tryAgain) {
            fprintf(stdout, "Error - must enter \"Y\" or \"N\"");
        }
        fprintf(stdout, "\nDo you want to play again [Y/N]: ");
        scanf("%2s", yn);
        tryAgain = 1;

        while(getchar() != '\n');

        if(yn[1] != '\0')
            continue;

        if(yn[0] == 'Y'){
            playAgain = 1;
            break;
        }
        else if(yn[0] == 'N'){
            playAgain =0 ;
            break;
        }
        else
            continue;
    }while(1);

   }while(playAgain == 1);
    fprintf(stdout, "Exiting\n");
    return 0;
}

const char* pR(char r) {
    if(r == 'T') {
        static char ten[] = "10";
        return ten;
    }
    static char rS[2];
    rS[0] = r;
    rS[1] = '\0';
    return rS;
}

void display_user_hand(struct player* target) {
   int k = target->hand_size;
   int i=1 ;
   struct hand* currentHand = target->card_list;
   if(k == 0) {
      fprintf(stdout, "\n");
      return;
   }
   fprintf(stdout, "%s%c", pR(currentHand->top.rank), currentHand->top.suit);
   //fprintf(stdout, "%s%c", currentHand->top.rank, currentHand->top.suit);
   while(i<target->hand_size){
      currentHand = currentHand->next;
      fprintf(stdout," %s%c" , pR(currentHand->top.rank), currentHand->top.suit);
      //fprintf(stdout," %s%c" , currentHand->top.rank, currentHand->top.suit);
      i++;
   }
   fprintf(stdout, "\n");
}

void display_book(struct player* target) {
   int k=1;
   if(target->book == NULL || target->book == 0) {
      fprintf(stdout, "\n");
   }
   fprintf(stdout, " %s", pR(target->book[0]));
   //fprintf(stdout, "%s", target->book[0]);
   while(k<7 && target->book[k] != 0) {
      fprintf(stdout, " %s", pR(target->book[k++]));
   }
   fprintf(stdout,"\n");
}
