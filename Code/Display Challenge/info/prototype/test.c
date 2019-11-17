#include<stdio.h>
#include <stdlib.h>

struct Challenge
{
    int time;
    int level;
    int sequence_num[10];
};

void print_char(char text);
void display_challenge(int numberSequence[]);

int main()
{   
    struct Challenge ch1;
    char buffer [33];
    ch1.time = 1;
    ch1.level = 0;
    //snprintf(buffer, sizeof(buffer), "%d",ch1.time);
    //printf(buffer);
    //printf("\nA sample C program\n\n");
    char cmd = 'a';
    char hnibble = cmd & 0xFF;
    //printf("%c", hnibble);
    //printf("\nA sample C program\n\n");
    int num = 9;
    num = num + '0';
    print_char(num);
    printf("\n");

    int numbers[] = {9,2,3,4,5};
   display_challenge(numbers);
    printf("\n");

    return 0;
}

void print_char(char text){
    printf("%c", text);
}

void display_challenge(int numberSequence[], int size, int time) {
   // read the array of int and convert it into an array of char 
   int index;
   for(index = 0; index < size; index++){
      int numChar;
      numChar = numberSequence[index] + '0'; // converting int number to a char (apprently the way to convert it and get a char on the LCD)
      putcLCD(numChar);
      if(index < (size-1)){
          putcLCD('-');
      }
   }