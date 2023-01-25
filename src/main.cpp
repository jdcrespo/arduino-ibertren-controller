#include <Arduino.h>
#include "functions/screen/main.h"

LiquidCrystal_I2C lcd(DISPLAY_ADDRESS, DISPLAY_COLUMNS, DISPLAY_ROWS);
boolean currentDirection = true;
double lastDraw = 0;
boolean mustRedraw = false;
int lastPotPosition = 0;


boolean LED1State = false;
boolean LED2State = false;

long buttonTimer = 0;
long longPressTime = 500;

boolean buttonActive = false;
boolean longPressActive = false;

void setup()
{
  pinMode(ENCODER_BUTTON, INPUT);
  pinMode(MOTOR_A_EN, OUTPUT);
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);

  //TCCR1B = TCCR1B & B11111000 | B00000001; // set timer 1 divisor to 1 for PWM frequency of 31.37255 KHz

  //TCCR1B = TCCR1B & B11111000 | B00000100; // for PWM frequency of 122.55 Hz

  TCCR1B = TCCR1B & B11111000 | B00000101; // for PWM frequency of 30.64 Hz


  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  initScreen(lcd);
  
  drawSplashScreen(lcd);
  lastDraw = millis();

  lastPotPosition = analogRead(POTENTIOMETER_PIN);
  analogWrite(MOTOR_A_EN, 0);
  digitalWrite(MOTOR_A_IN1, currentDirection);
  digitalWrite(MOTOR_A_IN2, !currentDirection);
}
void loop()
{

  int newPotPosition = analogRead(POTENTIOMETER_PIN);
  if(abs(newPotPosition - lastPotPosition) > POTENTIOMETER_DEBOUNCE){
    lastPotPosition = newPotPosition;
    mustRedraw = true;
  }



  if(!digitalRead(ENCODER_BUTTON)){

		if(buttonActive == false){
			buttonActive = true;
			buttonTimer = millis();
		}

		if((millis() - buttonTimer > longPressTime) && (longPressActive == false)){
			longPressActive = true;
			LED1State = !LED1State;
      if(LED1State){
        Serial.println("entering menu");
      }else{
        Serial.println("exiting menu");
      }
		}

	}else{

		if(buttonActive == true){
			if(longPressActive == true){
				longPressActive = false;
			}else{
				currentDirection = !currentDirection;
        mustRedraw = true;
			}
			buttonActive = false;
		}
	}


  if((millis() - lastDraw) >= 500 || mustRedraw){
    
    drawMainScreen(map(lastPotPosition, 0, 1023, 0, 100), currentDirection, lcd);

    lastDraw = millis();
  }

  if(mustRedraw){
    analogWrite(MOTOR_A_EN, map(lastPotPosition, 0, 1023, 0, 255));
    digitalWrite(MOTOR_A_IN1, currentDirection);
    digitalWrite(MOTOR_A_IN2, !currentDirection);
  }

  mustRedraw = false;
}