//  ofProtonect.cpp
//
//  Created by Theodore Watson on 11/16/15


#include "ofMain.h"

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <transfer_pool.h>
#include <event_loop.h>
#include "ofRGBPacketProcessor.h"

#include "ofAppGLFWWindow.h"
#include "ofAppRunner.h"

class ofProtonect{

    public:
  
        ofProtonect();
    
        int openKinect(std::string serialNo);
//        void updateKinect(ofPixels & rgbPixels, ofFloatPixels & depthPixels, vector <ofPoint> & pcPoints);
//        void updateKinect(ofPixels & rgbPixels, ofFloatPixels & depthPixels, vector <ofPoint> &pcPoints, vector<ofColor> & pcColors);
//        void updateKinect(ofPixels & rgbPixels, ofFloatPixels & depthPixels, ofFloatPixels & bigdepthPixels ,vector <ofPoint> &pcPoints, vector<ofColor> & pcColors);
        void updateKinect(ofPixels & rgbPixels, ofFloatPixels & depthPixels, ofFloatPixels & bigdepthPixels);
        int closeKinect();
    
        libfreenect2::Freenect2 & getFreenect2Instance(){
            return freenect2;
        }
  
  
        //---------
        // Added by kidapu
        void apply(int dx, int dy, float dz, float &cx, float &cy);
  
  
//        libfreenect2::Freenect2Device::ColorCameraParams getColorCameraParams() {
//            if (!dev) {return;}
//            return dev->getColorCameraParams();
//        }
//        libfreenect2::Freenect2Device::IrCameraParams getIrCameraParams() {
//            if (!dev) {return;}
//            return dev->getIrCameraParams();
//        }
  
        

  
    protected:
  
        bool bOpened;
        
        libfreenect2::Freenect2 freenect2;

        libfreenect2::Freenect2Device *dev = 0;
        libfreenect2::PacketPipeline *pipeline = 0;

        libfreenect2::FrameMap frames;

        libfreenect2::Registration* registration;
        libfreenect2::SyncMultiFrameListener * listener;
        libfreenect2::Frame * undistorted = NULL;
        libfreenect2::Frame * registered = NULL;
//        libfreenect2::Frame  * rgb4 = NULL;
        libfreenect2::Frame  * bigFrame = NULL;

};
