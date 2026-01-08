//********************************************
// Author: Russell Saenz
// File: LCDTools.h
// Description: Defines NO_NUMBER, DEBUG_LCD and the functions for the LCD.
//********************************************

#pragma once

#include <Arduino.h>
#define NO_NUMBER (-1)

//set true to have lcd outputs
#define DEBUG_LCD (false)


extern uint8_t messageTimer;

void setupLCD();

void setScreen();

void printToScreen(const char *message, float number, uint8_t messageDuration);
