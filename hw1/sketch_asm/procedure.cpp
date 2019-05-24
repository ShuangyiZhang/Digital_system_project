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

      mov r24,%1
      mov r25,%2

      push r25
      push r24

      call sumval

      movw r22,r24
      ; store result in r22:r23
      
      pop r24
      pop r25
      
      
      push r25
      push r24
   
      
      call diffval
      
      clc
      add r22, r25
      adc r23, r1
      
  
      pop r24
      pop r25
      
   rjmp exit0

   sumval:

      add r24, r25
      clr r25
      adc r25, r1

      ret

   diffval:

      cp r24,r25
      brlo b_minus_a
      sub r24, r25
      mov r25, r24
      ret 

   b_minus_a:
      sub r25, r24
      ret 

   exit0:
      movw %0, r22      

   )" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
             by the assembly language */
      : "r" (a), "r" (b)  /* a is %1, r24, b is %2, r25, both are register-sized
              variables */
      : "r25", "r24", "r23", "r22");   /* r24, r25 are modified by this assembly
             language fragment */

     return out;
}
