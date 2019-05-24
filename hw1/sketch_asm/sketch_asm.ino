#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("fib(0) is: ");
  Serial.println(recurse(0));
  Serial.print("fib(1) is: ");
  Serial.println(recurse(1));
  Serial.print("fib(8) is: ");
  Serial.println(recurse(8));
  Serial.print("fib(16) is: ");
  Serial.println(recurse(16));
  Serial.print("fib(24) is: ");
  Serial.println(recurse(24));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(1400);
  digitalWrite (13, LOW);
  delay(1400);
}
