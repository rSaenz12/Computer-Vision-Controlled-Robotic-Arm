//********************************************
// Author: Russell Saenz
// File: computerVision.h
// Description: Function signatures for colorDistance and findObject
//********************************************

#pragma once

#include "structs.h"

double colorDistance(const cv::Vec3b &currentColor, const cv::Vec3b &existingColor) ;

void findObject(cv::Mat &frame, Object &object);
