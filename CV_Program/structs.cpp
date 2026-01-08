//********************************************
// Author: Russell Saenz
// File: structs.cpp
// Description: Adds width to rectangle for drawing offset.
//      Checks if the object is within a single cell.
//********************************************

#include "structs.h"

//adds padding of x pixels to edges
void Object::padRectangle(const int pixels) {
    topLeft.x -= pixels;
    topLeft.y -= pixels;

    bottomRight.x += pixels;
    bottomRight.y += pixels;
}

//basically the check cell above but void
void Object::checkCell(const std::vector<Cell> &cells) {
    for (const Cell cell: cells) {
        if (topLeft.x >= cell.topLeftX && topLeft.x + width <= cell.topLeftX + cell.width) {
            if (topLeft.y >= cell.topLeftY && topLeft.y + height <= cell.topLeftY + cell.height) {
                locationFound = true;
                currentLocation = cell.coordinate;
                return;
            }
        }
    }
    //signal poor placement in grid
    locationFound = false;
}
