#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "mixasm.h"

void cmd2LCD (char cmd);
void openLCD(void);
void putcLCD(char cx);
void putsLCD(char *ptr); 
   
   
void main(void) {
	char *msg1 = "ECE470/570 @ OU";
	char *msg2 = "LCD works!!!!!!!!";
	
	// Messages are placed line by line indepedently
	// There might be other ways to control the LCD so that for example we can shift data on LCD:
	//    we can displace the same message, or we can do something with the cursor shift and data to do so.
	openLCD();
	putsLCD(msg1);
	cmd2LCD(0xC0); // move cursor to 2nd row, 1st column
	putsLCD(msg2);
	while(1);  
}

// In Assembly, we use PTK. In C, we use PORTK
// PORTK: bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
//                  DB7  DB6  DB5  DB4  EN   RS

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

