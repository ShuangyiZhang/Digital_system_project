#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))

unsigned int recurse(unsigned char n)
{
   unsigned int out;

     asm volatile (R"(

      .data
      ; any data values/static storage can go here

      .text

      
      ; holds left call value
      clr r23
      
      ; r22 holds value of n
      mov r22,%1

      sbr r21, 1

      call fib


  rjmp exit1

  fib:

      push r23
      push r22
      ; holds total value
      clr r24

  

      ; compare n with 0
      cp r22, r1
      breq ret_0

      ; compare n with 1
      cp r22, r21
      breq ret_1

      rjmp else


  ret_0:
      ; total += 0
      pop r22
      pop r23
      ret

  ret_1:
      ; total += 1

      add r24, r21
      ; rjmp exit1
      adc r24, r1

      pop r22
      pop r23
      ret

  else:

      ; n -= 1, f(n-1)
      sub r22, r21
      call fib

      ; save result of f(n-1)
      mov r23, r24

      clr r24
      ; n -= 1. f(n-2)
      sub r22, r21
      call fib

      ; f(n-2) + f(n-1)
      add r24, r23
      ; add carry bit
      adc r25, r1

      pop r22
      pop r23

      ret

  exit1:
      movw %0, r24      

   )" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
             by the assembly language */
      : "r" (n)  /* n is %1, r24, r25, both are register-sized
              variables */
      : "r25", "r24", "r23", "r22");   /* r24, r25 are modified by this assembly
             language fragment */

     return out;
}
