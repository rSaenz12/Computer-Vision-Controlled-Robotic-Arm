//********************************************
// Author: Russell Saenz
// File: main.cpp
// Description: Main driver file for computer vision /  robot arm.
//      Initializes objects, program loop, handles camera and gui (grid lines)
//********************************************

#include "structs.h"
#include "computerVision.h"
#include "arduino.h"
#include "inputHandling.h"

using namespace std;

void menu(const State &state);

int main() {
    //0 is the default indexing for the camera, might change per system
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        cout << "Error: Could not open webcam.\n";
        return -1;
    }

    Arduino arduino;
    if (!arduino.arduino.is_open()) {
        cout << "Failed to open serial port!\n";
        return 1;
    }
    cout << "Port opened. Waiting for Arduino to initialize...\n";
    usleep(2000000);

    //camera settings for display
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_CONTRAST, 50);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 800);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 800);
    cap.set(cv::CAP_PROP_BUFFERSIZE, 3);

    freopen("/dev/null", "w", stderr);

    cv::Mat frame;
    cap >> frame;

    const int stepX = frame.cols / gridColumns;
    const int stepY = frame.rows / gridRows;

    //setting cell's attributes
    vector<Cell> cells;
    for (int i = 0; i < gridRows; i++) {
        for (int j = 0; j < gridColumns; j++) {
            Cell cell;
            cell.topLeftX = stepX * i;
            cell.topLeftY = stepY * j;
            cell.width = stepX;
            cell.height = stepY;
            cell.coordinate = cv::Point2i(i, j);
            cell.centerCoordinates = cv::Point2i(cell.topLeftX + (stepX / 2), cell.topLeftY + (stepY / 2));
            cells.push_back(cell);
        }
    }

    //ui variables
    const cv::Scalar gridColor(0, 255, 0);
    const cv::Scalar rectangleColor(0, 0, 255);
    const int thickness = 1;

    //objects
    Object object;
    Calibration calibrate;
    State state;

    //main loop
    while (true) {
        if (state.actionCommited) {
            menu(state);
            state.actionCommited = false;
        }

        cap >> frame;
        if (frame.empty()) {
            cout << "Error: Blank frame grabbed.\n";
            break;
        }

        // if not calibrating then the grid is displayed
        if (!state.armCalibrating) {
            findObject(frame, object);
            object.padRectangle(5);

            //rectangle outlines object
            cv::rectangle(frame, object.topLeft, object.bottomRight,
                          rectangleColor, thickness);

            //grid
            for (int i = 1; i < gridColumns; i++) {
                cv::line(frame, cv::Point(i * stepX, 0),
                         cv::Point(i * stepX, frame.rows), gridColor, thickness);
            }
            for (int i = 1; i < gridRows; i++) {
                cv::line(frame, cv::Point(0, i * stepY),
                         cv::Point(frame.cols, i * stepY), gridColor, thickness);
            }
        } else if (state.armCalibrating) {
            //center line to help allign arm
            cv::line(frame, cv::Point(frame.cols / 2, 0),
                     cv::Point(frame.cols / 2, frame.rows),
                     rectangleColor, thickness * 2);
        }

        cv::imshow("Display", frame);

        int input = 0;
        input = cv::waitKey(10);

        //EXITS PROGRAM
        if (input == 'q') {
            exit(arduino, frame);
            break;
        }

        //anything other than q
        if (input > 0) {
            processInput(input, object, arduino, calibrate, cells, frame, state);
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}

void menu(const State &state) {
    cout << "\n";
    cout << "Press 'q' to quit\n";
    cout << "Press 'c' to calibrate\n";

    if (!state.armCalibrating) {
        cout << "Press 'f' to calibrate arm\n";
    } else {
        cout << "Press 'f' to end calibration of arm\n";
    }

    if (state.isCalibrated) {
        cout << "Press 'm' to measure\n";
        cout << "Press 'l' to locate\n";

        if (!state.isServoStopped) {
            if (state.isArmCalibrated) {
                cout << "Press 'a' to move object\n";
            }
            cout << "Press 'h' to stop servos\n";
        } else {
            cout << "Press 'g' to start servos\n";
        }
    }
    cout << "\n";
}
