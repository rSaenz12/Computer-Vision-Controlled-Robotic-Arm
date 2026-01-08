//
// Created by Rusty on 1/2/2026.
//
#include <wire.h>
#include <LiquidCrystal_I2C.h>
#include "LCDTools.h"

LiquidCrystal_I2C lcd(0x27, 16,2);

uint8_t messageTimer = 5;

void setupLCD() {
    //LCD SETUP
    Wire.begin();

    lcd.init();
    lcd.backlight();
    lcd.clear();
}

//if message duration ==0, then it displays until the next print
//message duration is also sent in seconds
void printToScreen(const char *message,const float number, const uint8_t messageDuration) {
    Serial.println(message);
    if (number>NO_NUMBER) {
        Serial.println(number);
    }

    #if DEBUG_LCD
        int counter = 0;
        int row = 0;

        lcd.clear();

        lcd.setCursor(0, row);

        for (int i = 0; message[i] != '\0'; i++) {
            if (counter >= 16) {
                row = (row + 1) % 2;
                lcd.setCursor(0, row);
                counter = 0;
            }
            lcd.print(message[i]);
            counter++;
        }
        if (number > NO_NUMBER) {
            if (counter > 10) {
                row = (row + 1) % 2;
                lcd.setCursor(0, row);
            }
            lcd.print(messageDuration);
        }
        if (messageDuration > 0) {
            //messageDuration is in seconds, delay deals with miliseconds.
            delay(messageDuration * 1000);
            lcd.clear();
        }
    #endif
}


void setScreen() {

    printToScreen("Enter q, c, f,  m, l, a, h, or g", NO_NUMBER, 0);


}
