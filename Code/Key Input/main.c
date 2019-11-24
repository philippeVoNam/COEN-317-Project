char key_input();

char key_input(){

    // init
    char keyPressed;

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

         // setting the key 
         keyPressed = keypad[0][column]; 

         asm_mydelay1ms(15);

        break;
 
      }
      else if(row == 0x20){
         
      asm_mydelay1ms(15);
         putcLCD(keypad[1][column]);     //OUTPUT TO LCD

        // setting the key 
        keyPressed = keypad[1][column]; 

         asm_mydelay1ms(15);
 
        break;
      }
      else if(row == 0x40){
      
      asm_mydelay1ms(15);
         putcLCD(keypad[2][column]);      //OUTPUT TO LCD

        // setting the key 
        keyPressed = keypad[2][column]; 

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

        // setting the key 
        keyPressed = keypad[3][column]; 

         asm_mydelay1ms(15);
         break;
      }

      }

      do{
         asm_mydelay1ms(15);
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row != 0x00);                //MAKE SURE BUTTON IS NOT STILL HELD
   }                                      //CLOSE WHILE(1)

   return keyPressed;
}