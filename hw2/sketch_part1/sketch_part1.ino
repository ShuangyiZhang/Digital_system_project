int val;
int last_read = 0b00000100; //by default, the button is open(not pressed)
unsigned long last_time;
unsigned long debounce = 10; //10 ms as debounce delay

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  DDRD &= 0b11111011; // just read PD2, which is pin 4
  val = PIND & 0b00000100; // store intial value

}

void loop() {
  // put your main code here, to run repeatedly:
  


  int reading = PIND & 0b00000100;

  //if current state changes, start the timer.
  if(reading!= last_read){
    last_time = millis();
  }
  
  if ((millis() - last_time) > debounce) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so update it as the current state:

      //polling
      if (reading != val) {
  
        val = reading;
        
        Serial.print("INPUT CHANGED\n");
      }

  }
  //remember last reading
  last_read = reading;
}
