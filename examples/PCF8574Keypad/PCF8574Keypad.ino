#include "Wire.h"
#include "PCF8574.h"
#include "KeypadPCF8574.h"

const uint8_t PCF8574IntPin = 4;
const uint8_t deviceAddress = 0x20;
const uint8_t SDAPin = 21;//32;
const uint8_t SCLPin = 22;//33;
const byte KeypadRows = 4; //four rows
const byte KeypadCols = 4; //four columns
const uint KeypadDebouncems = 50;
const uint KeypadHoldms = 50;

const uint8_t IOExpanderInitValue = 0xFF;
PCF8574 IOExpander = PCF8574(deviceAddress,&Wire);

char KeypadKeys[KeypadRows][KeypadCols] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte KeypadRowPins[KeypadRows] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte KeypadColPins[KeypadCols] = {0, 1, 2, 3}; //connect to the column pinouts of the keypad

KeypadPCF8574 keypad = KeypadPCF8574(&IOExpander, makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins, KeypadRows, KeypadCols );

void PrintKey(char key)
{
  if (keypad.isPressed(key))
  {
    Serial.print("K:");
    Serial.println(key);
  }
  //Serial.print("K:");
  //Serial.print(key);
  //Serial.print(":");
  //Serial.println(keypad.isPressed(key));
}

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);

  pinMode(PCF8574IntPin,INPUT_PULLUP);
  //Wire.begin(SDAPin, SCLPin);
  //Wire.setClock(100000);
  keypad.addEventListener(&PrintKey);
  while(IOExpander.begin(SDAPin, SCLPin, IOExpanderInitValue) == false)
  {
    Serial.println("ERROR: cannot communicate to keypad.\n");
    delayMicroseconds(100000);
  }
  keypad.setDebounceTime(KeypadDebouncems);
  keypad.setHoldTime(KeypadHoldms);
}

void loop()
{
  keypad.getKeys();
}