#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("procedure(0, 0) is: ");
  Serial.println(procedure(0, 0));
  Serial.print("procedure(255, 0) is: ");
  Serial.println(procedure(255, 0));
  Serial.print("procedure(8, 16) is: ");
  Serial.println(procedure(8, 16));
  Serial.print("procedure(100, 100) is: ");
  Serial.println(procedure(100, 100));
  Serial.print("procedure(24, 255) is: ");
  Serial.println(procedure(24, 255));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(1400);
  digitalWrite (13, LOW);
  delay(1400);
}
