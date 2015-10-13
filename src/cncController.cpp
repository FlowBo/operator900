//
//  cncController.cpp
//  findColorArray
//
//  Created by Florian Born on 12/10/15.
//
//

#include "cncController.hpp"

cncController::cncController(){};

void cncController::setup(int boudrate){
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
    ofLogNotice("ofApp::setup") << "Connected Devices: ";
    
    for (std::size_t i = 0; i < devicesInfo.size(); ++i)
    {
        ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
    }
    
    if (!devicesInfo.empty())
    {
        // Connect to the first matching device.
        bool success = device.setup(devicesInfo[0], boudrate);
        
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
void cncController::sendGCode(std::string gcode){
    try
    {
        uint8_t buffer[1024];
        while (device.available() > 0)
        {
            std::size_t sz = device.readBytes(buffer, 1024);
            for (int i = 0; i < sz; ++i)
            {
                std::cout << buffer[i];
            }
        }
            ofx::IO::ByteBuffer textBuffer(gcode);
            device.writeBytes(textBuffer);
            device.writeByte('\n');
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::update") << exc.what();
    }
}
