#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "customCharacters.h"
#include "main.h"

class Screen
{
private:
  int cursorValue;
  LiquidCrystal_I2C &lcd;

  long buttonTimer;
  long longPressTime = 500;
  bool buttonActive;
  bool longPressActive;
  bool isShortPress;
  bool isLongPress;

public:
  Screen(LiquidCrystal_I2C &lcdObj);
  void init();
  LiquidCrystal_I2C &getLCD();
  void updateCursorValue(int newValue);
  bool updateButtonState(bool buttonRead);
  int getCursorValue();
  bool getShortPress();
  bool getLongPress();
  void cleanVars();
};

Screen::Screen(LiquidCrystal_I2C &lcdObj) : lcd(lcdObj)
{
  buttonTimer = 0;
  buttonActive = false;
  longPressActive = false;
  isShortPress = false;
  isLongPress = false;
}

void Screen::init()
{
  lcd.createChar(PROGRESS_BAR_FULL_ID, PROGRESS_BAR_FULL);
  lcd.createChar(PROGRESS_BAR_LEFT_EMPTY_ID, PROGRESS_BAR_LEFT_EMPTY);
  lcd.createChar(PROGRESS_BAR_RIGHT_EMPTY_ID, PROGRESS_BAR_RIGHT_EMPTY);
  lcd.createChar(PROGRESS_BAR_MIDDLE_EMPTY_ID, PROGRESS_BAR_MIDDLE_EMPTY);
}

LiquidCrystal_I2C &Screen::getLCD()
{
  return lcd;
}

void Screen::updateCursorValue(int newValue)
{
  cursorValue = newValue;
}

int Screen::getCursorValue()
{
  return cursorValue;
}

bool Screen::updateButtonState(bool buttonRead)
{
  bool mustRedraw = false;
  if (buttonRead)
  {

    if (buttonActive == false)
    {
      buttonActive = true;
      buttonTimer = millis();
    }

    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false))
    {
      longPressActive = true;
      /**
       * LONG PRESS ACTION
       */
      isLongPress = true;
      // Serial.println("LONG PRESS");
      mustRedraw = true;
    }
  }
  else
  {

    if (buttonActive == true)
    {
      if (longPressActive == true)
      {
        longPressActive = false;
      }
      else
      {
        /**
         * SHORT PRESS ACTION
         */
        isShortPress = true;
        // Serial.println("SHORT PRESS");
        mustRedraw = true;
      }
      buttonActive = false;
    }
  }
  return mustRedraw = true;
  ;
}

bool Screen::getShortPress()
{
  return isShortPress;
}

bool Screen::getLongPress()
{
  return isLongPress;
}

void Screen::cleanVars()
{
  isShortPress = false;
  isLongPress = false;
  cursorValue = 0;
}

#endif