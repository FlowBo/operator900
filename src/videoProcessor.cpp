//
//  videoProcessor.cpp
//  findColorArray
//
//  Created by Florian Born on 13/10/15.
//
//

#include "videoProcessor.hpp"


videoProcessor::videoProcessor(){};

void videoProcessor::setup(ofVideoGrabber * video){
    string boardName = "boardConfiguration.yml";
    videoInput = video;
    aruco.setup("intrinsics.int", videoInput->getWidth(), videoInput->getHeight(), boardName);
    aruco.getBoardImage(board.getPixelsRef());
    board.update();
}

void videoProcessor::update(){
    bool bNewFrame = false;
    videoInput->update();
    bNewFrame = videoInput->isFrameNew();
    if (bNewFrame){ aruco.detectBoards(videoInput->getPixelsRef() );}

    if(aruco.getNumMarkers() == 4){
        std::vector<aruco::Marker> m = aruco.getMarkers();
        convertedMarker.clear();
        for (std::vector<aruco::Marker>::iterator it = m.begin(); it != m.end();++it){
            convertedMarker.push_back(ofVec2f((*it).getCenter().x,(*it).getCenter().y ));
        }
        updateMarker(convertedMarker);
    }
    
    unsigned char * pixels = videoInput->getPixels();
}

void videoProcessor::draw(){
    videoInput->draw(0, 0);
    for(int i=0;i<aruco.getNumMarkers();i++){
        aruco.begin(i);
        drawMarker(0.15,ofColor::white);
        aruco.end();
    }
    ofDrawBitmapString("topLeft" + ofToString(TL) , TL.x , TL.y);
    ofDrawBitmapString("topRight"+ ofToString(TR), TR.x , TR.y);
    ofDrawBitmapString("bottomLeft"+ ofToString(BL), BL.x , BL.y);
    ofDrawBitmapString("bottomRight"+ ofToString(BR), BR.x , BR.y);
}

void videoProcessor::updateMarker(std::vector<ofVec2f> m){
    
    //TODO
    //Replace calculation with marker ID
    float dist = 99999999.0f;
    std::vector<ofVec2f>::iterator toDelete;
    for (std::vector<ofVec2f>::iterator it = m.begin(); it != m.end();++it){
        float pointDist = (*it).distance(ofVec2f(0,0));
        if(pointDist < dist )
        {
            dist = pointDist;
            toDelete = it;
            TL = (*it);
        }
    }
    m.erase(toDelete);
    dist = 0;
    for (std::vector<ofVec2f>::iterator it = m.begin(); it != m.end();++it){
        
        float pointDist = (*it).distance(ofVec2f(0,0));
        if(pointDist > dist )
        {
            dist = pointDist;
            toDelete = it;
            BR = (*it);
        }
    }
    m.erase(toDelete);
    if(m.at(0).x  < m.at(1).x ){
        BL = m.at(0);
        TR = m.at(1);
    }else{
        BL = m.at(1);
        TR = m.at(0);
    }
}

void videoProcessor::drawMarker(float size, const ofColor & color){
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

