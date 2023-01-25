#include "main.h"
#include "customCharacters.h"

void initScreen(LiquidCrystal_I2C lcd)
{
  lcd.createChar(PROGRESS_BAR_FULL_ID, PROGRESS_BAR_FULL);
  lcd.createChar(PROGRESS_BAR_LEFT_EMPTY_ID, PROGRESS_BAR_LEFT_EMPTY);
  lcd.createChar(PROGRESS_BAR_RIGHT_EMPTY_ID, PROGRESS_BAR_RIGHT_EMPTY);
  lcd.createChar(PROGRESS_BAR_MIDDLE_EMPTY_ID, PROGRESS_BAR_MIDDLE_EMPTY);
}

void drawSplashScreen(LiquidCrystal_I2C lcd)
{
  lcd.setCursor(4,0);
  lcd.print("Ibertren");
  lcd.setCursor(1,1);
  lcd.print("PWM Controller");
  delay(2000);
  lcd.clear();
}

void drawMainScreen(int currentPower, boolean currentDirection, LiquidCrystal_I2C lcd)
{
  lcd.setCursor(0,1);
  lcd.print("Pwr:");
  lcd.setCursor(9,1); 
  lcd.print("Dir:");

  int numBoxesFull = map(currentPower, 0, 100, 0, DISPLAY_COLUMNS);

  for (int i = 0; i < DISPLAY_COLUMNS; i++)
  {

    lcd.setCursor(i, 0);
    if(i <= numBoxesFull && currentPower > 0){
      lcd.write(PROGRESS_BAR_FULL_ID);
    }else{
      if(i == 0){
        lcd.write(PROGRESS_BAR_LEFT_EMPTY_ID);
      }else if(i == DISPLAY_COLUMNS -1){
        lcd.write(PROGRESS_BAR_RIGHT_EMPTY_ID);
      }else{
        lcd.write(PROGRESS_BAR_MIDDLE_EMPTY_ID);
      }
    }
  }
  
  int spacesRequired = 3 -((String)currentPower).length();  
  lcd.setCursor(4 + spacesRequired, 1);

  lcd.print(currentPower);
  lcd.setCursor(7,1);
  lcd.print("%");
  lcd.setCursor(14, 1);
  if(currentDirection){
    lcd.print("->");
  }else{
    lcd.print("<-");
  }
}