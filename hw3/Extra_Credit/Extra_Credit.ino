// Citation:
// bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/
// https://github.com/adafruit/Adafruit_SSD1306/blob/master/Adafruit_SSD1306.cpp

/*!
  We adapted relevant portions for SPI implemention from the Adafruit SSD1306 source  
 */

#include "Adafruit_SSD1306.h"
#include "splash.h"
#include <avr/pgmspace.h>

// SOME DEFINES AND STATIC VARIABLES USED INTERNALLY -----------------------
#define ssd1306_swap(a, b) \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

#define WIRE_WRITE wire->write ///< Wire write function in recent Arduino lib
#define SSD1306_SELECT       digitalWrite(csPin, LOW);  ///< Device select
#define SSD1306_DESELECT     digitalWrite(csPin, HIGH); ///< Device deselect
#define SSD1306_MODE_COMMAND digitalWrite(dcPin, LOW);  ///< Command mode
#define SSD1306_MODE_DATA    digitalWrite(dcPin, HIGH); ///< Data mode
#define SETWIRECLOCK wire->setClock(wireClk)    ///< Set before I2C transfer
#define RESWIRECLOCK wire->setClock(restoreClk) ///< Restore after I2C xfer

// Check first if Wire, then hardware SPI, then soft SPI:
#define TRANSACTION_START   \
 if(wire) {                 \
   SETWIRECLOCK;            \
 } else {                   \
   SSD1306_SELECT;          \
 } ///< Wire, SPI or bitbang transfer setup
#define TRANSACTION_END     \
 if(wire) {                 \
   RESWIRECLOCK;            \
 } else {                   \
   SSD1306_DESELECT;        \
 } ///< Wire, SPI or bitbang transfer end

// CONSTRUCTORS, DESTRUCTOR ------------------------------------------------

Adafruit_SSD1306::Adafruit_SSD1306(uint8_t w, uint8_t h,
  int8_t mosi_pin, int8_t sclk_pin, int8_t dc_pin, int8_t rst_pin,
  int8_t cs_pin) : Adafruit_GFX(w, h), spi(NULL), wire(NULL), buffer(NULL),
  mosiPin(mosi_pin), clkPin(sclk_pin), dcPin(dc_pin), csPin(cs_pin),
  rstPin(rst_pin) {
}

Adafruit_SSD1306::~Adafruit_SSD1306(void) {
  if(buffer) {
    free(buffer);
    buffer = NULL;
  }
}

// LOW-LEVEL UTILS ---------------------------------------------------------
inline void Adafruit_SSD1306::SPIwrite(uint8_t d) {
  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    digitalWrite(mosiPin, d & bit);
    digitalWrite(clkPin , HIGH);
    digitalWrite(clkPin , LOW);
  } 
}

void Adafruit_SSD1306::ssd1306_command1(uint8_t c) {
    SSD1306_MODE_COMMAND
    SPIwrite(c);
}

void Adafruit_SSD1306::ssd1306_commandList(const uint8_t *c, uint8_t n) {
    SSD1306_MODE_COMMAND
    while(n--) SPIwrite(pgm_read_byte(c++));
}

void Adafruit_SSD1306::ssd1306_command(uint8_t c) {
  TRANSACTION_START
  ssd1306_command1(c);
  TRANSACTION_END
}

boolean Adafruit_SSD1306::begin(uint8_t vcs, uint8_t addr, boolean reset,
  boolean periphBegin) {

  if((!buffer) && !(buffer = (uint8_t *)malloc(WIDTH * ((HEIGHT + 7) / 8))))
    return false;

  clearDisplay();
  drawBitmap((WIDTH - splash1_width) / 2, (HEIGHT - splash1_height) / 2,
  splash1_data, splash1_width, splash1_height, 1);

    vccstate = vcs;
    pinMode(dcPin, OUTPUT); // Set data/command pin as output
    pinMode(csPin, OUTPUT); // Same for chip select
    SSD1306_DESELECT
    // Soft SPI
    pinMode(mosiPin, OUTPUT); // MOSI and SCLK outputs
    pinMode(clkPin , OUTPUT);
    digitalWrite(clkPin, LOW); // Clock low

  // Reset SSD1306 if requested and reset pin specified in constructor
  if(reset && (rstPin >= 0)) {
    pinMode(     rstPin, OUTPUT);
    digitalWrite(rstPin, HIGH);
    delay(1);                   // VDD goes high at start, pause for 1 ms
    digitalWrite(rstPin, LOW);  // Bring reset low
    delay(10);                  // Wait 10 ms
    digitalWrite(rstPin, HIGH); // Bring out of reset
  }

   // spi->beginTransaction(spiSettings);
  TRANSACTION_START
  // Init sequence
  static const uint8_t PROGMEM init1[] = {
    SSD1306_DISPLAYOFF,                   // 0xAE
    SSD1306_SETDISPLAYCLOCKDIV,           // 0xD5
    0x80,                                 // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX };               // 0xA8
  ssd1306_commandList(init1, sizeof(init1));
  ssd1306_command1(HEIGHT - 1);

  static const uint8_t PROGMEM init2[] = {
    SSD1306_SETDISPLAYOFFSET,             // 0xD3
    0x0,                                  // no offset
    SSD1306_SETSTARTLINE | 0x0,           // line #0
    SSD1306_CHARGEPUMP };                 // 0x8D
  ssd1306_commandList(init2, sizeof(init2));

  ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

  static const uint8_t PROGMEM init3[] = {
    SSD1306_MEMORYMODE,                   // 0x20
    0x00,                                 // 0x0 act like ks0108
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDEC };
  ssd1306_commandList(init3, sizeof(init3));

  static const uint8_t PROGMEM init4b[] = {
    SSD1306_SETCOMPINS,                 // 0xDA
    0x12,
    SSD1306_SETCONTRAST };              // 0x81
  ssd1306_commandList(init4b, sizeof(init4b));
  ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);
  
  ssd1306_command1(SSD1306_SETPRECHARGE); // 0xd9
  ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
  static const uint8_t PROGMEM init5[] = {
    SSD1306_SETVCOMDETECT,               // 0xDB
    0x40,
    SSD1306_DISPLAYALLON_RESUME,         // 0xA4
    SSD1306_NORMALDISPLAY,               // 0xA6
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYON };                 // Main screen turn on
  ssd1306_commandList(init5, sizeof(init5));

  TRANSACTION_END
  return true; // Success
}

void Adafruit_SSD1306::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if((x >= 0) && (x < width()) && (y >= 0) && (y < height())) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch(getRotation()) {
     case 1:
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      break;
     case 2:
      x = WIDTH  - x - 1;
      y = HEIGHT - y - 1;
      break;
     case 3:
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
    switch(color) {
     case WHITE:   buffer[x + (y/8)*WIDTH] |=  (1 << (y&7)); break;
     case BLACK:   buffer[x + (y/8)*WIDTH] &= ~(1 << (y&7)); break;
     case INVERSE: buffer[x + (y/8)*WIDTH] ^=  (1 << (y&7)); break;
    }
  }
}

void Adafruit_SSD1306::clearDisplay(void) {
  memset(buffer, 0, WIDTH * ((HEIGHT + 7) / 8));
}


uint8_t *Adafruit_SSD1306::getBuffer(void) {
  return buffer;
}

// REFRESH DISPLAY ---------------------------------------------------------

void Adafruit_SSD1306::display(void) {
  TRANSACTION_START
  static const uint8_t PROGMEM dlist1[] = {
    SSD1306_PAGEADDR,
    0,                         // Page start address
    0xFF,                      // Page end (not really, but works here)
    SSD1306_COLUMNADDR,
    0 };                       // Column start address
  ssd1306_commandList(dlist1, sizeof(dlist1));
  ssd1306_command1(WIDTH - 1); // Column end address
  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr   = buffer;

  SSD1306_MODE_DATA
  while(count--) SPIwrite(*ptr++);
  
  TRANSACTION_END
#if defined(ESP8266)
  yield();
#endif
}

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

Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);

void setup() {
  Serial.begin(9600);
pinMode(encoderPin1, INPUT);
pinMode(encoderPin2, INPUT);
digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
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
  Serial.print(encoderValue);
  disp.display();
  disp.clearDisplay();
}

int reverse(int x){
  if (x>=128) return 1;
  else return -1;
}
void setVelocity(){
  if (encoderValue>=255){
encoderValue = 255;
  } 
  else if (encoderValue<=0){
      encoderValue = 0;
  }
    dx = abs(dxMax *(encoderValue-128)/128);
    dy = abs(dyMax *(encoderValue-128)/128);
 
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
