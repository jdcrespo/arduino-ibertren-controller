#ifndef SCREEN_MAIN_H
    #define SCREEN_MAIN_H

    #include "../../settings.h"

    #include <Arduino.h>
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>

    void initScreen(LiquidCrystal_I2C lcd);

    void drawSplashScreen(LiquidCrystal_I2C lcd);

    void drawMainScreen(int currentPower, boolean currentDirection, LiquidCrystal_I2C lcd);

#endif