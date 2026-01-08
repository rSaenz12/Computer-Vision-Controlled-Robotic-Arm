//
// Created by Rusty on 1/2/2026.
//

#pragma once


#include <Arduino.h>
#include <Servo.h>

//added length / offset of the magnet and its fastener
constexpr double pickupClearance = 0.5;
constexpr double magnetDangle = 1.0;

//because arm is on a platform above the surface
constexpr double platformHeight = 6.5 + pickupClearance + magnetDangle;

//because distance is from front of platform which extends ahead of the actual arm
constexpr double platformOffset = 9.5;

//delays
constexpr uint8_t movementDelay = 45;
constexpr int pause = 500;
constexpr int longPause = 5000;

constexpr uint8_t magnetPin = 8;

struct Grid {
    double width = 22.0;
    double height = 18.0;

    uint8_t rows = 3;
    uint8_t columns = 3;

    //default constructor of default values
    Grid() = default;

    //initializer
    Grid(const double width, const double height, const uint8_t rows, const uint8_t columns) : width(width),
        height(height), rows(rows), columns(columns) {
    }

    void findTriangle(double distanceFromLeft, double distanceFromBottom);
};

struct ServoStruct {
    Servo servo;

    const char *name;

    uint8_t pin;

    uint8_t homeAngle;
    uint8_t maxAngle;
    uint8_t minAngle;

    bool reversed = false;

    ServoStruct(const char *name, const uint8_t pin, const uint8_t homeAngle, const uint8_t maxAngle,
                const uint8_t minAngle,
                const bool reversed) : name(name), pin(pin), homeAngle(homeAngle), maxAngle(maxAngle),
                                       minAngle(minAngle), reversed(reversed) {
    }

    void startServo();

    void stopServo();

    void moveTo(uint8_t targetAngle);

    void fullSweepMotion();

    void returnHome();
};


void calibrateCamera();

void setupArm();

void promptUser();

void sweepServoMenu();

void runDiagnostic();

void calibrate();

void extendArmToDistance(double distance, bool autoReturnHome);

void swivelBaseToAngle(uint8_t angle, bool autoReturnHome);

void parseForMovements();

void returnAllHome();

void stopAllServos();

void startMagnet();

void stopMagnet();

