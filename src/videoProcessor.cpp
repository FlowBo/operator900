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
    videoInput = video;
}

void videoProcessor::update(){

}

void videoProcessor::draw(){
    videoInput->draw(0, 0);
}