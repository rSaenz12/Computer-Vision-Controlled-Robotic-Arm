//********************************************
// Author: Russell Saenz
// File: inputHandling.h
// Description: Declares Arduino struct
//********************************************

#pragma once

#include <iostream>
#include <fstream>

#include "structs.h"

struct Arduino {
    using string = std::string;

    std::fstream arduino;

    //communication codes
    const string calibrationCode = "1000";
    const string armCalibrationCode = "900";
    const string stopServosCode = "999";
    const string startServosCode = "998";
    const string movementCode = "700";
    const string measurementCode = "500";
    const string locationCode = "350";

    Arduino() {
        std::cout << "Accessing Arduino" << "\n";

        //"/dev/ttyACM0" is the default Serial port pass through, could change from system to system
        arduino = std::fstream("/dev/ttyACM0", std::ios::out | std::ios::binary);
    }

    Arduino(const Arduino&) = delete;

    Arduino& operator=(const Arduino&) = delete;

    ~Arduino() {
        arduino.close();
    }

    void arduinoSerialOutput(const std::string &data);

    void sendMovement(Object &object, const std::vector<Cell> &cells, Calibration calibration, const cv::Mat &frame);
};

