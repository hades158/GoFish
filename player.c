#include <stdio.h>
#include "player.h"

int add_card(struct player* target, struct card* new_card) {
   int i=0;
   //int k = target->hand_size;
   if(target->hand_size == 0) {
      target->card_list = (struct hand*)malloc(sizeof(struct hand));
      target->card_list->top = *new_card;
      target->card_list->next = NULL;
    }
    else if(target->hand_size > 0){
        int k = target->hand_size-1;
        struct hand* currentCard = target->card_list;
        while(i < k){
            currentCard = currentCard->next;
            i++;
        }
        currentCard->next = (struct hand*)malloc(sizeof(struct hand));
        currentCard->next->top = *new_card;
        currentCard->next->next = NULL;
        if(i != k) {
            return 1;
        }
    }
    target->hand_size++;
    check_add_book(target);
    return 0;
}

int remove_card(struct player* target, struct card* old_card) {
   int k=0;
   struct hand* currentCard = target->card_list;
   struct hand* previous = NULL;
   if(target->hand_size <= 0) {
      return 1;
   }

   while(k<target->hand_size) {
      if((currentCard->top.suit == old_card->suit && currentCard->top.rank == old_card->rank) && currentCard != NULL) {
         break;
      }
      previous = currentCard;
      currentCard = currentCard->next;
      k++;
   }

   if(currentCard == NULL) {
      return 1;
   }

   if(previous == NULL || previous->top.suit == 0 || &(previous->top.suit) == NULL) {
      target->card_list = target->card_list->next;
      free(currentCard);
      target->hand_size--;
      return 0;
   }

   else {
      previous->next = currentCard->next;
      free(currentCard);
      target->hand_size--;
      return 0;
   }
   return 1;
}

char check_add_book(struct player* target) {
   int i = 0;
   int matches = 1;
   if(target->card_list == NULL) {
      return 0;
   }
   struct hand* lastHand = target->card_list;
   while(i++ < target->hand_size && lastHand->next != NULL) {
      lastHand = lastHand->next;
   }

   struct card lastCard = lastHand->top;
   struct hand* currentHand = target->card_list;
   struct hand* firstHand;
   struct hand* secondHand;
   struct hand* thirdHand;
   //while(i< (target->hand_size - 1) && currentHand != NULL) { //while loop does not work.
   for(i = 0; i < target->hand_size - 1 && currentHand != NULL; i++) {
      if(currentHand->top.rank == lastCard.rank) {
         if(matches == 1) { //switch case didn't work.
                firstHand = currentHand;
            } else if(matches == 2) {
                secondHand = currentHand;
            } else if(matches == 3) {
                thirdHand = currentHand;
                matches++;
                break;
            } else{
               return 0;
            }
            matches++;
        }
        currentHand = currentHand->next;
    }
    int k;
    if(matches == 4) {
        char rank = firstHand->top.rank;
        //while(k<7) { //changed from for loop to while loop
        for(k=0;k<7;k++){
            if(target->book[k] == 0 || target->book[k] == '\0') {
                target->book[k] = rank;
                break;
                //k++;
            }
        }
        remove_card(target, &firstHand->top);
        remove_card(target, &secondHand->top);
        remove_card(target, &thirdHand->top);
        remove_card(target, &lastHand->top);
        return rank;
    }
    return 0;
}


int search(struct player* target, char rank) {
    int i=0;
    struct hand* currentHand = target->card_list;
    int k = target->card_list;
    while(currentHand != NULL && i<k) { //our while loop works.
        if(currentHand->top.rank == rank)
            return 1;
        currentHand = currentHand->next;
        i++;
    }

    return 0;
}

int transfer_cards(struct player* src, struct player* dest, char rank) {
   int tranfers=0;
   int k=0;
   struct hand* sourceHand = src->card_list;
   while(sourceHand!= NULL){ //while loop works
      if(sourceHand->top.rank == rank){
         add_card(dest, &sourceHand->top);
         remove_card(src, &sourceHand->top);
         sourceHand = sourceHand->next;
         tranfers++;
      }
      else{
         sourceHand = sourceHand->next;
      }
      k++;
   }
   if(k != src->hand_size){
       return 1;
   }
   return tranfers;
}

int game_over(struct player* target) { //need to change
   int i=0;
   for(i = 0; i < 7; i++) {
   //while(k<7){
      if(target->book[i] ==0 ){
         return 0;
      }
      //k++;
   }
   return 1;
}

int reset_player(struct player* target) { //need to change
    while(target->card_list != NULL) {
        remove_card(target, &target->card_list->top);
    }

    if(target->hand_size != 0 || target->card_list != NULL) {
        return -1;
    }

    int i;
    for(i = 0; i < 7; i++) {
        target->book[i] = 0;
    }

    if(i != 7 || target->hand_size != 0) {
        return -1;
    }
    return 0;
}

char computer_play(struct player* target) { //need to change
    int i; 
    int randIndex = rand() % target->hand_size;
    struct hand* current_hand = target->card_list;
    for(i = 0; i < randIndex; i++) {
        current_hand = current_hand->next;
    }

    if(current_hand->top.rank == 'T')
        fprintf(stdout, "Player 2's turn, enter a Rank: 10\n");
    else
        fprintf(stdout, "Player 2's turn, enter a Rank: %c\n", current_hand->top.rank);

    return current_hand->top.rank;
}

/*char user_play(struct player* target) { //need to change
    char rank;
    do {
        fprintf(stdout, "Player 1's turn, enter a Rank: ");
        char buf[4] = "";
        scanf("%3s", buf); // If {Enter} is pressed before other input, this still blocks. I believe this is fine. 
        while(getchar() != '\n'); // Clear anything after the 3rd character from stdin 

        // Check for a "10" 
        if(buf[0] == '1' && buf[1] == '0' && buf[2] == '\0')
            rank = 'T';
        else if(buf[1] == '\0')
            rank = buf[0];
        else { // Invalid input length 
            fprintf(stdout, "Error - must have at least one card from rank to play\n");
            continue;
        }

        // If the selected rank is in the player's hand, return it 
        if(search(target, rank) && buf[0] != 'T') // Input 'T' improperly results in a successful search 
            break;

        fprintf(stdout, "Error - must have at least one card from rank to play\n");
    }while(1);

    return rank;
}*/

char user_play(struct player* target) {
   char rank;
   do {
        fprintf(stdout, "Player 1's turn, enter a Rank: ");
        char userInput[2] = "";
        scanf("%2s", userInput);
        while(getchar() != '\n'); {
           if(userInput[0] == '1' && userInput[1] == '0' && userInput[2] == '\0')
            rank = 'T';
        if(userInput[1] == '\0') {
            rank = userInput[0];
            //printf("if loop works");
            //printf("%c", rank);
        }
        else {
            fprintf(stdout, "Error - must have at least one card from rank to play\n");
            continue;
        }
        }
        if(search(target, rank)&& userInput[0] != 'T')
            //printf("going in search");
            break;
        //printf(stdout, "Error - must have at least one card from rank to play\n");
   } while(1);

    return rank;
}