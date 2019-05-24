#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("Add(0, 0) is: ");
  Serial.println(testasm(0, 0));
  Serial.print("Add(1, 2) is: ");
  Serial.println(testasm(1, 2));
  Serial.print("Add(255, 255) is: ");
  Serial.println(testasm(255, 255));
  Serial.print("Add(128, 127) is: ");
  Serial.println(testasm(128, 127));
  Serial.print("Add(24, 60) is: ");
  Serial.println(testasm(24, 60));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(1400);
  digitalWrite (13, LOW);
  delay(1400);
}
