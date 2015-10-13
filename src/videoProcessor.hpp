#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "ofxAruco.h"
#include "ofxCv.h"

class videoProcessor{
public:
    videoProcessor();
    void setup(ofVideoGrabber * video);
    void update();
    void draw();
    void drawMarker(float size, const ofColor & color);
    ofVideoGrabber * videoInput;
    
    //True colors
    std::vector<ofColor> trueColors;
    std::vector<ofVec2f> trueColorsPos;
    //Marker
    ofxAruco aruco;
    ofImage board;
    ofBaseVideoDraws * cam;
    std::vector<ofVec2f> mirrorVec;
    std::vector<ofVec2f> mirrorVecInterpolated;
    std::vector<ofVec2f> convertedMarker;
    void updateMarker(std::vector<ofVec2f> m);
    ofVec2f TL;
    ofVec2f TR;
    ofVec2f BL;
    ofVec2f BR;
    
    
};