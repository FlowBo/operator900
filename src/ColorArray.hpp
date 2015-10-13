#pragma once

#include "ofMain.h"

class ColorArray{
public:
    
    std::vector<ofColor> colors;
    std::vector<ofVec2f> colorPos;
    ofVec2f size;
    ofVec2f origin;
    float step;
    int resolution;
    int videoWidth;
    int videoHeight;
    ColorArray();
    void setup(float x, float y,float w,float h,int videoWidth, int videoHeight);
    ofVec2f getOrigin();
    void draw();
    void setResolution(int n);
    unsigned char * videoImage;
    void videoUpdate(unsigned char * pixels);
    ofColor findSurroundingColors(unsigned char * image, int x, int y, float radius);
    void calculateColorArray();
    void updatePosition(int x, int y);
    ofVec2f interpolateVec(ofVec2f v);
    ofVec2f TL;
    ofVec2f TR;
    ofVec2f BL;
    ofVec2f BR;
    void updateMarker(ofVec2f tl, ofVec2f tr, ofVec2f bl, ofVec2f br);
    std::vector<ofVec2f> interpolatedMirrors;
};