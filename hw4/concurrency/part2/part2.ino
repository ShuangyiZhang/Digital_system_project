// Citation: Paul Han at OH 
#include "concurrency.h"
#define LED_1 3
#define LED_2 4
#define LED_3 5

lock_t *l;

void p1 (void)
{
  while (1)
  {
    lock_acquire(l);
    Serial.print("1");
    digitalWrite(LED_1, HIGH);
    delay(500);
    digitalWrite(LED_1, LOW);
    delay(300);
    lock_release(l);
  }
  /* process 1 here */
  return;
}

void p2 (void)
{
  
  while (1)
  {
    lock_acquire(l);
    Serial.print("2");
    digitalWrite(LED_2, HIGH);
    delay(500);
    digitalWrite(LED_2, LOW);
    delay(300);
    lock_release(l);
    
  }
/* process 2 here */
 return;
}

void p3 (void) // when p1 releases lock, p3 was not added to the blocked_procs
{
  
  while (1)
  {
   lock_acquire(l);
    Serial.print("3");
    digitalWrite(LED_3, HIGH);
    delay(500);
    digitalWrite(LED_3, LOW);
    delay(300);
   lock_release(l);
  }
 return;
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_1,OUTPUT);
  pinMode(LED_2,OUTPUT);
  pinMode(LED_3,OUTPUT);

  l = malloc(sizeof(lock_t));
  lock_init(l);
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
  //process_start();
  
  /* if you get here, then all processes are either finished or
     there is deadlock */
  while (1) ;
}
