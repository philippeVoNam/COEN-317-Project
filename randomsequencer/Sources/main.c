#include <hidef.h>      /* common defines and macros */
#include <mc9s12dp256.h>     /* derivative information */
#include <stdio.h>
#include "mixasm.h"
#include <stdlib.h>
#include <time.h>
//#include "lcd.h"
#pragma LINK_INFO DERIVATIVE "mc9s12dp256b"

//scan row and return any key pressed
//returns null if no key pressed
//will only return one character pressed at a time

void cmd2LCD (char cmd);
void openLCD(void);
void putcLCD(char cx);
void putsLCD(char *ptr);
void delay_LCD(int seconds);
void display_challenge(char numberSequence[], int size, int time);


const unsigned char keypad[4][4] =
{
'1','2','3','A',
'4','5','6','B',
'7','8','9','C',
'*','0','#','D'
};

const unsigned char element[14] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D'};
const unsigned char start_message[15] = { 'S','t','a','r','t',' ','G','a','m','e','?',' ','1','/','0'};
unsigned char sequence[10];	

unsigned char column,row;
unsigned int a,i,seed,answer;


/**************MAIN*******************************/
void main(void){                          //OPEN MAIN
    asm_mydelay1ms(2000);
    openLCD();
    asm_mydelay1ms(2000);
    answer = display_start();
    while(1){
      DDRA = 0x0F;
      PORTA = PORTA | 0x0F; 
  		row = PORTA & 0xF0;
  		while(row == 0x00) {
  			row = PORTA & 0xF0;
  			seed++;
  		}
      asm_mydelay1ms(2000);
  	  srand(seed);
  		for(i = 0; i < 10; i++) {
  			a = rand() % 14;
  			sequence[i] = element[a];	
  		}
  		display_challenge(sequence, 10, 5);	
    }
}                                         //CLOSE MAIN

void cmd2LCD (char cmd) {  // cmd: two nibbles. First the higher is sent over DB7-DB4 pins. This is actually DB7-DB4
                          //   Then, the lower nibble is sent over DB7-DB4 pins. This is actually DB3-DB0
    char hnibble, lnibble;
    // note that here, RS is always 0
    
    PORTK = 0x00; // EN, RS=0    

    // sending higher nibble        
    hnibble = cmd &0xF0;
    PORTK = 0x02; // EN = 1    
    hnibble >>=2; // shift two bits to the right to align with PORTK positions
    PORTK = hnibble|0x02;
    asm("nop"); // 1 cycle = 1/(24*10^6) = 41.66 ns
    asm("nop");
    asm("nop");
    PORTK = 0x00; // EN,RS=0    
    
    // Sending lower nibble:
    lnibble = cmd &0x0F;
    PORTK = 0x02; // EN = 1
    lnibble <<= 2; // shift two bits to the left to align with PORTK positions
    PORTK = lnibble|0x02;
    asm("nop");
    asm("nop");
    asm("nop");    
    PORTK = 0x00; // EN, RS = 0
    
    // Wait 50 us for order to complete (enough time for most IR instructions)
    asm_mydelay10us(5);
}

void openLCD(void) {
    DDRK = 0xFF; // PortK configured as outputs
    asm_mydelay1ms(100); // wait for 100 ms
    cmd2LCD(0x28); // set 4-bit data, 2-line display, 5x8 font
    cmd2LCD(0x0F); // turn on display, cursor, blinking
    cmd2LCD(0x06); // move cursor right
    cmd2LCD(0x80);  // set DDRAM address. DDRAM data are ent after this setting. address=0
    cmd2LCD(0x01); // clear screen, move cursor to home
    asm_mydelay1ms(2); // wait for 2 ms. The 'clear display' instruction requires this
    
}

void putcLCD(char cx) {
    char hnibble, lnibble;
    //temp = cx;
    // note that here, RS is always 1
    
    PORTK = 0x01; // RS=1, EN=0
    
    // sending higher nibble
    hnibble = cx&0xF0;
    PORTK = 0x03; // RS=1, EN=1
    hnibble >>= 2; // shift two bits to the right to align with PORTK positions
    PORTK = hnibble|0x03;
    asm("nop");
    asm("nop");
    asm("nop");    
    PORTK = 0x01; // RS=1, E=0
    
    // sending lower nibble
    lnibble = cx & 0x0F;
    PORTK = 0x03; // RS=1, E=1
    lnibble <<= 2;  // shift two bits to the left to align with PORTK positions        
    PORTK = lnibble|0x03;
    asm("nop");
    asm("nop");
    asm("nop");    
    PORTK = 0x01;// RS=1, E=0
    
    // Wait 50 us for order to complete (enough time for most IR instructions)    
    asm_mydelay10us(5); // wait for 50  us
}

void putsLCD (char *ptr) {
    while (*ptr) {
        putcLCD(*ptr);
        ptr++;
    }
}

int display_start(){
   int index;
   for(index = 0; index < 15; index++){
      putcLCD(start_message[index]);
   }
   do{
    PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
    row = PORTA & 0xF0;              //READ ROWS
   }while(row == 0x00);              //WAIT FOR INPUT!!!
   cmd2LCD(0x01); // clear screen
   asm_mydelay1ms(2000);
}
// functions
void display_challenge(char numberSequence[], int size, int time) {
   // read the array of int and convert it into an array of char 
   int index;
   for(index = 0; index < size; index++){
      putcLCD(numberSequence[index]);
   }
   
   // Delay the output of the LCD for a specific amount of time before clearing hte LCD as per the challenge 
   delay_LCD(time);  
}

void delay_LCD(int seconds){
    // seems to be a limit to the delay, had to cascade them to get a noticeable delay
    int delayMax = seconds * 5; // number of iteration to get seconds of delay (ie. 10 cycles is 2 seconds delay)
    int delayCycles;
    for (delayCycles = 0; delayCycles < delayMax; delayCycles++){
        asm_mydelay1ms(2000);
    }
    cmd2LCD(0x01); // clear screen
}