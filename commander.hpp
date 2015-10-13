#pragma once

#include "ofMain.h"
#include "ColorArray.hpp"
#include "cncController.hpp"

#include "ofBitmapFont.h"

class commander{
public:
    commander();
    void setup();
    void update();
    void draw();
    ColorArray colorArray;
};