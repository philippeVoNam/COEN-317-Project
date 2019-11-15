#include <hidef.h>      /* common defines and macros */
#include <mc9s12dp256.h>     /* derivative information */
#include <stdio.h>
#include "mixasm.h"
//#include "lcd.h"
#pragma LINK_INFO DERIVATIVE "mc9s12dp256b"

//scan row and return any key pressed
//returns null if no key pressed
//will only return one character pressed at a time

void cmd2LCD (char cmd);
void openLCD(void);
void putcLCD(char cx);
void putsLCD(char *ptr);


const unsigned char keypad[4][4] =
{
'1','2','3','A',
'4','5','6','B',
'7','8','9','C',
'*','0','#','D'
};
unsigned char column,row;

/**************MAIN*******************************/
void main(void){                          //OPEN MAIN
    char *msg1 = "Press button";
   //DDRB = 0xFF;                           //MAKE PORTB OUTPUT
   DDRJ |=0x02; 
   //PTJ &=~0x02;                            //ACTIVATE LED ARRAY ON PORT B
   DDRP |=0x0F;                           //
   PTP |=0x0F;                            //TURN OFF 7SEG LED
   DDRA = 0x0F;                           //MAKE ROWS INPUT AND COLUMNS OUTPUT
   
	openLCD();
	putsLCD(msg1);
	msg1 = "on key pad";
	cmd2LCD(0xC0); // move cursor to 2nd row, 1st column
	putsLCD(msg1);
	// seems to be a limit to the delay, had to cascade them to get a noticeable delay
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  asm_mydelay1ms(2000);
  cmd2LCD(0x01); // clear screen
  
   while(1){                              //OPEN WHILE(1)
      do{                                 //OPEN do1
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row == 0x00);                //WAIT UNTIL KEY PRESSED //CLOSE do1



      do{                                 //OPEN do2
         do{                              //OPEN do3
            asm_mydelay1ms(1);                   //WAIT
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
      // PERFORM THE REQUIRED ACTION (OUTPUT TO LCD OR CLEAR SCREEN
      if(row == 0x10){
         	
         	asm_mydelay1ms(15);
         putcLCD(keypad[0][column]);         //OUTPUT TO LCD
         asm_mydelay1ms(15);
 
      }
      else if(row == 0x20){
         
      asm_mydelay1ms(15);
         putcLCD(keypad[1][column]);     //OUTPUT TO LCD
         asm_mydelay1ms(15);
 
      }
      else if(row == 0x40){
      
      asm_mydelay1ms(15);
         putcLCD(keypad[2][column]);      //OUTPUT TO LCD
         asm_mydelay1ms(15);
 
      }
      else if(row == 0x80){
      
      // * BUTTON WILL CLEAR THE SCREEN HERE
      if(keypad[3][column]=='*'){
          asm_mydelay1ms(15);
          cmd2LCD(0x01); // COMMAND TO CLEAR THE SCREEN
      }else{
          asm_mydelay1ms(15);
         putcLCD(keypad[3][column]);     //OUTPUT TO LCD
         asm_mydelay1ms(15);
      }
      
 
      }

      do{
         asm_mydelay1ms(15);
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row != 0x00);                //MAKE SURE BUTTON IS NOT STILL HELD
   }                                      //CLOSE WHILE(1)
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