//almost same as part 2 except the ISR, void change_name()
int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
byte leds1 = 0;
byte leds2 = 0;
int currentLED1;
int currentLED2;
int Number = 0;

int last_state = 0b00000100; //by default, the button is open(not pressed)
unsigned long debounce = 200; //200 ms as debounce delay
unsigned long last_time;
unsigned char font[10][5] =  {{0x3E, 0x51, 0x49, 0x45, 0x3E},// 0
  {0x00, 0x42, 0x7F, 0x40, 0x00},// 1
  {0x42, 0x61, 0x51, 0x49, 0x46},// 2
  {0x21, 0x41, 0x45, 0x4B, 0x31},// 3
  {0x18, 0x14, 0x12, 0x7F, 0x10},// 4
  {0x27, 0x45, 0x45, 0x45, 0x39},// 5
  {0x3C, 0x4A, 0x49, 0x49, 0x30},// 6
  {0x01, 0x71, 0x09, 0x05, 0x03},// 7
  {0x36, 0x49, 0x49, 0x49, 0x36},// 8
  {0x06, 0x49, 0x49, 0x29, 0x1E}};// 9
int rowpin[7] = {12, 11, 2, 9, 4, 5, 6};
int colpin[5] = {1, 3, 10, 7 ,8};

//Write a port-B pin
void myDigitalWriteB(int pinNumber, int value){
  //Pin 13 = PB 5, Pin 12 = PB 4, Pin 11 = PB 3
  if (value)
    PORTB |= (value<<(pinNumber-8));
  else
    PORTB &= (value<<(pinNumber-8));
}

//Set input or output of a port-B pin.
void myPinModeB(int pinNumber, int mode){
  //Pin 13 = PB 5, Pin 12 = PB 4, Pin 11 = PB 3
  if (mode)
    DDRB |= (mode<<(pinNumber-8));
  else
    DDRB &= (mode<<(pinNumber-8));
}



void myShiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // Shifts 8 bits out MSB first

  int pinState;
  int i;

  //MSB first, so count down from 7 to 0
  for (i=7; i>=0; i--)  {
    myDigitalWriteB(myClockPin, 0);
    //check each bit
    if ( (myDataOut & (1<<i)) > 0) {
      pinState= 1;
    } else {  
      pinState= 0;
    }
    myDigitalWriteB(myDataPin, pinState);
    myDigitalWriteB(myClockPin, 1);
    myDigitalWriteB(myDataPin, 0);
  }

  //stop shifting
  myDigitalWriteB(myClockPin, 0);
}

void setup()
{
    Serial.begin(9600);
    myPinModeB(latchPin, OUTPUT);
    myPinModeB(dataPin, OUTPUT);
    myPinModeB(clockPin, OUTPUT);
    
    attachInterrupt(0, change_number, FALLING);

}

//interrupt service routine on falling edge
void change_number()
{
  //debouncing
   unsigned long interrupt_time = millis();
 
    if ((interrupt_time - last_time) > debounce) {
      Number++;
      if (Number >= 10)
        Number = 0;
      
    }
  //remember last reading
  last_time = interrupt_time;
}

void clear_cols(int k)
{
    // set all other columns to low
  for (int c = 0; c < 5; c++)
  {
    if (c != k)
    {
      if (colpin[c]<7)
        bitSet(leds1, colpin[c]);
      else
        bitSet(leds2, 13 - colpin[c]);
    }
  }
}

void loop()
{
    leds1 = 0;
    leds2 = 0;

    // iterate each col
    for(int k=0; k<5; k++) 
    { 
        leds1 = 0;
        leds2 = 0;

        clear_cols(k);
        for (int j =0;j<7;j++){
          if (font[Number][k] & (1<<j)){
            //col k, row j should light up
            if (rowpin[j]<7){
              bitSet(leds1, rowpin[j]);
  
            }else{
              bitSet(leds2, 13-rowpin[j]);
            }
          }
        }
        
        myDigitalWriteB(latchPin, LOW);   
        myShiftOut(dataPin, clockPin, leds2);
        myShiftOut(dataPin, clockPin, leds1);
        myDigitalWriteB(latchPin, HIGH);
        
      } 
 

}
