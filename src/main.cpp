#include <Arduino.h>
#include "functions/screen/main.h"
#include "functions/screen/MainScreen.h"

LiquidCrystal_I2C lcd(DISPLAY_ADDRESS, DISPLAY_COLUMNS, DISPLAY_ROWS);
Screen screen(lcd);
MainScreen mainScreen(screen);

boolean currentDirection = true;
double lastDraw = 0;
boolean mustRedraw = false;

int lastPotPosition = 0;

int desiredSpeed = 0;
float currentSpeed = 0;

boolean isMenuActive = false;

boolean isSerialAppActive = true;

bool currentSelectedParam = false;
int accParam = mainScreen.getAccParam();
int decParam = mainScreen.getDecParam();

void printSpeedInfo(boolean isBraking)
{
  if (isSerialAppActive)
  {
    return;
  }
  if (isBraking)
  {
    Serial.print("brake");
  }
  else
  {
    Serial.print("accel");
  }
  Serial.print(" pwr: ");
  Serial.print(desiredSpeed);
  Serial.print(" spd: ");
  Serial.println(currentSpeed);
}

void sendAppCommand(String command)
{
  if (isSerialAppActive)
  {
    Serial.println(command);
  }
}

unsigned long last_run = 0;
int encoder_value = 0;

void shaft_moved()
{
  if (millis() - last_run > 2)
  {
    if (digitalRead(ENCODER_INPUT_A))
    {
      encoder_value++;
    }
    else
    {
      encoder_value--;
    }
    last_run = millis();
  }
}

void setup()
{
  pinMode(ENCODER_BUTTON, INPUT);
  pinMode(DIRECTION_SWITCH_PIN, INPUT);
  pinMode(LED_INDICATOR, OUTPUT);
  pinMode(MOTOR_A_EN, OUTPUT);
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_INPUT_B), shaft_moved, LOW);

  // TCCR1B = TCCR1B & B11111000 | B00000001; // set timer 1 divisor to 1 for PWM frequency of 31.37255 KHz

  // TCCR1B = TCCR1B & B11111000 | B00000100; // for PWM frequency of 122.55 Hz

  TCCR1B = TCCR1B & B11111000 | B00000101; // for PWM frequency of 30.64 Hz

  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  screen.init();

  drawSplashScreen(lcd);
  lastDraw = millis();

  currentDirection = digitalRead(DIRECTION_SWITCH_PIN);

  analogWrite(MOTOR_A_EN, 0);
  digitalWrite(MOTOR_A_IN1, currentDirection);
  digitalWrite(MOTOR_A_IN2, !currentDirection);
}

void loop()
{
  /**
   * CHECK THROTLE POSITION
   */
  int newPotPosition = analogRead(POTENTIOMETER_PIN);
  if (abs(newPotPosition - desiredSpeed) > POTENTIOMETER_DEBOUNCE)
  {
    desiredSpeed = newPotPosition;
    mustRedraw = true;
    sendAppCommand(String("THROTLE-1=") + String(newPotPosition));
  }

  /**
   * CHECK REVERSER POSITION
   */
  bool newDirection = digitalRead(DIRECTION_SWITCH_PIN);
  if (newDirection != currentDirection)
  {
    currentDirection = newDirection;
    mustRedraw = true;
  }

  /**
   * CHECK JOB BUTTON
   */
  mustRedraw = mustRedraw || screen.updateButtonState(!digitalRead(ENCODER_BUTTON));

  /**
   * CHECK ENCODER VALUE
   */
  if (encoder_value != 0)
  {
    Serial.println(encoder_value);
    screen.updateCursorValue(encoder_value);
    mustRedraw = true;
  }

  /**
   * HANDLE SCREEN AND PARAMETERS UPDATE
   */
  if ((millis() - lastDraw) >= 500 || mustRedraw)
  {
    if (screen.getLongPress())
    {
      lcd.clear();
      isMenuActive = !isMenuActive;
    }

    if (isMenuActive)
    {
      lcd.setCursor(0, 0);
      lcd.print("ACA EL MENU");
    }
    else
    {
      accParam = mainScreen.getAccParam();
      decParam = mainScreen.getDecParam();
      mainScreen.Draw(
          map(desiredSpeed, 0, 1023, 0, 100),
          map(currentSpeed, 0, 1023, 0, 100),
          !currentDirection);
    }
    lastDraw = millis();
  }

  if (mustRedraw)
  {
    // analogWrite(MOTOR_A_EN, map(lastPotPosition, 0, 1023, 0, 255));
    digitalWrite(MOTOR_A_IN1, currentDirection);
    digitalWrite(MOTOR_A_IN2, !currentDirection);
  }

  // braking
  if (desiredSpeed < (int)currentSpeed)
  {
    float brakingAcc = 1.0 + (decParam / 100000.0);
    currentSpeed = currentSpeed / brakingAcc;
    if (currentSpeed < 0)
    {
      currentSpeed = 0;
    }
    analogWrite(MOTOR_A_EN, map(currentSpeed, 0, 1023, 0, 255));
    printSpeedInfo(true);
  }

  // acceleration
  if (desiredSpeed > (int)currentSpeed)
  {
    if (currentSpeed == 0)
    {
      currentSpeed = 1;
    }
    else
    {
      float acceleration = 1.0 + (accParam / 100000.0);
      currentSpeed = currentSpeed * acceleration;
    }

    if (currentSpeed > desiredSpeed)
    {
      currentSpeed = desiredSpeed;
    }
    analogWrite(MOTOR_A_EN, map(currentSpeed, 0, 1023, 0, 255));
    printSpeedInfo(false);
  }

  // INDICATE IF TRACK HAS POWER
  digitalWrite(LED_INDICATOR, (int)currentSpeed > 50);

  // CLEAN VARIABLES FOR NEXT CYCLE
  mustRedraw = false;
  encoder_value = 0;
  screen.cleanVars();
}