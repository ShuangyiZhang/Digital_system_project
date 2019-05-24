/* Citation: https://learn.adafruit.com/photocells?view=all#using-a-photocell */


//to implement anologRead():
//0. convert pin number
//1. set the analog reference (high two bits of ADMUX) and select the channel (low 4 bits)
//2. start the conversion by setting the ADSC bit of ADCSRA to 1
//3. wait for the conversion to finish and return the result stored in ADCL and ADCH.

int photocellPin = A0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int LEDpin = 5;          // connect Red LED to pin 5 (PWM pin)
int LEDbrightness;        // 
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   
}
 
void loop(void) {
  photocellReading = analogRead(photocellPin);  
 
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);
  // LED gets brighter the darker it is at the sensor
  // that means we have to -invert- the reading from 0-1023 back to 1023-0
  photocellReading = 1023 - photocellReading;
  //now we have to map 0-1023 to 0-255 since thats the range analogWrite uses
  LEDbrightness = map(photocellReading, 0, 1023, 0, 255);
       // the raw analog reading
 Serial.print(" Convert to = ");
  Serial.println(LEDbrightness);
  analogWrite(LEDpin, LEDbrightness);
 
  delay(1000);
}
