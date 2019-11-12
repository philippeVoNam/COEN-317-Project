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