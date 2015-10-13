#include "ofApp.h"
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
    ofSetVerticalSync(true);
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
    videoController.update();
//    unsigned char * pixels = video.getPixels();
    //update Color tracking
//    cArray.videoUpdate(pixels);
//    cArray.calculateColorArray();
    
//    serialUpdate();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);
    videoController.draw();
    ofFill();
    cArray.draw();
    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 710);
    gui.draw();
    
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
