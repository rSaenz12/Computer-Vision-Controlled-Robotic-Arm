cmake_minimum_required(VERSION 3.16)
project(CamTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

#Dependencies
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(V4L2 REQUIRED libv4l2)

#Define Executable
add_executable(CamTest main.cpp
        structs.h
        structs.cpp
        computerVision.cpp
        computerVision.h
        arduino.cpp
        arduino.h
        inputHandling.cpp
        inputHandling.h
)

#Combined Link Libraries (Using Keywords)
target_link_libraries(CamTest PRIVATE
        ${OpenCV_LIBS}
        ${V4L2_LIBRARIES}
)

#Include Directories
target_include_directories(CamTest PRIVATE
        ${V4L2_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
)