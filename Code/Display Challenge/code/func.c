// function declarations 
void delay_LCD(int seconds);
void display_challenge(char numberSequence[], int size, int time);

// functions
void display_challenge(char numberSequence[], int size, int time) {
   // read the array of int and convert it into an array of char 
   int index;
   for(index = 0; index < size; index++){
      char numChar = numberSequence[index];
      putcLCD(numChar);
      if(index < (size-1)){
          putcLCD('-'); // putting a - between numbers except for the last one
      }
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

// FIXME everytime we need to display_challenge -> we need a delay 