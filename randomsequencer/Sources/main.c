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
void display_start();
void display_get_ready();
void display_incorrect();
void display_correct();
void display_level();
void display_goodbye();
void display_you_lose();
void display_you_win();
void key_input();
void display_answer_sequence();

const unsigned char keypad[4][4] =
{
'1','2','3','A',
'4','5','6','B',
'7','8','9','C',
'*','0','#','D'
};

const unsigned char element[14] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D' };
const unsigned char start_message[15] = { 'S','t','a','r','t',' ','G','a','m','e','?',' ','1','/','0' };
const unsigned char get_ready_message[15] = { 'G','e','t',' ','r','e','a','d','y','.','.','.','G','O','!' };
const unsigned char incorrect_message[12] = { 'I','n','c','o','r','r','e','c','t','.','.','.' };
const unsigned char correct_message[8] = { 'C','o','r','r','e','c','t','!' };
const unsigned char level_message[6] = { 'L','e','v','e','l', ' ' };
const unsigned char goodbye_message[9] = { 'G','o','o','d',' ','B','y','e','!' };
const unsigned char you_lose[9] = { 'Y','o','u',' ','L','o','s','e','!' };
const unsigned char you_win[10] = { 'Y','o','u',' ','W','i','n','!','!','!' };
unsigned char sequence[16];	

unsigned char column,row,answer,level_char;
unsigned int a,i,seed, level;
unsigned char answer_sequence[16];
                           

/**************MAIN*******************************/
void main(void){ 
    //DDRB = 0xFF;                           //MAKE PORTB OUTPUT
    DDRJ |=0x02; 
    //PTJ &=~0x02;                            //ACTIVATE LED ARRAY ON PORT B
    DDRP |=0x0F;                           //
    PTP |=0x0F;                            //TURN OFF 7SEG LED
    DDRA = 0x0F;                         //OPEN MAIN    
    asm_mydelay1ms(2000);
    openLCD();
    asm_mydelay1ms(2000);
    while (1){
      asm_mydelay1ms(2000);
      display_start();
      if(answer == '1'){
        level = 2; 
        DDRA = 0x0F;
        PORTA = PORTA | 0x0F; 
      	row = PORTA & 0xF0;
      	while(row == 0x00) {
      		row = PORTA & 0xF0;
      		seed++;
      	}
      	asm_mydelay1ms(2000);
        openLCD();
      	srand(seed);
      	for(i = 0; i < 16; i++) {
      		a = rand() % 14;
      		sequence[i] = element[a];
      	}
      	while(level > 0 && level <= 16){
      	  asm_mydelay1ms(2000);
      	  display_level();
       	  asm_mydelay1ms(2000);	
      		display_challenge(sequence, level, 5);
          cmd2LCD(0x01); // clear screen
          asm_mydelay1ms(2000);	
          display_get_ready();    		
      		for(i = 0; i < level; i++){
      		  key_input();
      		  asm_mydelay1ms(2000);
      		  answer_sequence[i] = answer;
      		}
      		asm_mydelay1ms(2000);
      		for(i = 0; i < level; i++){
      		  if(answer_sequence[i] != sequence[i]){
      		    level = 17;
      		    break;
      		  }
      		}
      		if(level == 17){
      		 display_incorrect(); 
      		} 
      		else{      		 
      		 display_correct();
      		 level = level + 2;
      		}      		
      		asm_mydelay1ms(2000);
      		cmd2LCD(0x01); // clear screen
      	}
      	asm_mydelay1ms(2000);
      	if(level == 17){
      	 display_you_lose();
      	} 
      	else if(level == 18){
      	 display_you_win(); 
      	}
      }
      else{
        display_goodbye();
        break;       
      }
      asm_mydelay1ms(2000);
      cmd2LCD(0x01); // clear screen
    }
}
//CLOSE MAIN
                  

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

void display_start(){
   int index;
   for(index = 0; index < 15; index++){
      putcLCD(start_message[index]);
   }
   key_input();
   cmd2LCD(0x01);         //clear screen
   asm_mydelay1ms(2000);
}

void display_get_ready(){
  int index;
  for(index = 0; index < 9 ; index++){
    putcLCD(get_ready_message[index]); 
  }
  putcLCD(get_ready_message[index]);
  index++;
  delay_LCD(1);
  putcLCD(get_ready_message[index]);
  index++;
  delay_LCD(1);
  putcLCD(get_ready_message[index]);
  index++;
  delay_LCD(1);
  putcLCD(get_ready_message[index]);
  index++;
  asm_mydelay1ms(100);
  putcLCD(get_ready_message[index]);
  index++;
  asm_mydelay1ms(100);
  putcLCD(get_ready_message[index]);
  index++;
  asm_mydelay1ms(100);
  cmd2LCD(0x01); // clear screen
  asm_mydelay1ms(2000);	
}

void display_incorrect(){  
  int index;
  cmd2LCD(0x01); // clear screen
  asm_mydelay1ms(2000);	
  for(index = 0; index < 12 ; index++){
    putcLCD(incorrect_message[index]); 
  }
  delay_LCD(2);  
  cmd2LCD(0x01); // clear screen
}

void display_correct(){
  int index;
  cmd2LCD(0x01); // clear screen
  asm_mydelay1ms(2000);
  for(index = 0; index < 8 ; index++){
    putcLCD(correct_message[index]); 
  }
  delay_LCD(2);  
  cmd2LCD(0x01); // clear screen
}

void display_level(){
  int index;
  if(level == 2){
    level_char = '1';
  } 
  else if(level == 4){
     level_char = '2';
  }
  else if(level == 6){
     level_char = '3';
  }
  else if(level == 8){
     level_char = '4';
  }
  else if(level == 10){
     level_char = '5';
  }
  else if(level == 12){
     level_char = '6';
  }
  else if(level == 14){
     level_char = '7';
  }
  else if(level == 16){
     level_char = '8';
  }
  asm_mydelay1ms(100);
  for(index = 0; index < 6 ; index++){
    putcLCD(level_message[index]); 
  }
  putcLCD(level_char);
  delay_LCD(2);
  cmd2LCD(0x01); // clear screen
}

void display_goodbye(){
   int index;
   for(index = 0; index < 9; index++){
      putcLCD(goodbye_message[index]);
   }
   delay_LCD(5);
   cmd2LCD(0x01); // clear screen
}

void display_you_lose(){
   int index;
   for(index = 0; index < 9; index++){
      putcLCD(you_lose[index]);
   }
   delay_LCD(3);
   cmd2LCD(0x01); // clear screen
}

void display_you_win(){
   int index;
   for(index = 0; index < 10; index++){
      putcLCD(you_win[index]);
   }
   delay_LCD(5);
   cmd2LCD(0x01); // clear screen
}

void display_answer_sequence(){
  int index;
  for(index = 0; index < 16; index++){
    putcLCD(answer_sequence[index]);	 
  }
  delay_LCD(5);
  cmd2LCD(0x01); // clear screen
}

void  display_challenge(char numberSequence[], int size, int time) {
   // read the array of int and convert it into an array of char 
   int index;
   for(index = 0; index < size; index++){
      putcLCD(numberSequence[index]);
   }
   // Delay the output of the LCD for a specific amount of time before clearing hte LCD as per the challenge 
   delay_LCD(time);
   cmd2LCD(0x01); 
 }


void delay_LCD(int seconds){
    // seems to be a limit to the delay, had to cascade them to get a noticeable delay
    int delayMax = seconds * 5; // number of iteration to get seconds of delay (ie. 10 cycles is 2 seconds delay)
    int delayCycles;
    for (delayCycles = 0; delayCycles < delayMax; delayCycles++){
      asm_mydelay1ms(2000);
      //cmd2LCD(0x01); //    cmd2LCD(0x01); // clear screen
    }
}

void key_input(){
	// schar key_input(){lay
  	// seems to be a limit to the delay, had to cascade them to get a noticeable delay
 //a noticeable delay
	// seems to be a limit to the delay, had to cascade them to get a noticeable delay
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  //cmd2LCD(0x01); // clear screen
  
   while(1){                              //OPEN WHILE(1)
      do{                                 //OPEN do1
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row == 0x00);                //WAIT UNTIL KEY PRESSED //CLOSE do1

      do{                                 //OPEN do2
         do{                              //OPEN do3
            asm_mydelay1ms(15);                   //WAIT
            row = PORTA & 0xF0;           //READ ROWS
         }while(row == 0x00);             //CHECK FOR KEY PRESS //CLOSE do3
         
         asm_mydelay1ms(15);                     //WAIT FOR DEBOUNCE
         row = PORTA & 0xF0;
      }while(row == 0x00);                //FALSE KEY PRESS //CLOSE do2

      // THIS WHILE LOOP CHECKS EACH COLUMN TO FIND THE INTERSECTION IN THE KEY MATRIX
      // AND SETS THE COLUMN AND ROW VALUES ACCORDINGLY
      while(1){                           //OPEN while(1)
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x01;                   //COLUMN 0 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 0
            column = 0;
            break;                        //BREAK OUT OF while(1)
         }
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x02;                   //COLUMN 1 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 1
            column = 1;
            break;                        //BREAK OUT OF while(1)
         }

         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x04;                   //COLUMN 2 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 2
            column = 2;
            break;                        //BREAK OUT OF while(1)
         }
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x08;                   //COLUMN 3 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 3
            column = 3;
            break;                        //BREAK OUT OF while(1)
         }
         row = 0;                         //KEY NOT FOUND
      break;                              //step out of while(1) loop to not get stuck
      }                                   //end while(1)

      // ONCE THE ROW AND COLUMN VALUES ARE KNOWN
      // PERFORM THE REQUIRED ACTIn]; 
   //keypad[0][column]keypad[0][columnON (OUTPUT TO LCD OR CLEAR SCREEN
      if(row == 0x10){
         	
         asm_mydelay1ms(15);
         putcLCD(keypad[0][column]);         //OUTPUT TO LCD
         answer = keypad[0][column];
         asm_mydelay1ms(15);
         break;
      }
      else if(row == 0x20){
         
      asm_mydelay1ms(15);
         putcLCD(keypad[1][column]);     //OUTPUT TO LCD
         answer = keypad[1][column];
         asm_mydelay1ms(15);
         break;
 
      }
      else if(row == 0x40){
      
      asm_mydelay1ms(15);
         putcLCD(keypad[2][column]);      //OUTPUT TO LCD
         answer = keypad[2][column];
         asm_mydelay1ms(15);
         break;
 
      }
      else if(row == 0x80){
      
      // * BUTTON WILL CLEAR THE SCREEN HERE
      if(keypad[3][column]=='*'){
          asm_mydelay1ms(15);
          cmd2LCD(0x01); // COMMAND TO CLEAR THE SCREEN
      }else{
          asm_mydelay1ms(15);
         putcLCD(keypad[3][column]);     //OUTPUT TO LCD
         answer = keypad[3][column];
         asm_mydelay1ms(15);
         break;
      }
      
 
      }

      do{
         asm_mydelay1ms(15);
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row != 0x00);                //MAKE SURE BUTTON IS OT
   }
}