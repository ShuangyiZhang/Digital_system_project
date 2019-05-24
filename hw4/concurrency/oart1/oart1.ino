// Citation: Paul Han at OH 
#include "concurrency.h"
#define LED_1 3
#define LED_2 4
#define LED_3 5

int T = 0;

void p1 (void)
{
  while (1)
  {
    digitalWrite(LED_1, HIGH);
    delay(1000);
    digitalWrite(LED_1, LOW);
    delay(1000);
  }
  /* process 1 here */
  return;
}

void p2 (void)
{
  while (1)
  {
    digitalWrite(LED_2, HIGH);
    delay(2000);
    digitalWrite(LED_2, LOW);
    delay(2000);
  }
/* process 2 here */
 return;
}

void p3 (void)
{
  while (1)
  {
    digitalWrite(LED_3, HIGH);
    delay(4000);
    digitalWrite(LED_3, LOW);
    delay(4000);
  }
 return;
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_1,OUTPUT);
  pinMode(LED_2,OUTPUT);
  pinMode(LED_3,OUTPUT);
  if (process_create (p1, 64) < 0) {
    return;
  }
  Serial.print("P1 SUCCEEDDED");
  if (process_create (p2, 64) < 0) {
    
    return;
  }
  Serial.print("P2");
  if (process_create (p3, 64) < 0) {
    
    return;
  }
  Serial.print("P3");
}

void loop()
{
  Serial.print("BEFORE START");
  process_start();
  
  /* if you get here, then all processes are either finished or
     there is deadlock */
  while (1) ;
}
