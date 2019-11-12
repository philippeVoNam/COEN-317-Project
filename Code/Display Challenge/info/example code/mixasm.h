/* mixasm.h. The name of the ASM file must be mixasm.asm*/
#ifndef _MIXASM_H_
#define _MIXASM_H_

/* Here we list the functions */
void asm_mydelay1ms(unsigned char val); // 1 byte for Input parameter
void asm_mydelay10us(unsigned char val); // 1 byte for Input parameter
// value: Parameter passing scheme: 1 byte: Register B, so 'value' will be in Register B      
/* function that adds the parameter value (on B) to global CData */
/* and then stores the result in ASMData */
/* variable which receives the result of AddVar */

/* External declaration of variables */
/* Here we list the variables that can be accessed by the .c file. These variables are usually
   the output of functions*/
extern char ASMData1ms;// This variable/constant defined in the ASM file will be accessible in the .c file
extern char ASMData10us;// This variable/constant defined in the ASM file will be accessible in the .c file

#endif /* _MIXASM_H_ */