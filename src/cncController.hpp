#pragma once

#include "ofMain.h"
#include "ofxSerial.h"

class cncController{
public:
    //functions
    cncController();
    void setup(int boudrate);
//    void update();
    void sendGCode(std::string gcode);
    //variables
    ofx::IO::SerialDevice device;
};