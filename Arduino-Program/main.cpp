#include <Arduino.h>
#include <LCDTools.h>
#include <armMovement.h>

void setup() {
    Serial.begin(9600);

    setupLCD();

    setupArm();
}

void loop() {
    setScreen();

    while (Serial.available() == 0) {}

    const int keyCode = static_cast<int>(Serial.parseInt());

    switch (keyCode) {
        //cameraCalibration
        case 1000: {
            calibrateCamera();
            break;
        }
        //shutting down
        case 999: {
            stopAllServos();
            break;
        }
        //starts all servos, use if you shut down the servos and want to continue operation after
        case 998: {
            setupArm();
            break;
        }
            //runs caliratoin for arm, user lines up the arm with the camera
        case 900: {
            calibrate();
            break;
        }
        //moves arm in every direction
        case 850: {
            runDiagnostic();
            break;
        }
        //arm movement
        case 700: {
            parseForMovements();
            break;
        }
        //testing magnet functionality
        case 1: {
            startMagnet();
            delay(5000);
            stopMagnet();
            break;
        }
        default: {
            break;
        }
    }
}
