//
//  videoController.cpp
//  findColorArray
//
//  Created by Florian Born on 12/10/15.
//
//

#include "videoController.hpp"

videoController::videoController(){};

void videoController::setup(int cameraWidth,int cameraHeight, int framerate){
    string boardName = "boardConfiguration.yml";

    camWidth 		= cameraWidth;	// try to grab at this size.
    camHeight 		= cameraHeight;
    video.setDesiredFrameRate(framerate);
    video.setVerbose(true);
    video.initGrabber( camWidth ,camHeight);
    cam = &video;
    ofSetVerticalSync(true);
    aruco.setup("intrinsics.int", cam->getWidth(), cam->getHeight(), boardName);
    aruco.getBoardImage(board.getPixelsRef());
    board.update();
    step = 10;

}
void videoController::update(){

        bool bNewFrame = false;
        video.update();
        bNewFrame = video.isFrameNew();
        if (bNewFrame){
            aruco.detectBoards(cam->getPixelsRef());
        }
        if(aruco.getNumMarkers() == 4){
            std::vector<aruco::Marker> m = aruco.getMarkers();
            convertedMarker.clear();
            for (std::vector<aruco::Marker>::iterator it = m.begin(); it != m.end();++it){
                convertedMarker.push_back(ofVec2f((*it).getCenter().x,(*it).getCenter().y ));
            }
            updateMarker(convertedMarker);
        }
        int totalPixels = camWidth*camHeight*3;
        pixels = video.getPixels();
}

void videoController::calculateColorArray(){
    interpolatedMirrors.clear();
    for(std::vector<ofVec2f>::iterator it = colorPos.begin(); it != colorPos.end(); ++it){
        interpolatedMirrors.push_back(interpolateVec(ofVec2f( (*it).x, (*it).y) ));
    }
    
    int c = 0;
    for(std::vector<ofVec2f>::iterator it = interpolatedMirrors.begin(); it != interpolatedMirrors.end(); ++it) {
        int x = (*it).x;
        int y = (*it).y;
        ofColor newColor = findSurroundingColors(pixels,x,y,step);
        trueColors.at(c) = newColor;
        c++;
    }
}
ofVec2f videoController::interpolateVec(ofVec2f v){
    
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
ofColor videoController::findSurroundingColors(unsigned char * image, int x, int y, float dia){
    int r = 0;
    int g = 0;
    int b = 0;
    int counter = 0;
    for (int i = 0; i < dia; i++) {
        for (int j = 0; j < dia; j++) {
            int nX = abs(x+i);
            int nY = abs(y+i);
            int arrayPos = abs((nX+nY*camWidth) * 3);
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
void videoController::drawMarker(float size, const ofColor & color){
    ofDrawAxis(size);
    ofPushMatrix();
    // move up from the center by size*.5
    // to draw a box centered at that point
    ofTranslate(0,size*0.5,0);
    ofFill();
    ofSetColor(color,50);
    ofBox(size);
    ofNoFill();
    ofSetColor(color);
    ofBox(size);
    ofPopMatrix();
}


std::vector<ofColor> videoController::getTrueColors(){
    return trueColors;
}