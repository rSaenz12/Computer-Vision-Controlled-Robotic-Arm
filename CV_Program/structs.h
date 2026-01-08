//********************************************
// Author: Russell Saenz
// File: structs.h
// Description: Declares Cell, Object, and Calibration structs. Also declares gridRows and gridColumns
//********************************************

#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

constexpr int gridRows = 3;
constexpr int gridColumns = 3;

struct Cell {
    int topLeftX = 0;
    int topLeftY = 0;

    int width = 0;
    int height = 0;

    cv::Point2i coordinate = cv::Point2i(0, 0);
    cv::Point2i centerCoordinates = cv::Point2i(0, 0);
};

struct Object {
    bool objectFound = false;

    bool locationFound = false;

    int width = 0;
    int height = 0;

    cv::Point2i center = cv::Point2i(0, 0);

    cv::Point2i topLeft = cv::Point2i(0, 0);
    cv::Point2i bottomRight = cv::Point2i(0, 0);
    cv::Point2i currentLocation = cv::Point2i(0, 0);

    void padRectangle(int pixels);

    void checkCell(const std::vector<Cell> &cells);
};

struct Calibration {
    double pixelPerInch = 0;
    double pixelPerCentimeter = 0;

    void calibrateMeasurement(cv::Mat frame);
};