//********************************************
// Author: Russell Saenz
// File: inputHandling.cpp
// Description: Handles user inputs, calling corresponding functions
//********************************************

#include "inputHandling.h"

using namespace std;

//***********************WARNING*****************************************
//IF USING LESS THAN C++ 20, USE ONE OF THE FIRST TWO OPTIONS
//
//C style
// char message[64];
// snprintf(message, sizeof(message), "<%s,%.2f,%.2f>\n", arduino.measurementCode.c_str(),  heightInches, widthInches);
//
//C++ 11 OR UP
//Needs message.str() to turn to string
// ostringstream message;
// message << arduino.measurementCode <<" "<< heightInches <<" "<< widthInches;
//
//c++ 20 OR UP
//Currently used in this code
// string message = format("<{},{:.2f},{:.2f}>", arduino.measurementCode, heightInches, widthInches);
//
// FOR ALL
//**NOTE**
// Data type of message must be changed in the function signature to reflect the method.
// arduino.arduinoSerialOutput(message);
//********************************************************************

void processInput(const int input, Object &object, Arduino &arduino, Calibration &calibration, const vector<Cell> &cells,
                  const cv::Mat &frame, State &state) {
    switch (input) {
        //callibrating the measurment using a 12 inch ruler on the surface
        case 'c': {
            cameraCalibration(object, arduino, calibration, frame, state);
            break;
        }
        case 'm': {
            measureObject(object, calibration, frame, state);
            break;
        }
        case 'l': {
            locateObject(object, cells, state);
            break;
        }
        case 'f': {
            calibrateArm(arduino, state);
            break;
        }
            //will begin a prompt for moving object to a specified place on the grid
        case 'a': {
            moveObject(object, arduino, cells, calibration, frame, state);
            break;
        }
            //starts servos if stopped
        case 'g': {
            startServo(arduino, state);
            break;
        }
            //stops servos if running
        case 'h': {
            stopServo(arduino, state);
        }
            //exit
        case 'q': {
            exit(arduino, frame);
            break;
        }
        default: {
            state.actionCommited = false;
            break;
        }
    }
}

void cameraCalibration(const Object &object, Arduino &arduino, Calibration &calibration, const cv::Mat &frame, State &state) {
    calibration.calibrateMeasurement(frame);

    double width = object.width / calibration.pixelPerInch;

    cout << "calibrated" << "\n";

    cout << "width: " << width << " inches" << "\n";

    cout << "pixels per inch: " << calibration.pixelPerInch << "\n";

    const double cameraWidth = frame.cols / calibration.pixelPerInch;
    const double cameraHeight = frame.rows / calibration.pixelPerInch;
    string message = format("<{}, {:.2f}, {:.2f}, {:d}, {:d}>", arduino.calibrationCode, cameraWidth, cameraHeight,
                            gridColumns, gridRows);
    arduino.arduinoSerialOutput(message);

    state.isCalibrated = true;
    state.actionCommited = true;
}

void measureObject(const Object &object, const Calibration &calibration, const cv::Mat &frame, State &state) {
    if (state.isCalibrated) {
        const double widthInches = object.width / calibration.pixelPerInch;
        const double heightInches = object.height / calibration.pixelPerInch;

        cout << "width: " << widthInches << " inches" << "\n";
        cout << "height: " << heightInches << " inches" << "\n";
    } else {
        cout << "ERROR NOT CALIBRATED" << "\n";
    }

    state.actionCommited = true;
}

void locateObject(Object &object, const vector<Cell> &cells, State &state) {
    if (object.objectFound) {
        object.checkCell(cells);


        //using a boolean to check if object exists in a cell
        if (!object.locationFound) {
            cout << "ERROR OBJECT NOT IN A SINGLE CELL, REPOSITION IT" << "\n";
        } else {
            cout << "current location is " << object.currentLocation.x << " " << object.currentLocation.y << "\n";
        }
    } else {
        cout << "ERROR OBJECT NOT FOUND" << "\n";
    }

    state.actionCommited = true;
}

void calibrateArm(Arduino &arduino, State &state) {
    if (!state.armCalibrating) {
        cout << "Calibrate arm" << "\n";
        state.armCalibrating = true;
    } else {
        cout << "Calibration complete" << "\n";
        state.armCalibrating = false;
        state.isArmCalibrated = true;
    }

    arduino.arduinoSerialOutput(arduino.armCalibrationCode);
}

void moveObject(Object &object, Arduino &arduino, const vector<Cell> &cells, Calibration calibration,
                const cv::Mat &frame, State &state) {
    if (object.objectFound && object.locationFound && state.isArmCalibrated && state.isCalibrated) {
        arduino.sendMovement(object, cells, calibration, frame);
    } else {
        if (!object.objectFound) {
            cout << "ERROR OBJECT NOT FOUND" << "\n";
        }
        if (!object.locationFound) {
            cout << "ERROR LOCATION NOT FOUND .. PRESS 'l' TO LOCATE" << "\n";
        }
        if (!state.isArmCalibrated) {
            cout << "ERROR ARM NOT CALIBRATED" << "\n";
        }
        if (!state.isCalibrated) {
            cout << "ERROR NOT CALIBRATED" << "\n";
        }
    }
    state.actionCommited = true;
}

void startServo(Arduino &arduino, State &state) {
    arduino.arduinoSerialOutput(arduino.startServosCode);
    state.isServoStopped = false;
    state.actionCommited = true;
}

void stopServo(Arduino &arduino, State &state) {
    arduino.arduinoSerialOutput(arduino.stopServosCode);
    state.isServoStopped = true;
    state.actionCommited = true;
}

void exit(Arduino &arduino, const cv::Mat &frame) {
    cout << "capturing" << "\n";
    arduino.arduinoSerialOutput(arduino.stopServosCode);

    //saves image
    cv::imwrite("capture.JPG", frame);
}
