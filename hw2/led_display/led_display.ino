int val = -5;
int last_state = 0b00010000;
unsigned long last_time;
unsigned long debounce = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRD &= 0b11101111; // just read port 4
  val = PIND & 0b00010000; // store intial value
}

void loop() {
  // put your main code here, to run repeatedly:
  

  int reading = PIND & 0b00010000;
  if(reading!= last_state){
    last_time = millis();
  }
  if ((millis() - last_time) > debounce) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

      if ((PIND & 0b00010000) != val) {
    // reset the debouncing timer
    
  
        val = PIND & 0b00010000;
        
        Serial.print("INPUT CHANGED               \n");
      }
  }
  last_state = reading;
}
