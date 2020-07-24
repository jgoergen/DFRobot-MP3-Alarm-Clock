#include <Keypad.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h" // https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
// http://markus-wobisch.blogspot.com/2016/09/arduino-sounds-dfplayer.html
// https://www.amazon.com/gp/product/B01MQ0GFZV/ref=ppx_yo_dt_b_asin_title_o08_s00?ie=UTF8&psc=1

// SETTINGS ////////////////////////////////////////////
#define ALARM_VOLUME            20
#define MAX_SOUND_INDEX         55

// PIN DEFINITIONS /////////////////////////////////////
#define MAX7219_CLK_PIN         12
#define MAX7219_CS_PIN          11
#define MAX7219_DIN_PIN         10
#define MP3_RX_PIN              14
#define MP3_TX_PIN              15


// MISC DEFS
#define MAX7219_CHAIN_LENGTH    4
#define CHAR_SPACING            1
#define BUF_SIZE                75
#define KEYPAD_ROWS             4
#define KEYPAD_COLS             4
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
byte KEYPAD_ROW_PINS[KEYPAD_ROWS] = {9, 8, 7, 6}; 
byte KEYPAD_COL_PINS[KEYPAD_COLS] = {5, 4, 3, 2}; 
char KEYPAD_KEY_DEFINITIONS[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, MAX7219_DIN_PIN, MAX7219_CLK_PIN, MAX7219_CS_PIN, MAX7219_CHAIN_LENGTH);
Keypad customKeypad = Keypad(makeKeymap(KEYPAD_KEY_DEFINITIONS), KEYPAD_ROW_PINS, KEYPAD_COL_PINS, KEYPAD_ROWS, KEYPAD_COLS); 
SoftwareSerial mySoftwareSerial(MP3_RX_PIN, MP3_TX_PIN); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

char keyReading;

void setup(){
  
  Serial.begin(115200);
  randomSeed(analogRead(A7));
  Display_Init();
  Audio_Init();
  Clock_Init();
  Audio_PlayRandomSound();
}
  
void loop() {
  
  keyReading = customKeypad.getKey();

  if (keyReading) {
    
    Clock_HandleKey(keyReading);
    keyReading = "";
  }

  Audio_Update();
  Clock_Update();
  delay(33);
}
