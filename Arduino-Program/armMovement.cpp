//********************************************
// Author: Russell Saenz
// File: armMovement.cpp
// Description: Handles the math required for finding desired angles of the joints.
//       Uses delay to slow arm movement rather than millis because there is no alternate
//      tasks, so delay isnt blocking functionality.
//********************************************

#include "armMovement.h"
#include <LCDTools.h>
#include <math.h>

bool armIsOn = false;

bool calibrated = false;

//object of Grid for use
Grid grid;

//fil out grid object with grid information recieved from the cv program
void calibrateCamera() {
    while (Serial.available()==0) {}
    const double gridWidth = Serial.parseFloat();

    while (Serial.available() == 0) {}
    const double gridHeight = Serial.parseFloat();

    while (Serial.available() == 0) {}
    const uint8_t gridColumns = static_cast<uint8_t>(Serial.parseInt());

    while (Serial.available() == 0) {}
    const uint8_t gridRows = static_cast<uint8_t>(Serial.parseInt());

    grid = Grid(gridWidth, gridHeight, gridColumns, gridRows);
}

//initializing the servos and their charictaristics
ServoStruct baseServo(3, 90, 180, 0, false);
ServoStruct armServo1(5, 0, 45, 0, false);
ServoStruct armServo2(6, 0, 180, 0, true);

void setupArm() {
    if (!armIsOn) {
        baseServo.startServo();
        armServo1.startServo();
        armServo2.startServo();

        pinMode(magnetPin, OUTPUT);

        armIsOn = true;
    }
}

//attach servo
void ServoStruct::startServo() {
    //without the move to before the attach, the servo seems to jerk in
    //a random direction before going to home angle
    moveTo(homeAngle);

    servo.attach(pin);

    //delay needed to allow servo proper initialization time.
    delay(1000);
}

//detach servo
void ServoStruct::stopServo() {
    returnHome();

    delay(500);

    servo.detach();
}

//moves a servo to desired angle
void ServoStruct::moveTo(uint8_t targetAngle) {
    //can only move as far as the set max angle
    if (targetAngle > maxAngle) {
        targetAngle = maxAngle;

        printToScreen(F("TARGET > MAX"), targetAngle, messageTimer);
    }

    //needed to mount a servo in reverse, this fixes its movement issues
    if (reversed) {
        targetAngle = static_cast<uint8_t>(180) - targetAngle;
    }

    while (servo.read() != targetAngle) {
        uint8_t currentAngle = servo.read();

        if (currentAngle > targetAngle) {
            servo.write(currentAngle - 1);
        } else if (currentAngle < targetAngle) {
            servo.write(currentAngle + 1);
        }

        //delay to make movements not so jerky
        delay(movementDelay);
    }
}

//sweeps home to max to min then home
void ServoStruct::fullSweepMotion() {
    returnHome();

    delay(movementDelay);

    moveTo(maxAngle);
    moveTo(minAngle);

    delay(movementDelay);

    returnHome();
}

//function made for quick call back to home
void ServoStruct::returnHome() {
    moveTo(homeAngle);
}

//moves each servo individually
//run and observe the arms movement
void runDiagnostic() {
    armServo2.moveTo(30);
    baseServo.fullSweepMotion();
    baseServo.moveTo(45);
    armServo2.fullSweepMotion();
    armServo2.moveTo(30);
    armServo1.moveTo(15);
    armServo1.returnHome();
    baseServo.returnHome();
    armServo2.returnHome();
}

void calibrate() {
    if (!calibrated) {
        armServo2.moveTo(75);
        armServo1.moveTo(30);
        armServo2.moveTo(180);
        armServo1.moveTo(45);
        calibrated = true;
    }else {
        armServo1.moveTo(30);
        armServo2.moveTo(75);

        returnAllHome();
    }
}

// finds the arm angle based on distance
 void extendArmToDistance(const double distance, bool autoReturnHome) {
     printToScreen(F("DISTANCE RECEIVED: "), distance, messageTimer);

     //** FINDING NEEDED TRIANGLE **

     //a^2 + b^2 = c^2
    const double hypotenuse = sqrt(
         (distance * distance) + platformHeight * platformHeight);

    constexpr double armSegment1Height = 20.0;

    constexpr double armSegment2Height = 20.5;

    double cosineArm1 = ((armSegment1Height * armSegment1Height) + (hypotenuse * hypotenuse) - (armSegment2Height * armSegment2Height)) /
                    (2.0 * armSegment1Height * hypotenuse);

    double cosineArm2 = ((armSegment1Height * armSegment1Height) + (armSegment2Height * armSegment2Height) - (hypotenuse * hypotenuse)) /
                    (2.0 * armSegment1Height * armSegment2Height);

     cosineArm1 = constrain(cosineArm1, -1.0, 1.0);
     cosineArm2 = constrain(cosineArm2, -1.0, 1.0);

     const uint8_t armServo1Angle = static_cast<uint8_t>(abs(90 - (180 * acos(cosineArm1) / M_PI)));
     const uint8_t armServo2Angle = static_cast<uint8_t>(abs(180 * acos(cosineArm2) / M_PI));

     printToScreen(F("Calculating servo values"), NO_NUMBER, messageTimer);
     printToScreen(F("Hypotenuse: "), hypotenuse, messageTimer);
     printToScreen(F("ARM 1: "), armServo1Angle, messageTimer);
     printToScreen(F("ARM 2: "), armServo2Angle, messageTimer);

    //had to avoid hitting an overhead camera
    //decided to break the movements into 2 step process
    //step 1
    armServo2.moveTo(static_cast<uint8_t>(armServo2Angle/2));
    armServo1.moveTo(static_cast<uint8_t>(armServo1Angle/2));
    //step2
    armServo2.moveTo(armServo2Angle);
    armServo1.moveTo(armServo1Angle);


     if (autoReturnHome) {
         delay(longPause);

         armServo1.returnHome();
         armServo2.returnHome();
     }
 }

void swivelBaseToAngle(uint8_t angle, bool autoReturnHome) {
    printToScreen(F("Moving base to "), angle, messageTimer);

    baseServo.moveTo(angle);

    if (autoReturnHome) {
        delay(longPause);

        baseServo.returnHome();
    }
}

void Grid::findTriangle(const double distanceFromLeft, const double distanceFromBottom) {
    bool moveRight = false;
    double legX;
    //since the math to get the base angle is a triangle, direction was hard to grasp.
    //this allows left or right swivel depending on which side of th center the triangle is on
    if (distanceFromLeft > width/2) {
        legX =  distanceFromLeft - width/2 ;
        moveRight = true;
    }
    else {
        legX = width/2 - distanceFromLeft;
    }

    const double legY = platformOffset + distanceFromBottom;

    double baseServoAngle = atan(legY / legX);

    const double targetDistance = legY / sin(baseServoAngle);;


    //converts radians to degrees
    baseServoAngle = 180.0 * baseServoAngle / M_PI;

    printToScreen(F("TARGET DISTANCE: "), targetDistance, messageTimer);

    printToScreen(F("BASE ANGLE: "), baseServoAngle, messageTimer);

    extendArmToDistance(targetDistance, false);

    //handles left right swivel of base
    if (moveRight) {
        //The arm was completely home made. It has a slight lean to the left and the elbow is off to the left.
        //This was added to help combat the lean
        const double rightSideOffset = 15.0;

        swivelBaseToAngle(static_cast<uint8_t>(baseServoAngle-rightSideOffset), false);
    }else {
        swivelBaseToAngle(static_cast<uint8_t>(180 - baseServoAngle), false);
    }
}

//parses and orders movements
void parseForMovements() {
    //looking for distance from the left
    while (Serial.available() == 0) {}
    const double distanceFromLeft = Serial.parseFloat();

    //looking for distance from the bottom
    while (Serial.available() == 0) {}
    const double distanceFromBottom = Serial.parseFloat();


    //looking for distance from the left
    while (Serial.available() == 0) {}
    const double targeDistanceFromLeft = Serial.parseFloat();

    //looking for distance from the bottom
    while (Serial.available() == 0) {}
    const double targetDistanceFromBottom = Serial.parseFloat();

    //debugging messages on lcd/ in console
    printToScreen(F("DISTANCE FROM LEFT: "), distanceFromLeft, messageTimer);
    printToScreen(F("DISTANCE FROM BOTTOM: "), distanceFromBottom, messageTimer);
    printToScreen(F("COLUMN: "), targeDistanceFromLeft, messageTimer);
    printToScreen(F("ROW: "), targetDistanceFromBottom, messageTimer);


    //moving to where object is now
    grid.findTriangle(distanceFromLeft, distanceFromBottom);

    delay(pause);
    startMagnet();
    delay(longPause);

    //intermediate / neutral position to stop the arm from hittim the camera between movements
    armServo1.moveTo(15);
    armServo2.moveTo(65);

    //moving to where object will be
    grid.findTriangle(targeDistanceFromLeft, targetDistanceFromBottom);

    delay(pause);
    stopMagnet();
    delay(longPause);

    returnAllHome();

    delay(pause);
}

void returnAllHome() {
    printToScreen(F("Returning all home"), NO_NUMBER, messageTimer);

    baseServo.returnHome();
    delay(pause);
    armServo1.returnHome();
    delay(pause);
    armServo2.returnHome();
}

void stopAllServos() {
    if (armIsOn) {
        printToScreen(F("Stopping all servos"), NO_NUMBER, messageTimer);

        baseServo.stopServo();
        delay(pause);
        armServo1.stopServo();
        delay(pause);
        armServo2.stopServo();
    }

}

void startMagnet() {
    digitalWrite(magnetPin, HIGH);
}

void stopMagnet() {
    digitalWrite(magnetPin, LOW);
}
