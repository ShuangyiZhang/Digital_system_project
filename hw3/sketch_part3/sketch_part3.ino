// citation reference: http://playground.arduino.cc/Code/Bounce

#include <Adafruit_SSD1306.h>
#include <Bounce2.h>

// Citation:
// bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 128;
unsigned long last_time =0;
unsigned long debounce = 10;

long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

#define OLED_CLK 8
#define OLED_SI 9
#define OLED_RST 10
#define OLED_DC 11
#define OLED_CS 12

int dxMax = 32;
int dyMax = 32;
int dx = 0;
int dy = 0;
int signx = 1;
int signy = 1;
int x = 67;
int y = 37;
int inputPin =  2;
//
//Bounce  bouncer  = Bounce(); 

Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);

void setup() {
  Serial.begin(9600);
   // Setup the button
 //pinMode( inputPin ,INPUT);
 // Activate internal pull-up (optional) 
/// digitalWrite( inputPin ,HIGH);
 // After setting up the button, setup the object
 //bouncer .attach( inputPin );
 //bouncer .interval(5);


pinMode(encoderPin1, INPUT);
pinMode(encoderPin2, INPUT);

digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

//bouncer1 .attach(encoderPin1);
//bouncer2 .attach(encoderPin2);
//bouncer1 .interval(5);
//bouncer2 .interval(5);
//

//call updateEncoder() when any high/low changed seen
//on interrupt 0 (pin 2), or interrupt 1 (pin 3)
attachInterrupt(0, updateEncoder, CHANGE);
attachInterrupt(1, updateEncoder, CHANGE);

  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
  disp.display();
}

void loop() {
  checkBoundary();
  drawBall();

      Serial.print("\n X=");
//
  Serial.print(encoderValue);
//        Serial.print(" y= ");
//
//        Serial.print(signy);
  disp.display();
  disp.clearDisplay();

  //rot_value=bouncer.update ( );
}

int reverse(int x){
  if (x>=128) return 1;
  else return -1;
}
void setVelocity(){
  if (encoderValue>=255){
//    dx = dx/abs(dx)*dxMax;
//    dy = dy/abs(dy)*dyMax;
encoderValue = 255;
  } else if (encoderValue<=0){
//    dx = -dx/abs(dx)*dxMax;
//    dy = -dy/abs(dy)*dyMax;
      encoderValue = 0;
  }
//  if (encoderValue<128){
//    signx = -signx;
//    signy = -signy;
//  }
    dx = abs(dxMax *(encoderValue-128)/128);
    dy = abs(dyMax *(encoderValue-128)/128);
    
//  
//      Serial.print("\n X=");
//
//    Serial.print(signx);
//        Serial.print(" y= ");
//
//        Serial.print(signy);


 
}
void drawBall(){
   disp.fillRoundRect(x, y, 4, 4, 1, WHITE);
   x+=reverse(encoderValue)*dx*signx;
   y+=reverse(encoderValue)*dy*signy;
}

void checkBoundary(){
  if (x<=4){
    x = 4;
    signx = -signx;
  }

  if (x>=128-4){
    x=124;
    signx = -signx;
  }

  if (y<=4){
    y =4;
    signy = -signy;
  }

  if (y>=64-4){
    y = 60;
    signy = -signy;
  }
}

void updateEncoder(){
//int MSB = bouncer1.read();
//int LSB = bouncer2.read();
    unsigned long interrupt_time = millis();

    int MSB = digitalRead(encoderPin1); //MSB = most significant bit
    int LSB = digitalRead(encoderPin2); //LSB = least significant bit

    int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number 
    int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value 
    if ((interrupt_time - last_time) > debounce) {
      
      if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue += 4; 
      if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue -= 4; 
      
      setVelocity();

    }
    lastEncoded = encoded; //store this value for next time 
//  //remember last reading
  last_time = interrupt_time;

}
