#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxAruco.h"
#include "ofxCv.h"
#include "ofBitmapFont.h"


class videoController{
public:
    videoController();
    void setup(int cameraWidth,int cameraHeight, int famerate);
    void drawMarker(float size, const ofColor & color);
    void update();
    void calculateColorArray();
    ofVec2f interpolateVec(ofVec2f v);
    findSurroundingColors(unsigned char * image, int x, int y, float dia);
    std::vector<ofColor> getTrueColors();
    int camWidth;
    int camHeight;
    ofVideoGrabber video;
    unsigned char * pixels;
    
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
    std::vector<ofColor> trueColors;
    std::vector<ofVec2f> colorPos;
    std::vector<ofVec2f> interpolatedMirrors;
    int step;
};