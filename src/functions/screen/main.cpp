#include "main.h"

void drawSplashScreen(LiquidCrystal_I2C lcd)
{
  lcd.setCursor(4, 0);
  lcd.print("Ibertren");
  lcd.setCursor(1, 1);
  lcd.print("PWM Controller");
  delay(2000);
  lcd.clear();
}

void drawMenuScreen(
    int encoderValue,
    LiquidCrystal_I2C lcd)
{
  String options[4] = {
      "PWM Frequency",
      "Test item 1",
      "Test item 2",
      "Save setting"};
}