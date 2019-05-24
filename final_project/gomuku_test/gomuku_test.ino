/******************************************************************
 This is an example for the Adafruit RA8875 Driver board for TFT displays
 ---------------> http://www.adafruit.com/products/1590
 The RA8875 is a TFT driver for up to 800x480 dotclock'd displays
 It is tested to work with displays in the Adafruit shop. Other displays
 may need timing adjustments and are not guanteed to work.

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source hardware
 by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.


 Sources:
 Gomuku game functions and classes that implement the rules:
 https://github.com/tdang33/Gomoku-Five-in-a-row-
 ******************************************************************/


/******************************************************************
Debug Notes:

having a main.cpp file screws up execution; serial printing setup don't get executed

while loop inside run is problematic

board[x][y] = 'x'; is what is messing with execution; possibly supressing errors?
 ******************************************************************/


#include <SPI.h>
#include <math.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
#define RA8875_INT 3
#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx = -1, ty = -1;
int norm_x = -1, norm_y = -1;
double x_sum = 0;
double y_sum = 0;
int old_x = -1, old_y = -1;
const int N = 15;
const int M = 5;
char board[N][N];
char playerSymbol;
char computerSymbol;
int count;
int row_count = 15;
double width = 480/row_count;
int last_x, last_y;
 using namespace std;
class Board{

  public:
  
  inline Board();
  inline void clear();
  inline void print(int);
  inline bool checkEmpty(int, int);
  inline bool win(int, int);
  inline bool draw();
  inline char getValue(int, int);
  inline void setValue(int, int, char);
  inline int getCount();

  char getPlayerSymbol(){
    return playerSymbol;
  }
  char getComputerSymbol(){
    return computerSymbol;
  }
  void setPlayerSymbol(char c){
    playerSymbol = c;
  }
  void setComputerSymbol(char c){
    computerSymbol = c;
  }
  inline void render();
  inline bool rowOfFive(int, int);
  inline bool columnOfFive(int, int);
  inline bool mainDiagnolOfFive(int, int);
  inline bool reverseDiagnolOfFive(int, int);
  //std::string toString();
  //bool checkEmpty(int, int);
};

void Board::clear(){
  tft.fillScreen(RA8875_WHITE);
  tft.fillTriangle(550, 275, 650, 275, 600,150, RA8875_GREEN);
  tft.fillTriangle(550, 475, 650, 475, 600,350, RA8875_RED);

    for (int i = 0; i < row_count; i++)
    {
      tft.drawLine(0, width*i, 480, width*i, RA8875_BLACK);
      tft.drawLine(width*i, 0, width*i, 480, RA8875_BLACK);
    }

    for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      board[i][j] = '.';
    }
    
  }
}

//constructor . Create a new empty board
Board::Board(){
  clear();
}

void Board::render(){
    for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      if (board[i][j] == 'O'){
            tft.fillCircle((uint16_t)(i*width), (uint16_t)(j*width), 4, RA8875_BLACK);

      }else if (board[i][j] == 'X'){
            tft.fillCircle((uint16_t)(i*width), (uint16_t)(j*width), 4, RA8875_RED);

      }
    }
    }
  }
//check if the game is over after the move at position (x,y) or not
bool Board::win(int x, int y){
  return rowOfFive(x,y) || columnOfFive(x,y) || mainDiagnolOfFive(x,y) || reverseDiagnolOfFive(x,y);
}



bool Board::rowOfFive(int x, int y){
  //check for rows
  int temp = 1;
  int i = 1;
  while (y-i >= 0 && board[x][y-i] == board[x][y]){
    temp++;
    i++;
  }
  i = 1;
  while (y + i <= N-1 && board[x][y+i] == board[x][y]){
    temp++;
    i++;
  }
  return (temp >= M); 
}

bool Board::columnOfFive(int x, int y){
  //check for columns
  int temp = 1;
  int i = 1;
  while (x-i >= 0 && board[x-i][y] == board[x][y]){
    temp++;
    i++;
  }
  i = 1;
  while (x + i <= N-1 && board[x+i][y] == board[x][y]){
    temp++;
    i++;
  }
  return (temp >= M);
}

bool Board::mainDiagnolOfFive(int x, int y){
  //check for main diagnol
  int temp = 1;
  int i = 1;
  while (x-i >= 0 && y - i >= 0 && board[x-i][y-i] == board[x][y]){
    temp++;
    i++;
  }
  i = 1;
  while (x + i <= N-1 && y + i <= N-1 && board[x+i][y+i] == board[x][y]){
    temp++;
    i++;
  }
  return (temp >= M);
}

bool Board::reverseDiagnolOfFive(int x, int y){
  //check for reverse diagnol
  int temp = 1;
  int i = 1;
  while (x-i >= 0 && y + i <= N-1 && board[x-i][y+i] == board[x][y]){
    temp++;
    i++;
  }
  i = 1;
  while (x + i <= N-1 && y - i >= 0 && board[x+i][y-i] == board[x][y]){
    temp++;
    i++;
  }
  return (temp >= M);
}

//determine if the game's result is draw or not.
bool Board::draw(){
  if (count == N*N){
    return true;
  }
  return false;
}

//return value at position (i,j)
char Board::getValue(int i, int j){
  return board[i][j];
}

//return number of cells with placed X or O
int Board::getCount(){
  return count;
}

class Game{
  private:
  Board board;

  
  public:
  inline Game();
  inline void playConsole();

  inline char getCurrentSymbol(int);

  inline void run();
};

Game::Game(){
  board.clear();
}

//check to see the position (x,y) is eligible or not (for both the player and computer)
bool Board::checkEmpty(int x, int y){
  
  if (x >= 0 && x <= N-1 && y >= 0 && y <= N-1 && board[x][y] == '.'){
    return true;
  }
  return false;
}

//put X or O at clear the cell at position (x,y)
void Board::setValue(int x, int y, char c){
  board[x][y] = c;
  if ( c == '.'){
    count--;
  }else {
    count++;
  }
  return;
}

void Board::print(int current){
  if (current == 1)
    tft.fillCircle((uint16_t)(norm_x*width), (uint16_t)(norm_y*width), 4, RA8875_BLACK);
  else
    tft.fillCircle((uint16_t)(norm_x*width), (uint16_t)(norm_y*width), 4, RA8875_RED);
}


//play the game in console. Here is the structure of how the game is played
void Game::playConsole(){
  
  board.clear();
  bool flag = true;
  int current = 1;
  int playerChoice = 1;
  
  if (playerChoice == 1){
    board.setPlayerSymbol('X');
  }else {
    board.setPlayerSymbol('O');
  }

  char playerGoFirst = 'Y';

  if (playerGoFirst == 'Y' || playerGoFirst == 'y'){
    current = 1;
  } else {
    current = 2;
  }

  //tft.textMode();
  //tft.cursorBlink(32);


  /* Set a solid for + bg color ... */

  /* ... or a fore color plus a transparent background */


//  /* Set the cursor location (in pixels) */
//  tft.textSetCursor(600, 250);
//
//  /* Render some text! */
//  char string[15] = "Hello, World! ";
//  tft.textTransparent(RA8875_BLACK);
//  tft.textWrite(string);

  
  //tft.drawLine(0, 0, 300, 300, RA8875_RED);
  while (flag){
    delay(300); // this prevents multiple pieces from being placed
    // need to get x, y
    tft.touchEnable(true);
    x_sum = 0;
    y_sum = 0;
    int num_readings = 0;
    while (old_x == norm_x && old_y == norm_y)
    {
        num_readings++;
        float xScale = 1024.0F/tft.width();
        float yScale = 1024.0F/tft.height();
      
        /* Wait around for touch events */
        if (! digitalRead(RA8875_INT))
        {
          if (tft.touched())
          {
            Serial.println("Old:");
            Serial.println(old_x);
            Serial.println(old_y);

            Serial.print("Touch: ");
            tft.touchRead(&tx, &ty);
            Serial.print(tx); Serial.print(", "); Serial.println(ty);
            /* Draw a circle */
            x_sum += tx/(width*xScale);
            y_sum += ty/(width*yScale);
            norm_x = (int)(tx/(width*xScale));
            norm_y = (int)(ty/(width*yScale));
            //if (x
            Serial.println("New:");
            Serial.println(norm_x);
            Serial.println(norm_y);
          }
        }
        
    }
    tft.touchEnable(false);

    // actually normalize now
    //if (x_sum>0 && y_sum>0){
      double tmp = y_sum/num_readings;
      norm_x = (int)(x_sum/num_readings);
      Serial.println(tmp);
      //Serial.println(x_sum/num_readings);
      if (tmp < 5.3 && tmp > 2.9) {
        tmp -= 0.6;
      }else if (tmp <= 2.9){
        tmp -= 1;
      }else if (tmp > 8.8 && tmp < 11){
        tmp += 0.7;
      }else if (tmp >= 11){
        tmp += 1;
      }

      norm_y = (int)tmp;

       old_x = norm_x;
       old_y = norm_y;
    //}
    // restart the game
    if ( abs(ty-760) + abs(tx-530) < 200 )
    {
      board.clear();
      continue;
    }

//    if ( abs(ty-960) + abs(tx-530) < 100 )
//    {
//      Serial.println("UNDO!!!!!!!!!!!!!!!");
//      //board.setValue(last_x, last_y, '.');
//      tft.fillScreen(RA8875_WHITE);
//  tft.fillTriangle(550, 275, 650, 275, 600,150, RA8875_GREEN);
//  tft.fillTriangle(550, 475, 650, 475, 600,350, RA8875_RED);
//
//    for (int i = 0; i < row_count; i++)
//    {
//      tft.drawLine(0, width*i, 480, width*i, RA8875_BLACK);
//      tft.drawLine(width*i, 0, width*i, 480, RA8875_BLACK);
//    }
//    board.render();
//     //current = 3 - current;
//
//
//      continue;
//    }

    Serial.println("__________ONE MOVE_________");
    if (!board.checkEmpty(norm_x, norm_y)){
      continue; 
    }else {//if the move is legit, update the board
      Serial.println("empty, filling!");
      board.setValue(norm_x, norm_y, getCurrentSymbol(current));
      last_x = norm_x;
      last_y = norm_y;
      board.print(current);
      
    }

    //check if the game is over or not
    if (board.win(norm_x, norm_y)){
      //board.print(current);
      Serial.println("WON!");


      
      tft.textMode();
      tft.textSetCursor(300, 300);

  /* Render some text! */
  char string[15] = "WIN!";
//  tft.textTransparent(RA8875_WHITE);
//  tft.textWrite(string);
  tft.textEnlarge(10);
  tft.textColor(RA8875_WHITE, RA8875_RED);
  tft.textWrite(string);
//  tft.textTransparent(RA8875_CYAN);
//  tft.textWrite(string);
//  tft.textTransparent(RA8875_GREEN);
//  tft.textWrite(string);
//  tft.textColor(RA8875_YELLOW, RA8875_CYAN);
//  tft.textWrite(string);
//  tft.textColor(RA8875_BLACK, RA8875_MAGENTA);
//  tft.textWrite(string);
      //tft.fillScreen(RA8875_RED);
      tft.graphicsMode();
      delay(3000);
      board.clear();
      
    }else if (board.draw()){
      board.print(current);
      // cout << "Draw game!"<<endl;
      break;
    }else{ // if the game is not over, change current player between the player and the computer
      Serial.println("CHange color");
      current = 3 - current;
    }
    
  }
}


//get symbol of the current player
char Game::getCurrentSymbol(int current){
  if (current == 1){
    return board.getPlayerSymbol();
  }else{
    return board.getComputerSymbol();
  }
}

//start the game
void Game::run(){
  bool flag = true;
  char x;
  Serial.println("outside loop");
  playConsole();
}


void setup()
{
  Serial.begin(9600);
  Serial.println("RA8875 start");

  /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  Serial.println("Found RA8875");

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  Game game;
  
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);

  tft.touchEnable(true);

  Serial.print("Status: "); Serial.println(tft.readStatus(), HEX);
  Serial.println("Waiting for touch events ...!!!");
  //tft.fillScreen(RA8875_RED);
  game.run();
}

void loop()
{

}
