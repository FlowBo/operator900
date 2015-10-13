#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"


class videoProcessor{
public:
    videoProcessor();
    void setup(ofVideoGrabber * video);
    void update();
    void draw();
    ofVideoGrabber * videoInput;
    
};