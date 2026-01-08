//********************************************
// Author: Russell Saenz
// File: computerVision.cpp
// Description: Finds the object in view using the color difference between the background (white) and the object.
//      Calibrates pixels per inch by measruing a ruler laid flat in the center of the camera.
//********************************************

#include "computerVision.h"

//difference between colors rgb values, used in conjunction with the color buffer to detect very different colors
double colorDistance(const cv::Vec3b &currentColor, const cv::Vec3b &existingColor) {
    const double distanceRed = static_cast<double>(currentColor[0]) - existingColor[0];
    const double distanceGreen = static_cast<double>(currentColor[1]) - existingColor[1];
    const double distanceBlue = static_cast<double>(currentColor[2]) - existingColor[2];
    return sqrt(distanceRed * distanceRed + distanceGreen * distanceGreen + distanceBlue * distanceBlue);
}

void Calibration::calibrateMeasurement(cv::Mat frame) {
    if (frame.empty()) return;

    //object declared here becuase its only for getting the pixels per inch
    Object object;
    findObject(frame, object);


    pixelPerInch = static_cast<double>(object.width) / 12.0;
    pixelPerCentimeter = static_cast<double>(object.width) / 30.48;
}

void findObject(cv::Mat &frame, Object &object) {
    if (frame.empty()) return;

    int minY;
    int minX;
    int maxY;
    int maxX;
    const double colorBuffer = 50.0;

    bool found = false;
    if (object.locationFound) {
        int cellHeiht = frame.rows / gridRows;
        int cellWidth = frame.cols / gridColumns;

        minY = object.currentLocation.y * cellHeiht;
        minX = object.currentLocation.x * cellWidth;
        maxY = minY + cellHeiht;
        maxX = minX + cellWidth;
    } else {
        minY = 0;
        minX = 0;
        maxY = frame.rows;
        maxX = frame.cols;
    }

    int top = minY;
    int bottom = maxY - 1;
    int leftSide = minX;
    int rightSide = maxX - 1;

    //find top
    found = false;
    for (int y = minY + 1; y < maxY && !found; y++) {
        for (int x = minX; x < maxX; x++) {
            if (colorDistance(frame.at<cv::Vec3b>(y, x), frame.at<cv::Vec3b>(y - 1, x)) > colorBuffer) {
                top = y;
                found = true;
                break;
            }
        }
    }

    //find bottom
    found = false;
    for (int y = maxY - 2; y >= minY && !found; y--) {
        for (int x = minX; x < maxX; x++) {
            if (colorDistance(frame.at<cv::Vec3b>(y, x), frame.at<cv::Vec3b>(y + 1, x)) > colorBuffer) {
                bottom = y;
                found = true;
                break;
            }
        }
    }

    // find left
    found = false;
    for (int x = minX + 1; x < maxX && !found; x++) {
        for (int y = minY; y < maxY; y++) {
            if (colorDistance(frame.at<cv::Vec3b>(y, x), frame.at<cv::Vec3b>(y, x - 1)) > colorBuffer) {
                leftSide = x;
                found = true;
                break;
            }
        }
    }

    // find right
    found = false;
    for (int x = maxX - 2; x >= minX && !found; x--) {
        for (int y = minY; y < maxY; y++) {
            if (colorDistance(frame.at<cv::Vec3b>(y, x), frame.at<cv::Vec3b>(y, x + 1)) > colorBuffer) {
                rightSide = x;
                found = true;
                break;
            }
        }
    }


    if (found) {
        object.objectFound = true;
    } else {
        object.objectFound = false;
    }

    object.width = rightSide - leftSide;
    object.height = bottom - top;

    const int centerX = leftSide + (object.width / 2);
    const int centerY = top + (object.height / 2);

    object.center = cv::Point2i(centerX, centerY);
    object.topLeft = cv::Point2i(leftSide, top);
    object.bottomRight = cv::Point2i(rightSide, bottom);
}
