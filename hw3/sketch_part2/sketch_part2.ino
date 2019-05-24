//citation: https://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/

#define trigPin 11
#define echoPin 12
#define ledg 10
#define ledb 9
#define ledr 8

int photocellPin = A0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
//int LEDpin = 5;          // connect Red LED to pin 5 (PWM pin)
int LEDbrightness; 
void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(ledr, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
    Serial.print(distance);
    Serial.println(" cm");

  double LEDbrightness = (distance-2)/398.0*255; //map 2cm-400cm to 0-255
 
  if (LEDbrightness < 85) {  // This is where the LED On/Off happens
  digitalWrite(ledb,HIGH);
    digitalWrite(ledr,HIGH);
    set_brightness(ledg);
    //digitalWrite(ledr,LOW);

}
  else if (LEDbrightness>=85 && LEDbrightness<170){
    digitalWrite( ledg,HIGH);
        set_brightness(ledb);

       digitalWrite(ledr,HIGH);

  }else{
    //Serial.print("RED");
    digitalWrite(ledg,HIGH);
    digitalWrite(ledb,HIGH);
     set_brightness(ledr);

  }
  
  delay(1000);
}

void set_brightness(int LEDpin){
  photocellReading = analogRead(photocellPin);  

  //now we have to map 0-1023 to 0-255 since thats the range analogWrite uses
  LEDbrightness = map(photocellReading, 0, 1023, 0, 255);
  if (LEDpin==ledr){
    Serial.print(LEDbrightness);
    if (LEDbrightness>=40) LEDbrightness -= 40;
  }
  analogWrite(LEDpin, LEDbrightness);
}
