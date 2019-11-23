#include <hidef.h>      /* common defines and macros */
#include <mc9s12dp256.h>     /* derivative information */
#include <stdio.h>
#include "mixasm.h"
//#include "lcd.h"
#pragma LINK_INFO DERIVATIVE "mc9s12dp256b"

/**************MAIN*******************************/
void main(void){                          //OPEN MAIN
  
  showDIPSW('2');
}                                         //CLOSE MAIN

