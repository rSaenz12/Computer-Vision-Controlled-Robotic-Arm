***Computer Vision Robotic Arm Control***

AUTHOR: Russell Saenz
Contact: rsaenz12@islander.tamucc.edu

Languages: C++

Libraries:
    CV Program
        -OpenCV
        -libv4l2
    Arduino Program
        -Servo
        -LiquidCrystal_I2C

Description:
    This program is a computer vision program that employs an overhead camera to locate an object on a virtual grid.
    After an object is located, the object can be virtually measured, or moved via a robotic arm controlled by an Arduino.
    The Arduino is sent coordinates of the objects location and target location from the program.

    The Arduino receives communication codes to signal what task it will execute. Data sent in a string through the Serial Monitor
    allow the Arduino to make real time calculations for movement. Utilizing trigonometry the Arduino calculates the angle of
    The base, shoulder and elbow joints.

Files in Directory:
    /CV Program (developed in CLion)
        -CMakeLists.txt
        -main.cpp
        -arduino.h/cpp
        -computerVision.h/cpp
        -inputHandling.h/cpp
        -structs.h/cpp


    /Arduino Program (developed on PlatformIO)
        -main.cpp
        -armMovement.h/cpp
        -LCDTools.h/cpp

    /Images
        -images of the project

Requirements:
    Software
        -WSL2(Ubuntu)
            -OpenCV4
            -libv4l2
            -usbipd (for usb passthrough from windows to ubuntu)

    Hardware
        -Arduino Uno
            -LCD (for debugging)
        -3 joint Robotic Arm (magnet for grabbing objects)
        -USB Camera (overhead mounted)
        -12 Inch ruler (Dark color works best)

Key Commands:
     c : Calibrate camera using ruler
     f : Start/stop arm calibration mode
     m : Measure detected object
     l : Locate object in grid cell
     a : Move object to target cell
     g : Start servos
     h : Stop servos
     q : Quit and capture screenshot


Object Tracking




