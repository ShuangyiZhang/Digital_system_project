#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))

unsigned int procedure(unsigned char a, unsigned char b)
{
   unsigned int out;

     asm volatile (R"(

      .data
      ; any data values/static storage can go here

      .text
      
      ; Initally a and b are stored in r24 and r22.
      mov r24,%1
      mov r22,%2

      ; sumval:
      ; 1. Arguments are stored in r24 and r22
      ; 2. Return value is stored in r24 and r25
      ; 3. Registers modified: r24 and r25
      ; 4. No stack manipulation within function
      ; Everytime before calling sumval, push r24 and r25.
      
      push r24
      push r25
      call sumval
      movw r20,r24
      ; store result in r20:r21   
      pop r25
      pop r24
      
      ; diffval: 
      ; 1. Arguments are stored in r24 and r22
      ; 2. Return value is stored in r24, because the result can fit into a 8-bit register
      ; 3. Registers modified: r24 and r22
      ; 4. No stack manipulation within function
      ; Everytime before calling sumval, push r24 and r22.
      push r22
      push r24   
      call diffval
    
      clc
      add r20, r24
      adc r21, r1
      ; Store result in r21:r20
  
      pop r24
      pop r22
      
   rjmp exit0

   sumval:

      add r24, r22
      clr r25
      adc r25, r1
      ret

   diffval:

      cp r24,r22
      brlo b_minus_a
      ; use brlo for unsigned comparison
      sub r24, r22    
      ret 

   b_minus_a:
      sub r22, r24
      mov r24, r22
      ret 

   exit0:
      ; The main program storer output value in r21 and r20.
      movw %0, r20      

   )" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
             by the assembly language */
      : "r" (a), "r" (b)  /* a is %1, r24, b is %2, r25, both are register-sized
              variables */
      : "r25", "r24", "r23", "r22", "r21", "r20");   
     return out;
}
