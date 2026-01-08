//********************************************
// Author: Russell Saenz
// File: arduino.cpp
// Description: Handles communication between program and arduino via Serial Monitor.
//********************************************

#include "arduino.h"

using namespace std;

//for sending data to the arduino
void Arduino::arduinoSerialOutput(const string &data) {
    if (arduino.is_open()) {
        arduino << data << "\n";

        arduino.flush();
        cout << "Data sent!" << "\n";
    } else {
        cout << "Failed to open serial port!" << "\n";
    }
}

void Arduino::sendMovement(Object &object, const vector<Cell> &cells, Calibration calibration, const cv::Mat &frame) {
    double objectCenterX = static_cast<double>(object.center.x) / calibration.pixelPerInch;

    double objectCenterY = static_cast<double>(frame.rows - object.center.y) / calibration.pixelPerInch;

    int targetX = 0;
    int targetY = 0;


    cout << "Object is in cell " << object.currentLocation.x << "," << object.currentLocation.y << "\n";
    cout << "Enter X cell coordinate of target ";
    cin >> targetX;
    cout << "Enter Y cell coordinate of target ";
    cin >> targetY;

    if (targetX >= gridColumns || targetX < 0 || targetY >= gridRows || targetY < 0) {
        cout << "OUT OF BOUNDS" << "\n";
        return;
    }

    double distanceFromLeft = 0.0;
    double distanceFromBottom = 0.0;

    bool cellFound = false;
    for (const Cell cell: cells) {
        if (cell.coordinate.x == targetX && cell.coordinate.y == targetY) {
            distanceFromLeft = static_cast<double>(cell.centerCoordinates.x) / calibration.pixelPerInch;
            distanceFromBottom = static_cast<double>(frame.rows - cell.centerCoordinates.y) / calibration.pixelPerInch;
            cellFound = true;
            break;
        }
    }

    if (!cellFound) {
        cout<<"ERROR CELL NOT FOUND"<<"\n";
        return;
    }

    cout << "objCX" << objectCenterX << "\n";
    cout << "objCY" << objectCenterY << "\n";
    cout << "DistLeft" << distanceFromLeft << "\n";
    cout << "DistBottom" << distanceFromBottom << "\n";

    object.currentLocation.x = targetX;
    object.currentLocation.y = targetY;
    object.locationFound = false;

    //c++20
    string message = format("<{}, {:.2f}, {:.2f}, {:.2f}, {:.2f}>", movementCode, objectCenterX, objectCenterY,
                            distanceFromLeft, distanceFromBottom);

    arduinoSerialOutput(message);
}
