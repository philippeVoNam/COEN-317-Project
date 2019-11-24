#include <stdio.h>

struct challenge 
{
        int level;
        int size; // the size of the numbers you would like to display on the LCD (increases with levels)
        int seconds;
        char challenge_nums [16]; // sequence of numbers to be displayed
};

char numSequence [] = "123456789ABCDEF"; // REVIEW This should be generated by the random function 

struct challenge create_challenge(int level, char numSequence[]);
void print_challenge(struct challenge challenge);

int main()
{

   struct challenge challenge = create_challenge(3,numSequence);
   print_challenge(challenge);

   // get challenge -> 

   return 0;
}

struct challenge create_challenge(int level, char numSequence[]){
    /*
    - takes a level
    - based on the level -> calculates the size
    - for now -> we are keeping the time constant
    */

    struct challenge challenge;
    challenge.level = level;
    challenge.size = level * 2;
    challenge.seconds = 3;

    for(int i = 0; i < challenge.size; i++){
        challenge.challenge_nums[i] = numSequence[i];
    }

    return challenge;
}

void print_challenge(struct challenge challenge){
    printf("Challenge :");
    printf("\n");
    printf("level : %d", challenge.level);
    printf("\n");
    printf("size : %d", challenge.size);
    printf("\n");
    printf("seconds : %d", challenge.seconds);
    printf("\n");
    printf(challenge.challenge_nums);
    printf("\n");
}