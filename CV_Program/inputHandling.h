//********************************************
// Author: Russell Saenz
// File: inputHandling.h
// Description: Declares functions that are called via inputs.
//      State of program struct declared here.
//********************************************

#pragma once

#include "structs.h"

#include "arduino.h"

struct State {
    //tracks wether measurements has been calibrated
    bool isCalibrated = false;

    //makse sure cant use arm til calibrated
    bool isArmCalibrated = false;

    //when true it turns grid and object detection off is displays a center line for lining the arm up.
    bool armCalibrating = false;

    //for starting or stopping servos
    bool isServoStopped = false;

    //just for not repeat printing the menu
    bool actionCommited = true;
};



void cameraCalibration(const Object &object, Arduino &arduino, Calibration &calibration, const cv::Mat &frame, State &state);

void measureObject(const Object &object, const Calibration &calibration, const cv::Mat &frame, State &state);

void locateObject(Object &object, const std::vector<Cell> &cells, State &state);

void calibrateArm(Arduino &arduino, State &state);

void moveObject(Object &object, Arduino &arduino, const std::vector<Cell> &cells, Calibration calibration,
                const cv::Mat &frame, State &state);

void startServo(Arduino &arduino, State &state);

void stopServo(Arduino &arduino, State &state);

void exit(Arduino &arduino, const cv::Mat &frame);

void processInput(int input, Object &object, Arduino &arduino, Calibration &calibration, const std::vector<Cell> &cells,
                  const cv::Mat &frame, State &state);

