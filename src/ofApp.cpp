#include "ofApp.h"
void drawMarker(float size, const ofColor & color){
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
//--------------------------------------------------------------
void ofApp::setup(){
    string boardName = "boardConfiguration.yml";
    ofSetFrameRate(60);
    //gui
    gui.setup();
    gui.add(resolution.setup( "resolution", 30, 10, 90 ));
    gui.add(size.setup( "size", 400, 200, 700 ));
    //Serial
    serialSetup();
    //video
    camWidth 		= 1280;	// try to grab at this size.
    camHeight 		= 720;
    video.setDesiredFrameRate(60);
    video.setVerbose(true);
    video.initGrabber( camWidth ,camHeight);
    
    videoController.setup(&video);
    cam = &video;
    ofSetVerticalSync(true);
    aruco.setup("intrinsics.int", cam->getWidth(), cam->getHeight(), boardName);
    aruco.getBoardImage(board.getPixelsRef());
    board.update();
    
    videoController.setup(&video);
    
    
    //Color tracking
    cArray.setResolution(resolution);
    cArray.setup(50, 50, size, size, camWidth,camHeight);
    for (int x = 0; x < 30 ; x++) {
        for (int y = 0; y < 30 ; y++) {
            mirrorVec.push_back(ofVec2f(ofMap(x, 0, 30, 0, 1),ofMap(y, 0, 30, 0, 1)));
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //Update video
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
        cArray.updateMarker(topLeft,topRight,bottomLeft,bottomRight);
    }
    int totalPixels = camWidth*camHeight*3;
    unsigned char * pixels = video.getPixels();
    //update Color tracking
    cArray.videoUpdate(pixels);
    cArray.calculateColorArray();
    
    serialUpdate();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);
//    video.draw(0, 0);
    videoController.draw();
    ofFill();
    cArray.draw();
    for(int i=0;i<aruco.getNumMarkers();i++){
        aruco.begin(i);
        drawMarker(0.15,ofColor::white);
        aruco.end();
    }
    
    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 700);
    ofDrawBitmapString("topLeft" + ofToString(topLeft) , topLeft.x , topLeft.y);
    ofDrawBitmapString("topRight"+ ofToString(topRight), topRight.x , topRight.y);
    ofDrawBitmapString("bottomLeft"+ ofToString(bottomLeft), bottomLeft.x , bottomLeft.y);
    ofDrawBitmapString("bottomRight"+ ofToString(bottomRight), bottomRight.x , bottomRight.y);
    gui.draw();
    
}


void ofApp::updateMarker(std::vector<ofVec2f> m){
    float dist = 99999999.0f;
    std::vector<ofVec2f>::iterator toDelete;
    for (std::vector<ofVec2f>::iterator it = m.begin(); it != m.end();++it){
        float pointDist = (*it).distance(ofVec2f(0,0));
        if(pointDist < dist )
        {
            dist = pointDist;
            toDelete = it;
            topLeft = (*it);
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
            bottomRight = (*it);
        }
    }
    m.erase(toDelete);
    if(m.at(0).x  < m.at(1).x ){
        bottomLeft = m.at(0);
        topRight = m.at(1);
    }else{
        bottomLeft = m.at(1);
        topRight = m.at(0);
    }
}

void ofApp::serialSetup(){
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
    ofLogNotice("ofApp::setup") << "Connected Devices: ";
    
    for (std::size_t i = 0; i < devicesInfo.size(); ++i)
    {
        ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
    }
    
    if (!devicesInfo.empty())
    {
        // Connect to the first matching device.
        bool success = device.setup(devicesInfo[0], 115200);
        
        if(success)
        {
            ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[0];
        }
        else
        {
            ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[0];
        }
    }
    else
    {
        ofLogNotice("ofApp::setup") << "No devices connected.";
    }
}

void ofApp::serialUpdate(){
    try
    {
        // Read all bytes from the device;
        uint8_t buffer[1024];
        
        while (device.available() > 0)
        {
            std::size_t sz = device.readBytes(buffer, 1024);
            
            for (int i = 0; i < sz; ++i)
            {
                std::cout << buffer[i];
            }
        }
        if (charKey == 'f' || charKey == 'b') {
            std::string text;
            if (charKey == 'f' ) {
                text = ofToString("G0 X 10");
            }else{
                text = ofToString("G0 X 0");
            }
            
            ofx::IO::ByteBuffer textBuffer(text);
            
            device.writeBytes(textBuffer);
            device.writeByte('\n');
            charKey = '0';
        }
        // Send some new bytes to the device to have them echo'd back.
        
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::update") << exc.what();
    }
    
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'f'){
        charKey = 'f';
    }
    if(key == 'b'){
        charKey = 'b';
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
