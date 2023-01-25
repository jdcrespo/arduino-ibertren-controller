  #ifndef SCREEN_CUSTOM_CHARACTERS_H
    #define SCREEN_CUSTOM_CHARACTERS_H

    #include <Arduino.h>

    #define PROGRESS_BAR_FULL_ID 0 
    byte PROGRESS_BAR_FULL[] = {
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x1F
      };

    #define PROGRESS_BAR_LEFT_EMPTY_ID 1
    byte PROGRESS_BAR_LEFT_EMPTY[] = {
        0x1F,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x1F
      };

    #define PROGRESS_BAR_RIGHT_EMPTY_ID 2
    byte PROGRESS_BAR_RIGHT_EMPTY[] = {
        0x1F,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x1F
      };

    #define PROGRESS_BAR_MIDDLE_EMPTY_ID 3
    byte PROGRESS_BAR_MIDDLE_EMPTY[] = {
        0x1F,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x1F
      };

  #endif