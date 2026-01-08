//
// Created by Rusty on 1/2/2026.
//
#pragma once

#include <Arduino.h>
#define NO_NUMBER (-1)

//set true to have lcd outputs
#define DEBUG_LCD (false)


extern uint8_t messageTimer;

void setupLCD();

void setScreen();

void printToScreen(const char *message, float number, uint8_t messageDuration);
