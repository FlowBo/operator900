#pragma once

#include "ofMain.h"
#include "ColorArray.hpp"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxSerial.h"
#include "ofxAruco.h"
#include "ofxCv.h"
#include "ofBitmapFont.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //GUI
    ofxPanel gui;
    ofxIntSlider resolution;
    ofxFloatSlider size;
    
    //Video
    int camWidth;
    int camHeight;
    ofVideoGrabber video;
    
    //Color tacking
    ColorArray cArray;
    
    //serial
    void serialSetup();
    void serialUpdate();
    ofx::IO::SerialDevice device;
    char charKey;
    
    //Marker
    ofxAruco aruco;
    ofImage board;
    ofBaseVideoDraws * cam;
    std::vector<ofVec2f> mirrorVec;
    std::vector<ofVec2f> mirrorVecInterpolated;
    std::vector<ofVec2f> convertedMarker;
    void updateMarker(std::vector<ofVec2f> m);
    ofVec2f topLeft;
    ofVec2f topRight;
    ofVec2f bottomLeft;
    ofVec2f bottomRight;
    
};

