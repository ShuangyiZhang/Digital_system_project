int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
byte leds1 = 0;
byte leds2 = 0;
int currentLED1;
int currentLED2;
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

//Write a port-D pin
void myDigitalWriteD(int pinNumber, int value){
  //Pin 13 = PD 7, Pin 12 = PD 6, Pin 11 = PD 5
  PORTB = PORTB | (value<<(pinNumber-8));
}

//Set input or output of a port-D pin.
void myPinModeD(int pinNumber, int mode){
  //Pin 13 = PD 7, Pin 12 = PD 6, Pin 11 = PD 5
  DDRB = DDRB | (mode<<(pinNumber-8));
}



void myShiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // Shifts 8 bits out MSB first

  int pinState;
  //set up output pin
  myPinModeD(myClockPin, 1);
  myPinModeD(myDataPin, 1);

  //clear everything out
  myDigitalWriteD(myDataPin, 0);
  myDigitalWriteD(myClockPin, 0);
    Serial.print("my data out:\n");
        Serial.print(myDataOut);
    Serial.print("\n");

  //MSB first, so count down from 7 to 0
  for (int i=7; i>=0; i--)  {
    myDigitalWriteD(myClockPin, 0);
    //check each bit
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
         Serial.print("\n Setting 1 is:");

    Serial.print(pinState);
    } else {  
      Serial.print("\n Setting 0 is:");

    Serial.print(pinState);
      pinState= 0;
    }
    Serial.print("\n Data pin is:");

    Serial.print(myDataPin);
    Serial.print("\n Data pin state is:");

    Serial.print(pinState);
//delay(1000);
    //Send bit
    myDigitalWriteD(myDataPin, pinState);
    myDigitalWriteD(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    myDigitalWriteD(myDataPin, 0);
  }

  //stop shifting
  myDigitalWriteD(myClockPin, 0);
}

void setup()
{
    Serial.begin(9600);
    myPinModeD(latchPin, OUTPUT);
    myPinModeD(dataPin, OUTPUT);
    myPinModeD(clockPin, OUTPUT);
    Serial.print("HI");
    leds1 = 0;
    leds2 = 0;
    currentLED1 = 2;
    currentLED2 = 1;

}

void loop()
{
    /*
    if (currentLED == 9){
      currentLED = 7;
    }
    */

    bitSet(leds1, currentLED1);
    //bitSet(leds1, 3);

    bitSet(leds2, currentLED2);

    //Serial.print( leds);
  //  if (currentLED==8){
     //shiftOut(dataPin, clockPin, MSBFIRST, leds2);
     //delay(1000);

     //shiftOut(dataPin, clockPin, MSBFIRST, leds1);
     
    for(int k=0; k<5; k++) { // iterate each col
      leds1 = 0;
     leds2 = 0;
    /*
      for(int k=0; k<5; k++) {
          if (colpin[k]<7){
    bitSet(leds1, colpin[k]);

          }else{
          bitSet(leds2, 13-colpin[k]);
          }
      }*/
      /*if (colpin[k]<7){
    bitSet(leds1, colpin[k]);

          }else{
          bitSet(leds2, 13-colpin[k]);
          }*/
      for (int j =0;j<7;j++){
        if (font[0][k] & (1<<j)){
          //col k, row j should light up
          if (rowpin[j]<7){
            bitSet(leds1, rowpin[j]);

          }else{
            bitSet(leds2, 13-rowpin[j]);
          }
      //digita
        }
      }
      //lWrite(rowpins[row], HIGH); // prepare to write the row"
      Serial.print("\nr2:");
      Serial.print(leds2);
      Serial.print("\nr1:");
      Serial.print(leds1);
      digitalWrite(latchPin, LOW);

      shiftOut(dataPin, clockPin, MSBFIRST, leds2);
      shiftOut(dataPin, clockPin, MSBFIRST, leds1);
      digitalWrite(latchPin, HIGH);
      
/*
      for(int col=0; col<5; col++) {
         digitalWrite(colPins[7-col], character[row] & 1 << col ? LOW : HIGH);
       }
      delay(1);
      digitalWrite(rowPins[row],LOW);*/
      
      } 
      //delay(2000);
/*   
    }else{
          shiftOut(dataPin, clockPin, MSBFIRST, leds);

    }*/

    //delay(1000);
    //currentLED++;
    
}
