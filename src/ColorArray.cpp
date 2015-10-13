//
//  ColorArray.cpp
//  findColorArray
//
//  Created by Florian Born on 02/10/15.
//
//

#include "ColorArray.hpp"


ColorArray::ColorArray(){
    
};

void ColorArray::setup(float x, float y,float w,float h, int videoW, int videoH){
    size = ofVec2f(w,h);
    step = size.x/resolution;
    origin = ofVec2f(x,y);
    videoWidth = videoW;
    for (float xP = 0; xP < resolution; xP += 1) {
        for (float yP = 0; yP < resolution; yP += 1) {
            colorPos.push_back(ofVec2f(ofMap(xP, 0, resolution, 0,1),ofMap(yP, 0, resolution, 0, 1)));
            colors.push_back(ofColor(0,0,0));
        }
    }
};

void ColorArray::setResolution(int n){
    resolution = n;
}

ofVec2f ColorArray::getOrigin(){
    return origin;
}

void ColorArray::draw(){
    int c = 0;
    for(std::vector<ofVec2f>::iterator it = interpolatedMirrors.begin(); it != interpolatedMirrors.end(); ++it) {
        int x = (*it).x;
        int y = (*it).y;
        ofColor color = colors.at(c);
        //        ofCircle(x, y, 5);
        ofSetColor(color.r, color.g, color.b);
        ofRect(x, y, step, step);
        c++;
    }
}

void ColorArray::videoUpdate(unsigned char * pixels){
    videoImage = pixels;
}

void ColorArray::calculateColorArray(){
    interpolatedMirrors.clear();
    for(std::vector<ofVec2f>::iterator it = colorPos.begin(); it != colorPos.end(); ++it){
        interpolatedMirrors.push_back(
                                      interpolateVec(ofVec2f( (*it).x, (*it).y) ));
    }
    
    int c = 0;
    for(std::vector<ofVec2f>::iterator it = interpolatedMirrors.begin(); it != interpolatedMirrors.end(); ++it) {
        int x = (*it).x;
        int y = (*it).y;
        ofColor newColor = findSurroundingColors(videoImage,x,y,step);
        colors.at(c) = newColor;
        c++;
    }
}

ofColor ColorArray::findSurroundingColors(unsigned char * image, int x, int y, float dia){
    int r = 0;
    int g = 0;
    int b = 0;
    int counter = 0;
    for (int i = 0; i < dia; i++) {
        for (int j = 0; j < dia; j++) {
            int nX = abs(x+i);
            int nY = abs(y+i);
            int arrayPos = abs((nX+nY*videoWidth) * 3);
            ofColor c = ofColor((int)image[arrayPos],(int)image[arrayPos+1],(int)image[arrayPos+2]);
            r += c.r;
            g += c.g;
            b += c.b;
            counter++;
        }
    }
    r /= counter;
    g /= counter;
    b /= counter;
    return ofColor(r,g,b);
}

void ColorArray::updatePosition(int x, int y){
    origin = ofVec2f(x,y);
    colorPos.clear();
    for (float xPos = 0; xPos < size.x; xPos += step) {
        for (float yPos = 0; yPos < size.y; yPos += step) {
            colorPos.push_back(ofVec2f(xPos+origin.x,yPos+origin.y));
        }
    }
}
ofVec2f ColorArray::interpolateVec(ofVec2f v){
    
    // remapping
    float topX = ofMap(v.x, 0, 1, TL.x, TR.x);
    float bottomX = ofMap(v.x, 0, 1, BL.x, BR.x);
    float leftY = ofMap(v.y, 0,1, TL.y, BL.y);
    float rightY = ofMap(v.y, 0, 1, TR.y, BR.y);
    // interpolation
    float newX = ofLerp(topX, bottomX,v.y);
    float newY = ofLerp(leftY, rightY, v.x);
    //   cout << "(" << newX << ") (" << newY << ")" << endl;
    return ofVec2f(newX,newY);
    
    
}
void ColorArray::updateMarker(ofVec2f tlV, ofVec2f trV, ofVec2f blV, ofVec2f brV){
    TL = tlV;
    TR = trV;
    BL = blV;
    BR = brV;
    
    
}

