# Notes on the Display Challenge Function
2019-November-12 16:41:20

# Summary
- assumptions :
  - challenge should be a structure which has :
    - level number
    - sequence of numbers to be displayed
    - time allocated for user input
    - 
- display_challenge ( challenge )
  - reads the sequence and displays the appropriate characters on the LCD screen 
  - after a brief delay, clears the LCD
    - maybe that should tie in with the difficulty of the challenge

# Structure
- inputs 
  - challenge struct
- outputs
  - nothing 

# Brainstorm
- have a structure of challenge :
  - sequence_numbers = array
  - level            = int
  - time             = int 

# LCD Notes
- 2 Lines
- 16 Characters per lines
- Total Characters = 32 characters

- interface port = PORT K 

# Function snippet
```c
void display_challenge(int numberSequence[], int size, int time) {
   // read the array of int and convert it into an array of char 
   int index;
   for(index = 0; index < size; index++){
      int numChar;
      numChar = numberSequence[index] + '0'; // converting int number to a char (apprently the way to convert it and get a char on the LCD)
      putcLCD(numChar);
      putcLCD('-');
   }
   
   delay_LCD(time);
   
   // # To-Dos
   // - add time parameter to display the number for a limited amount of time    
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
```