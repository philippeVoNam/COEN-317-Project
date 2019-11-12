#include<stdio.h>
#include <stdlib.h>

struct Challenge
{
    int time;
    int level;
    int sequence_num[10];
};

int main()
{   
    struct Challenge ch1;
    char buffer [33];
    ch1.time = 1;
    ch1.level = 0;
    snprintf(buffer, sizeof(buffer), "%d",ch1.time);
    printf(buffer);
    printf("\nA sample C program\n\n");
    char cmd = 'a';
    char hnibble = cmd & 0xFF;
    printf("%c", hnibble);
    printf("\nA sample C program\n\n");
    return 0;
}