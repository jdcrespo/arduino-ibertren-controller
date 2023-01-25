#ifndef SETTINGS_H
    #define SETTINGS_H

    /* MAIN POTENTIOMETER INPUT */
    #define POTENTIOMETER_PIN A0
    #define POTENTIOMETER_DEBOUNCE 4

    /* ROTARY ENCODER PINOUT */
    #define ENCODER_INPUT_A 2
    #define ENCODER_INPUT_B 3
    #define ENCODER_BUTTON 4

    /* DISPLAY SETTINGS */
    #define DISPLAY_ADDRESS 0x27
    #define DISPLAY_COLUMNS 16
    #define DISPLAY_ROWS 2

    /* L298N PINOUT */
    #define MOTOR_A_EN 9
    #define MOTOR_A_IN1 5
    #define MOTOR_A_IN2 6
    #define MOTOR_B_EN 10
    #define MOTOR_B_IN1 7
    #define MOTOR_B_IN2 8
    
#endif