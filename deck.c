#include <stdio.h>
#include "deck.h"

#define NumofRanks 13
#define NumofSuits 4
#define TotalCards 52
#define MaxCards 7

int shuffle() {
   int i;
   int k;
   int r;
   int random;

   char ranks[NumofRanks]={'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
   char suits[NumofSuits]= {'C', 'D', 'H', 'S'};
   for(i=0; i<NumofSuits; i++) { //adds a new card to deck_instance
      for(k=0; k<NumofRanks; k++) {
         struct card new_card = {.suit=suits[i], .rank=ranks[k]};
         deck_instance.list[deck_instance.top_card++]= new_card;
      }
   }
   //printf("%s", &deck_instance.list[1]);
   //printf("%c", r);
   r = 1;
   for(i=0; i<TotalCards; i++) {
      random = rand() % r;
      if(r<51) {
         struct card swapcard= deck_instance.list[i];
         deck_instance.list[i]=deck_instance.list[random];
         deck_instance.list[random] = swapcard;
         r++;
      }
   }

   for(k=0; k<TotalCards; k++) {
      if(&deck_instance.list[k] == NULL) {
         printf("Error in shuffling.");
         return 1;
      }
   }
   deck_instance.top_card = TotalCards;
   return 0;
}

int deal_player_cards(struct player* target) {
   int i=0;
   struct card* newCard;
   while(i<MaxCards) {
      if(deck_size()<=0) {
         printf("Deck is empty.");
         return 1;
      }
      newCard = next_card();
      if(newCard == NULL) {
         printf("No new card.");
         return 1;
      }
     //printf("About to add new card.");
      i++;
      add_card(target, newCard);
   }
   if(i!=MaxCards) {
     printf("Not the right amount of cards.");
     return 1;
   }
   return 0;
}

struct card* next_card() {
   if(deck_instance.top_card <= 0) {
      return NULL;
   }
   return &deck_instance.list[--deck_instance.top_card];
}

size_t deck_size() {
   if(deck_instance.list[0].suit == 0 || deck_instance.list[0].rank == 0 || deck_instance.top_card < 0) {
      return 0;
   }
   return deck_instance.top_card;
}